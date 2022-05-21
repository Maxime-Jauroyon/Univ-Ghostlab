#if GHOSTLAB_TUI || GHOSTLAB_GUI

#include <common/ui.h>
#include <string.h>
#include <cimgui/cimgui.h>
#if GHOSTLAB_TUI
#include <cimtui/cimtui.h>
#elif GHOSTLAB_GUI
#include <SDL.h>
#include <glad/gl.h>
#include <imgui/gui/imgui_impl_sdl.h>
#include <imgui/gui/imgui_impl_opengl3.h>
#include <imgui/gui/fonts/sourcesanspro.h>
#include <imgui/gui/fonts/firacode.h>
#endif
#include <common/log.h>
#include <common/array.h>
#include <common/utils.h>
#include <common/command.h>

static float g_current_pos_y = 0.0f;
static char g_console_buf[512] = { 0 };
static bool g_console_show_info = true;
static bool g_console_show_warning = true;
static bool g_console_show_error = true;
static bool g_first_time = true;
static bool g_started = false;
static uint32_t g_start_time;
static uint32_t g_end_time;
#if GHOSTLAB_GUI
static SDL_Window *g_window;
static SDL_GLContext gl_context;
ImFontConfig *g_fontConfig;
#endif

int32_t gl_ui_create(const char *gui_title) {
#if GHOSTLAB_GUI
    gl_start_ticking();
#endif
    
    gl_assert(igDebugCheckVersionAndDataLayout(igGetVersion(), sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx)));
    igCreateContext(0);
    
    ImGuiIO *io = igGetIO();
    io->IniFilename = 0;
    
    igStyleColorsDark(0);
    
#if GHOSTLAB_TUI
    igTuiInit();
#elif GHOSTLAB_GUI
    gl_assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) == 0);

#ifdef __APPLE__
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI );
    g_window = SDL_CreateWindow(gui_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    gl_context = SDL_GL_CreateContext(g_window);
    SDL_GL_MakeCurrent(g_window, gl_context);
    SDL_GL_SetSwapInterval(1);
    
    gl_assert(gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) != 0);
    
    ImGui_ImplSDL2_InitForOpenGL(g_window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    g_fontConfig = ImFontConfig_ImFontConfig();
    g_fontConfig->RasterizerMultiply = 1.5f;
    g_fontConfig->OversampleH = 4;
    g_fontConfig->OversampleV = 4;
    ImFontAtlas_AddFontFromMemoryCompressedTTF(io->Fonts, SourceSansProRegular_compressed_data, (int)SourceSansProRegular_compressed_size, 16.0f, g_fontConfig, 0);
    io->FontDefault = io->Fonts->Fonts.Data[io->Fonts->Fonts.Size - 1];
    ImFontAtlas_AddFontFromMemoryCompressedTTF(io->Fonts, FiraCodeRegular_compressed_data, (int)FiraCodeRegular_compressed_size, 16.0f, g_fontConfig, 0);
#endif
    
    g_started = true;
    
    return 0;
}

void gl_ui_start_render(bool *quit) {
    g_start_time = gl_get_ticks();
    
    g_current_pos_y = 0.0f;

#if GHOSTLAB_TUI
    igTuiNewFrame();
#elif GHOSTLAB_GUI
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            *quit = true;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(g_window)) {
            *quit = true;
        }
    }
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(g_window);
#endif
    
    igNewFrame();
}

void gl_ui_end_render() {
    igRender();
    
#if GHOSTLAB_TUI
    igTuiRender();
#elif GHOSTLAB_GUI
    ImGuiIO *io = igGetIO();
    glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (igGetDrawData() != 0) {
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
    }
    SDL_GL_SwapWindow(g_window);
#endif

    gl_ui_wait_before_next_render();
}

void gl_ui_free() {
#if GHOSTLAB_TUI
    igTuiShutdown();
#elif GHOSTLAB_GUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
#endif
    
    if (igGetCurrentContext()) {
        igDestroyContext(igGetCurrentContext());
#if GHOSTLAB_GUI
        ImFontConfig_destroy(g_fontConfig);
#endif
    }

#if GHOSTLAB_GUI
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
#endif
}

void gl_ui_wait_before_next_render() {
    g_end_time = gl_get_ticks() - g_start_time;
    
    if (g_end_time < 0) {
        return;
    }
    
    int32_t sleep_duration = 16 - (int32_t)g_end_time;
    if (sleep_duration > 0) {
        gl_sleep(sleep_duration);
    }
}

void gl_igBegin(const char *title, float height) {
    ImGuiIO *io = igGetIO();
    
    igSetNextWindowPos((ImVec2) { 0, io->DisplaySize.y * g_current_pos_y }, ImGuiCond_Always, (ImVec2) { 0 });
#if GHOSTLAB_TUI
    igSetNextWindowSize((ImVec2) { io->DisplaySize.x, io->DisplaySize.y * height + (g_current_pos_y > 0.f ? 1.f : 0.f) }, ImGuiCond_Always);
#elif GHOSTLAB_GUI
    igSetNextWindowSize((ImVec2) { io->DisplaySize.x, io->DisplaySize.y * height + 1 }, ImGuiCond_Always);
#endif
    igBegin(title, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);
    g_current_pos_y += height;
}

void gl_igConsole(const struct gl_command_definition_t **cmd_defs, uint32_t cmd_defs_count) {
    ImGuiIO *io = igGetIO();
    
    if (g_first_time) {
        gl_log_push("you can now enter commands.\n");
        cmd_defs[0]->function(0);
        g_first_time = false;
    }
    
    float height = 1.0f - g_current_pos_y;
    
    gl_igBegin("Console", height);
    
    if (igBeginMenuBar()) {
        if (igBeginMenu("View", true)) {
            if (igMenuItemBool("Clear Logs", 0, false, true)) {
                gl_log_free();
            }
            
            igSeparator();
            
            igMenuItemBoolPtr("Show Info", 0, &g_console_show_info, true);
            igMenuItemBoolPtr("Show Warnings", 0, &g_console_show_warning, true);
            igMenuItemBoolPtr("Show Errors", 0, &g_console_show_error, true);
            
            igEndMenu();
        }
        
        igEndMenuBar();
    }
    
    igText("Enter a command:");
    
    igSameLine(0, -1);
    
    if (igInputText("", g_console_buf, 512, ImGuiInputTextFlags_EnterReturnsTrue, 0, 0) && strcmp(g_console_buf, "") != 0) {
        gl_execute_command(g_console_buf, 512, cmd_defs, cmd_defs_count, true);
    }
    
    igSameLine(0, -1);
    
    if (igButton("Send", (ImVec2) { 0, 0 }) && strcmp(g_console_buf, "") != 0) {
        gl_execute_command(g_console_buf, 512, cmd_defs, cmd_defs_count, true);
    }
    
    igBeginChildStr("#logs_output", (ImVec2) { 0, 0 }, true, ImGuiWindowFlags_HorizontalScrollbar);
    igPushFont(io->Fonts->Fonts.Data[io->Fonts->Fonts.Size - 1]);
    
    ImGuiListClipper clipper = { 0 };
    ImGuiListClipper_Begin(&clipper, gl_array_get_size(gl_logs()), -1);
    while (ImGuiListClipper_Step(&clipper)) {
        for (uint32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
            gl_log_t* log = gl_logs()[i];
            
            if (log->type == GL_LOG_TYPE_DEFAULT) {
                igTextUnformatted(log->data, 0);
            } else if (log->type == GL_LOG_TYPE_INFO && g_console_show_info) {
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 0.00f, 1.00f, 1.00f, 1.00f });
                igTextUnformatted(log->data, 0);
                igPopStyleColor(1);
            } else if (log->type == GL_LOG_TYPE_WARNING && g_console_show_warning) {
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 1.00f, 1.00f, 0.00f, 1.00f });
                igTextUnformatted(log->data, 0);
                igPopStyleColor(1);
            } else if (log->type == GL_LOG_TYPE_ERROR && g_console_show_error) {
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 1.00f, 0.00f, 0.00f, 1.00f });
                igTextUnformatted(log->data, 0);
                igPopStyleColor(1);
            } else if (log->type == GL_LOG_TYPE_USER) {
                igPushStyleColorVec4(ImGuiCol_Text, (ImVec4) { 0.00f, 1.00f, 0.00f, 1.00f });
                igTextUnformatted(log->data, 0);
                igPopStyleColor(1);
            }
        }
    }
    
    if (gl_log_should_scroll()) {
        igSetScrollHereX(1.0f);
        igSetScrollHereY(1.0f);
    }
    
    igPopFont();
    igEndChild();
    
    igEnd();
}

int32_t gl_igFilter(ImGuiInputTextCallbackData *data) {
    char e = (char)data->EventChar;
    
    if (e >= 33 && e <= 126) {
        return 0;
    }
    
    return 1;
}

int32_t gl_igFilterWithSpaces(ImGuiInputTextCallbackData *data) {
    char e = (char)data->EventChar;
    
    if (e >= 32 && e <= 126) {
        return 0;
    }
    
    return 1;
}

int32_t gl_igFilterNumbers(ImGuiInputTextCallbackData *data) {
    char e = (char)data->EventChar;
    
    if (e >= 48 && e <= 57) {
        return 0;
    }
    
    return 1;
}

bool gl_ui_started() {
    return g_started;
}

#endif
