#include "types.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "deps/cimgui/cimgui.h"
#include "deps/imtui/cimtui.h"

int main(void) {
    gl_assert(igDebugCheckVersionAndDataLayout(igGetVersion(), sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx)));
    
    igCreateContext(0);
    igTuiInit();
    
    ImGuiIO *io = igGetIO();
    
    bool demo = true;
    int nframes = 0;
    float fval = 1.23f;
    
    while (1) {
        igTuiProcessEvent();
        
        igTuiNewFrame();
        igNewFrame();
    
        igSetNextWindowPos((ImVec2) { 4, 27}, ImGuiCond_Once, (ImVec2) { 0, 0 });
        igSetNextWindowSize((ImVec2) { 50, 10}, ImGuiCond_Once);
        igBegin("Hello, World!", 0, 0);
        igText("NFrames = %d", nframes++);
        igText("Mouse Pos : x = %g, y = %g", io->MousePos.x, io->MousePos.y);
        igText("Time per frame %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        igEnd();
    
        igRender();
        igTuiDrawScreen();
    }
    
    igTuiShutdown();
    igDestroyContext(0);
    
    return 0;
}
