#include <common/types.h>
#include <cimgui/cimgui.h>
#include <cimtui/cimtui.h>

static bool g_quit = false;
static bool g_show_logs_window = true;
static bool g_show_logs_info = true;
static bool g_show_logs_warning = true;
static bool g_show_logs_error = true;

const char *g_test[] = {
    "[server] [error] Hello, World 1!",
    "[server] [error] Hello, World 2!",
    "[server] [error] Hello, World 3!",
    "[server] [error] Hello, World 4!",
    "[server] [error] Hello, World 5!",
    "[server] [error] Hello, World 6!",
    "[server] [error] Hello, World 7!",
    "[server] [error] Hello, World 8!",
    "[server] [error] Hello, World 9!",
    "[server] [error] Hello, World 10!",
    "[server] [error] Hello, World 11!",
    "[server] [error] Hello, World 12!",
    "[server] [error] Hello, World 13!",
    "[server] [error] Hello, World 14!",
    "[server] [error] Hello, World 15!",
    "[server] [error] Hello, World 16!",
    "[server] [error] Hello, World 17!",
    "[server] [error] Hello, World! 18",
    "[server] [error] Hello, World! 19",
    "[server] [error] Hello, World! 20",
    "[server] [error] Hello, World! 21",
    "[server] [error] Hello, World! 22",
    "[server] [error] Hello, World! 23",
    "[server] [error] Hello, World! 24",
    "[server] [error] Hello, World! 25",
    "[server] [error] Hello, World! 26",
    "[server] [error] Hello, World! 27",
    "[server] [error] Hello, World! 28",
    "[server] [error] Hello, World! 29",
    "[server] [error] Hello, World! 30",
    "[server] [error] Hello, World! 31",
    "[server] [error] Hello, World! 32",
    "[server] [error] Hello, World! 33",
};

void draw() {
    ImGuiIO *io = igGetIO();
    
    {
        igSetNextWindowPos((ImVec2) {0, 0}, ImGuiCond_Always, (ImVec2) {0});
        if (g_show_logs_window) {
            igSetNextWindowSize((ImVec2) {io->DisplaySize.x, io->DisplaySize.y * 0.6f}, ImGuiCond_Always);
        } else {
            igSetNextWindowSize((ImVec2) {io->DisplaySize.x, io->DisplaySize.y}, ImGuiCond_Always);
        }
        igBegin("Ghostlab Server", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);
        
        if (igBeginMenuBar()) {
            if (igBeginMenu("Options", true)) {
                igMenuItemBoolPtr("Show Logs ", 0, &g_show_logs_window, true);
                igMenuItemBoolPtr("Quit", 0, &g_quit, true);
                igEndMenu();
            }
            igEndMenuBar();
        }
        
        igEnd();
    }
    
    if (g_show_logs_window) {
        igSetNextWindowPos((ImVec2) { 0, io->DisplaySize.y - io->DisplaySize.y * 0.4f }, ImGuiCond_Always, (ImVec2) { 0 });
        igSetNextWindowSize((ImVec2) { io->DisplaySize.x, io->DisplaySize.y * 0.4f + ((int)io->DisplaySize.y % 5 != 0 ? 1.0f : 0.0f) }, ImGuiCond_Always);
        igBegin("Logs", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);
    
        if (igBeginMenuBar()) {
            if (igBeginMenu("Options", true)) {
                if (igMenuItemBool("Clear", 0, false, true)) {
                
                }
                igMenuItemBoolPtr("Show Info ", 0, &g_show_logs_info, true);
                igMenuItemBoolPtr("Show Warnings ", 0, &g_show_logs_warning, true);
                igMenuItemBoolPtr("Show Errors ", 0, &g_show_logs_error, true);
                igEndMenu();
            }
            igEndMenuBar();
        }
        
        igBeginChildStr("logs_output", (ImVec2) { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGuiListClipper clipper = { 0 };
        ImGuiListClipper_Begin(&clipper, 33, -1);
        while (ImGuiListClipper_Step(&clipper)) {
            for (uint32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                const char* test = g_test[i];
                igTextUnformatted(test, 0);
            }
        }
        //igSetScrollHereX(1.0f);
        //igSetScrollHereY(1.0f);
        igEndChild();
        
        
        igEnd();
    }
}

int main(void) {
    igTuiInit();
    while (!g_quit) {
        igTuiNewFrame();
        draw();
        igTuiRender();
    }
    igTuiShutdown();
    return 0;
}
