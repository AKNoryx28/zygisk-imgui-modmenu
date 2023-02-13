#ifndef ZYGISK_IMGUI_MODMENU_LOG_H
#define ZYGISK_IMGUI_MODMENU_LOG_H

#include <android/log.h>

#define LOG_TAG "Askan"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#endif //ZYGISK_IMGUI_MODMENU_LOG_H
