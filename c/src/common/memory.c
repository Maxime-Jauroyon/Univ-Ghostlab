#include <common/memory.h>
#include <string.h>
#include <stdlib.h>
#include <common/log.h>
#include "cimtui/cimtui.h"
#include "cimgui/cimgui.h"
#include "gui.h"

static uint64_t g_allocated_ptrs = 0;

void *gl_malloc(uint64_t size) {
    void *ptr = malloc(size);
    
    if (ptr) {
        g_allocated_ptrs++;
    } else {
        gl_log_push_error("couldn't allocate memory!\n");
    }
    
    return ptr;
}

void *gl_calloc(uint64_t count, uint64_t size) {
    void *ptr = calloc(count, size);
    
    if (ptr) {
        g_allocated_ptrs++;
    } else {
        gl_log_push_error("couldn't zero allocate memory!\n");
    }
    
    
    return ptr;
}

void gl_free(void *ptr) {
    if (ptr) {
        g_allocated_ptrs--;
        free(ptr);
    }
    
}

void *gl_memmove(void *dst, const void *src, uint64_t size) {
    void *ptr = memmove(dst, src, size);
    
    if (!ptr) {
        gl_log_push_error("couldn't move memory!\n");
    }
    
    return ptr;
}

void *gl_memcpy(void *dst, const void *src, size_t size) {
    void *ptr = memcpy(dst, src, size);
    
    if (ptr) {
        g_allocated_ptrs++;
    } else {
        gl_log_push_error("couldn't copy memory!\n");
    }
    
    return ptr;
}

void *gl_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    
    if (new_ptr) {
        if (!ptr) {
            g_allocated_ptrs++;
        }
    } else {
        gl_log_push_error("couldn't copy memory!\n");
    }
    
    return new_ptr;
}

void *gl_strdup(void *ptr) {
    void *new_ptr = strdup(ptr);
    
    if (new_ptr) {
        g_allocated_ptrs++;
    } else {
        gl_log_push_error("couldn't duplicate string!\n");
    }
    
    return new_ptr;
}

void gl_memory_check_for_leaks() {
    bool quit = g_allocated_ptrs == 0;
    
    while (!quit) {
        gl_gui_start_render(&quit);
    
        igOpenPopup("###Leaks", 0);
        
        if (igBeginPopupModal("Memory Leaks Detected###Leaks", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
            igText(" ");
            igText("At least %lu memory leaks detected!    ", g_allocated_ptrs);
            igText(" ");
            igText("You should inspect your code:");
            igText("- Through a debugger.");
            igText("- Through Valgrind.");
            igText(" ");
            igText("Press any key to terminate the program.    ");
            igText(" ");
    
            for (int i = 0; i < 512; ++i) {
                if (igIsKeyReleased(i) || igIsMouseDown(0)) {
                    igCloseCurrentPopup();
                    quit = true;
                    break;
                }
            }
    
            igEndPopup();
        }
        
        gl_gui_end_render();
    }
}
