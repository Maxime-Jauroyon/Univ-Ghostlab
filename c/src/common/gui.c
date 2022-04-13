#include <common/gui.h>
#include <string.h>
#include "cimtui/cimtui.h"
#include "cimgui/cimgui.h"
#include "command.h"
#include "log.h"
#include "array.h"

static float g_current_pos_y = 0.0f;
static char g_console_buf[512] = { 0 };
static bool g_console_show_info = true;
static bool g_console_show_warning = true;
static bool g_console_show_error = true;
static bool g_first_time = true;

void gl_gui_create() {
    igTuiInit();
}

void gl_gui_start_render() {
    g_current_pos_y = 0.0f;
    
    igTuiNewFrame();
}

void gl_gui_end_render() {
    igTuiRender();
}

void gl_gui_free() {
    igTuiShutdown();
}

void gl_igBegin(const char *title, float height) {
    ImGuiIO *io = igGetIO();
    
    igSetNextWindowPos((ImVec2) { 0, io->DisplaySize.y * g_current_pos_y }, ImGuiCond_Always, (ImVec2) { 0 });
    igSetNextWindowSize((ImVec2) { io->DisplaySize.x, io->DisplaySize.y * height + ((int)io->DisplaySize.y % 5 != 0 ? 1.0f : 0.0f) }, ImGuiCond_Always);
    igBegin(title, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);
    g_current_pos_y += height;
}

void gl_igConsole(const struct gl_command_definition_t **cmd_defs, uint32_t cmd_defs_count) {
    if (g_first_time) {
        gl_log_push("you can now enter commands.\n");
        cmd_defs[0]->function(0);
        g_first_time = false;
    }
    
    float height = 1.0f - g_current_pos_y;
    
    gl_igBegin("Console", height);
    
    if (igBeginMenuBar()) {
        if (igBeginMenu("Options", true)) {
            if (igMenuItemBool("Clear", 0, false, true)) {
                gl_logs_free();
            }
            
            igMenuItemBoolPtr("Show Info ", 0, &g_console_show_info, true);
            
            igMenuItemBoolPtr("Show Warnings ", 0, &g_console_show_warning, true);
            
            igMenuItemBoolPtr("Show Errors ", 0, &g_console_show_error, true);
            
            igEndMenu();
        }
        
        igEndMenuBar();
    }
    
    igText("Enter a command:");
    igSameLine(0, 1);
    if (igInputText("", g_console_buf, 512, ImGuiInputTextFlags_EnterReturnsTrue, 0, 0) && strcmp(g_console_buf, "") != 0) {
        gl_execute_command(g_console_buf, 512, cmd_defs, cmd_defs_count, true);
    }
    igSameLine(0, 1);
    if (igButton(" Send ", (ImVec2) { 0, 0 }) && strcmp(g_console_buf, "") != 0) {
        gl_execute_command(g_console_buf, 512, cmd_defs, cmd_defs_count, true);
    }
    
    igBeginChildStr("#logs_output", (ImVec2) { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar);
    
    ImGuiListClipper clipper = { 0 };
    ImGuiListClipper_Begin(&clipper, gl_array_get_size(gl_logs()), -1);
    while (ImGuiListClipper_Step(&clipper)) {
        for (uint32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
            gl_log_t* log = gl_logs()[i];
            char final_buf[512];
            
            if (log->type == GL_LOG_TYPE_DEFAULT) {
                sprintf(final_buf, "[" GHOSTLAB_EXECUTABLE_NAME "] %s", log->data);
                igText(final_buf, 0);
            } else if (log->type == GL_LOG_TYPE_INFO && g_console_show_info) {
                sprintf(final_buf, "[" GHOSTLAB_EXECUTABLE_NAME "] [info] %s", log->data);
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 0.00f, 1.00f, 1.00f, 1.00f });
                igText(final_buf, 0);
                igPopStyleColor(1);
            } else if (log->type == GL_LOG_TYPE_WARNING && g_console_show_warning) {
                sprintf(final_buf, "[" GHOSTLAB_EXECUTABLE_NAME "] [warning] %s", log->data);
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 1.00f, 1.00f, 0.00f, 1.00f });
                igText(final_buf, 0);
                igPopStyleColor(1);
            } else if (log->type == GL_LOG_TYPE_ERROR && g_console_show_error) {
                sprintf(final_buf, "[" GHOSTLAB_EXECUTABLE_NAME "] [error] %s", log->data);
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 1.00f, 0.00f, 0.00f, 1.00f });
                igText(final_buf, 0);
                igPopStyleColor(1);
            } else if (log->type == GL_LOG_TYPE_USER) {
                sprintf(final_buf, "$ %s", log->data);
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 0.00f, 1.00f, 0.00f, 1.00f });
                igText(final_buf, 0);
                igPopStyleColor(1);
            }
        }
    }
    
    if (gl_log_should_scroll()) {
        igSetScrollHereX(1.0f);
        igSetScrollHereY(1.0f);
    }
    
    igEndChild();
    
    igEnd();
}