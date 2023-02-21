#include "utils.h"
#include "log.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <sys/system_properties.h>

pid_t __target_pid__ = -1;

ssize_t process_v(pid_t __pid, const struct iovec *__local_iov,
                  unsigned long __local_iov_count, const struct iovec *__remote_iov,
                  unsigned long __remote_iov_count, unsigned long __flags, bool iswrite)
{
    return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

bool pvm(void *addr, void *buff, size_t size, bool is_write)
{
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buff;
    local[0].iov_len = size;
    remote[0].iov_base = addr;
    remote[0].iov_len = size;

    if (__target_pid__ == -1)
        __target_pid__ = getpid();

    ssize_t bytes = process_v(__target_pid__, local, 1, remote, 1, 0, is_write);
    return bytes == size;
}

namespace utils {
    int get_android_api_level() {
        char prop_value[PROP_VALUE_MAX];
        __system_property_get("ro.build.version.sdk", prop_value);
        return atoi(prop_value);
    }

    bool read_address(void *address, void *out_buffer, size_t length) {
        unsigned long page_size = sysconf(_SC_PAGE_SIZE);
        unsigned long size = page_size * sizeof(uintptr_t);
        return mprotect(
               (void *)((uintptr_t)address - ((uintptr_t)address % page_size) - page_size),
               (size_t)size, PROT_EXEC | PROT_READ | PROT_WRITE) == 0 && 
               memcpy(out_buffer, address, length) != 0;
    }

    bool write_address(void *address, void *in_buffer, size_t length) {
        unsigned long page_size = sysconf(_SC_PAGE_SIZE);
        unsigned long size = page_size * sizeof(uintptr_t);
        return mprotect(
               (void *)((uintptr_t)address - ((uintptr_t)address % page_size) - page_size),
               (size_t)size, PROT_EXEC | PROT_READ | PROT_WRITE) == 0 &&
               memcpy(address, in_buffer, length) != 0;
    }

    module_info find_module(const char *name) {
        module_info module{};
        char line[512];

        FILE *f = fopen("/proc/self/maps", "rt");

        if (!f) return module;

        while (fgets(line, sizeof line, f)) {
            uintptr_t tmpBase;
            uintptr_t tmpEnd;
            char tmpName[256];
            char tmpPerms[4]; 
            if (sscanf(line, "%" PRIXPTR "-%" PRIXPTR "%s %*s %*s %*s %s", &tmpBase, &tmpEnd, tmpPerms, tmpName) > 0) {
                if (!strcmp(basename(tmpName), name) && strstr(tmpPerms, "xp") != NULL) {
                    module.start_address = reinterpret_cast<void*>(tmpBase);
                    module.end_address = reinterpret_cast<void*>(tmpEnd);
                    module.size = tmpEnd - tmpBase;
                    module.name = std::string(tmpName);
                    break;
                }
            }
        }
        fclose(f);
        return module;
    }
    
    uintptr_t find_pattern(uint8_t* start, const size_t length, const char* pattern) {
        const char* pat = pattern;
        uint8_t* first_match = 0;
        for (auto current_byte = start; current_byte < (start + length); ++current_byte) {
            if (*pat == '?' || *current_byte == strtoul(pat, NULL, 16)) {
                if (!first_match)
                    first_match = current_byte;
                if (!pat[2])
                    return (uintptr_t)first_match;
                pat += *(uint16_t*)pat == 16191 || *pat != '?' ? 3 : 2;
            }
            else if (first_match) {
                current_byte = first_match;
                pat = pattern;
                first_match = 0;
            }
        } return 0;
    }
    uintptr_t find_pattern_in_module(const char* lib_name, const char* pattern) {
        module_info mod = find_module(lib_name);
        return find_pattern((uint8_t*)mod.start_address, mod.size, pattern);
    }

    int hook(void *target, func_t replace, func_t *backup) {
        unsigned long page_size = sysconf(_SC_PAGE_SIZE);
        unsigned long size = page_size * sizeof(uintptr_t);
        void *p = (void *)((uintptr_t)target - ((uintptr_t)target % page_size) - page_size);
        if (mprotect(p, (size_t)size, PROT_EXEC | PROT_READ | PROT_WRITE) == 0) {
            return DobbyHook(target, replace, backup);
        }
        return -1;
    }
}