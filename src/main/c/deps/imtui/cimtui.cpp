#include "cimtui.h"
#include "imtui-impl-text.h"
#include "imtui-impl-ncurses.h"
#include "../imgui/imgui.h"
#include "imtui.h"

static ImTui::TScreen *screen = nullptr;

void igTuiInit() {
    screen = ImTui_ImplNcurses_Init(true);
    ImTui_ImplText_Init();
}

void igTuiShutdown() {
    ImTui_ImplText_Shutdown();
    ImTui_ImplNcurses_Shutdown();
}

bool igTuiNewFrame() {
    bool r = ImTui_ImplNcurses_NewFrame();
    ImTui_ImplText_NewFrame();
    return r;
}

void igTuiDrawScreen() {
    ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
    ImTui_ImplNcurses_DrawScreen();
}

bool igTuiProcessEvent() {
    return ImTui_ImplNcurses_ProcessEvent();
}

//void igTuiScreenResize(tui_screen_t *screen, int32_t pnx, int32_t pny) {
//
//}
//
//void igTuiScreenClear(tui_screen_t *screen) {
//
//}
//
//uint32_t igTuiScreenSize(tui_screen_t *screen) {
//
//}