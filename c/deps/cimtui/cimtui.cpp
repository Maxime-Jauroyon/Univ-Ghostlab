#include <cimtui/cimtui.h>
#include <imgui/tui/imgui.h>
#include <imtui/imtui.h>
#include <imtui/imtui-demo.h>
#include <imtui/imtui-impl-text.h>
#include <imtui/imtui-impl-ncurses.h>

static ImTui::TScreen *screen = nullptr;

void igTuiInit() {
    screen = ImTui_ImplNcurses_Init(true);
    ImTui_ImplText_Init();
}

void igTuiShutdown() {
    if (ImGui::GetCurrentContext()) {
        ImTui_ImplText_Shutdown();
        ImTui_ImplNcurses_Shutdown();
    }
}

bool igTuiNewFrame() {
    igTuiProcessEvent();
    bool r = ImTui_ImplNcurses_NewFrame();
    ImTui_ImplText_NewFrame();
    return r;
}

void igTuiRender() {
    ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
    ImTui_ImplNcurses_DrawScreen();
}

bool igTuiProcessEvent() {
    return ImTui_ImplNcurses_ProcessEvent();
}

void igTuiShowDemoWindow(bool*b) {
    ImTui::ShowDemoWindow(b);
}
