#include "cimtui.h"
#include <imgui/imgui.h>
#include <imtui/imtui.h>
#include <imtui/imtui-impl-text.h>
#include <imtui/imtui-impl-ncurses.h>
#include <imtui/imtui-demo.h>

static ImTui::TScreen *screen = nullptr;

void igTuiInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(nullptr);
    screen = ImTui_ImplNcurses_Init(true);
    ImTui_ImplText_Init();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
}

void igTuiShutdown() {
    if (ImGui::GetCurrentContext()) {
        ImTui_ImplText_Shutdown();
        ImTui_ImplNcurses_Shutdown();
        ImGui::DestroyContext();
    }
}

bool igTuiNewFrame() {
    igTuiProcessEvent();
    bool r = ImTui_ImplNcurses_NewFrame();
    ImTui_ImplText_NewFrame();
    ImGui::NewFrame();
    return r;
}

void igTuiRender() {
    ImGui::Render();
    ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
    ImTui_ImplNcurses_DrawScreen();
}

bool igTuiProcessEvent() {
    return ImTui_ImplNcurses_ProcessEvent();
}

void igTuiShowDemoWindow(bool*b) {
    ImTui::ShowDemoWindow(b);
}