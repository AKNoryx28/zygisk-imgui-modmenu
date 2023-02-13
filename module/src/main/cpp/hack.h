#ifndef ZYGISK_IMGUI_MODMENU_HACK_H
#define ZYGISK_IMGUI_MODMENU_HACK_H

#define HOOKAF(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)


void hack_prepare(const char *game_data_dir);

#endif //ZYGISK_IMGUI_MODMENU_HACK_H
