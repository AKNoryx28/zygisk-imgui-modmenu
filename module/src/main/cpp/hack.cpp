#include "hack.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <dlfcn.h>
#include "log.h"
#include "game.h"
#include "utils.h"
#include "xdl.h"
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "EGL/egl.h"
#include <GLES3/gl3.h>

HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

static int glHeight, glWidth;
static bool is_setup = false;

void SetupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::StyleColorsLight();

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (!is_setup) {
        SetupImGui();
        is_setup = true;
    }

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

uintptr_t il2cpp_base = 0, il2cpp_end = 0;

void hack_start(const char *game_data_dir) {
    LOGI("hack start | %s", game_data_dir);
    do {
        il2cpp_base = utils::get_base_address(TargetLibName);
        il2cpp_end = utils::get_end_address(TargetLibName);
    } while (il2cpp_base == 0 || il2cpp_end == 0);
    LOGI("il2cpp: %p - %p", (void*)il2cpp_base, (void*)il2cpp_end);

}

void hack_prepare(const char *game_data_dir) {
    LOGI("hack thread: %d", gettid());
    int api_level = utils::get_android_api_level();
    LOGI("api level: %d", api_level);
    sleep(5);

    void *sym_input = DobbySymbolResolver("/system/lib/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    if (NULL != sym_input){
        DobbyHook((void *)sym_input, (dobby_dummy_func_t) myInput, (dobby_dummy_func_t*)&origInput);
    }
    
    void *egl_handle = xdl_open("libEGL.so", 0);
    void *eglSwapBuffers = xdl_sym(egl_handle, "eglSwapBuffers", nullptr);
    if (NULL != eglSwapBuffers) {
        utils::hook((void*)eglSwapBuffers, (func_t)hook_eglSwapBuffers, (func_t*)&old_eglSwapBuffers);
    }
    xdl_close(egl_handle);

    hack_start(game_data_dir);
}