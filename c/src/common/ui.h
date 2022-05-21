#if GHOSTLAB_TUI || GHOSTLAB_GUI

#ifndef GHOSTLAB_UI_H
#define GHOSTLAB_UI_H

#include <common/types.h>

struct gl_command_definition_t;
struct ImGuiInputTextCallbackData;

// Creates a UI (either GUI or TUI).
int32_t gl_ui_create(const char *gui_title);

// Starts the UI render.
void gl_ui_start_render(bool *quit);

// End the UI render (will show everything on the screen).
void gl_ui_end_render();

// Frees a UI.
void gl_ui_free();

void gl_ui_wait_before_next_render();

// Begins a UI section of a given title and height.
void gl_igBegin(const char *title, float height);

// Draws a console.
void gl_igConsole(const struct gl_command_definition_t **cmd_defs, uint32_t cmd_defs_count);

// Filters an input text to allow only alphanumerics characters.
int32_t gl_igFilter(struct ImGuiInputTextCallbackData *data);

// Filters an input text to allow alphanumerics characters and spaces.
int32_t gl_igFilterWithSpaces(struct ImGuiInputTextCallbackData *data);

// Filters an input text t o allow numerics characters only.
int32_t gl_igFilterNumbers(struct ImGuiInputTextCallbackData *data);

// Checks if the UI is started.
bool gl_ui_started();

#endif /* GHOSTLAB_UI_H */

#endif
