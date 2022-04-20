#ifndef GHOSTLAB_CLIENT_GUI_H
#define GHOSTLAB_CLIENT_GUI_H

#include <common/types.h>

struct gl_game_t;

void gl_client_draw();

void gl_client_main_window_draw();

void gl_client_main_window_menu_bar_draw();

void gl_client_main_window_game_data_draw(struct gl_game_t *game, bool show_player);

void gl_client_console_window_draw();

void gl_client_create_game_popup_draw();

void gl_client_join_game_popup_draw();

void gl_client_server_down_popup_draw();

void gl_client_create_game_popup_close();

void gl_client_join_game_popup_close();

void gl_client_error(uint32_t error);

#endif /* GHOSTLAB_CLIENT_GUI_H */
