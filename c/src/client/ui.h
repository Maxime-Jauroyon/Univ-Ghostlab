#ifndef GHOSTLAB_CLIENT_UI_H
#define GHOSTLAB_CLIENT_UI_H

#include <common/types.h>

struct gl_game_t;

// Draws the client's UI.
void gl_client_draw();

// Draws the client's main window.
void gl_client_main_window_draw();

// Draws the client main window's menu bar.
void gl_client_main_window_menu_bar_draw();

// Draws the client main window's game data.
void gl_client_main_window_game_data_draw(struct gl_game_t *game, bool show_player);

// Draws the client main window's console.
void gl_client_console_window_draw();

// Draws the client's create game popup.
void gl_client_create_game_popup_draw();

// Draws the client's join game popup.
void gl_client_join_game_popup_draw();

// Draws the client's server down game popup.
void gl_client_server_down_popup_draw();

// Draws the client's game over popup.
void gl_client_game_over_popup_draw();

// Shows the client's game over popup.
void gl_client_game_over_popup_show();

// Closes the client's create game popup.
void gl_client_create_game_popup_close();

// Closes the client's join game popup.
void gl_client_join_game_popup_close();

// Sets the client's UI error to show.
void gl_client_ui_error(uint32_t error);

#endif /* GHOSTLAB_CLIENT_UI_H */
