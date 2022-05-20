cmake_minimum_required(VERSION 3.14)

project(ghostlab VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEPS_SRC
        deps/cimgui/cimgui.cpp
        deps/cimgui/cimgui.h)
set(DEPS_TUI_SRC
        deps/imgui/tui/imconfig.h
        deps/imgui/tui/imgui.cpp
        deps/imgui/tui/imgui.h
        deps/imgui/tui/imgui_demo.cpp
        deps/imgui/tui/imgui_draw.cpp
        deps/imgui/tui/imgui_internal.h
        deps/imgui/tui/imgui_tables.cpp
        deps/imgui/tui/imgui_widgets.cpp
        deps/imgui/tui/imstb_rectpack.h
        deps/imgui/tui/imstb_textedit.h
        deps/imgui/tui/imstb_truetype.h
        deps/imtui/imtui.h
        deps/imtui/imtui-demo.cpp
        deps/imtui/imtui-demo.h
        deps/imtui/imtui-impl-ncurses.cpp
        deps/imtui/imtui-impl-ncurses.h
        deps/imtui/imtui-impl-text.cpp
        deps/imtui/imtui-impl-text.h
        deps/cimtui/cimtui.cpp
        deps/cimtui/cimtui.h)
set(DEPS_GUI_SRC
        deps/glad/gl.c
        deps/glad/gl.h
        deps/glad/khrplatform.h
        deps/imgui/gui/imconfig.h
        deps/imgui/gui/imgui.cpp
        deps/imgui/gui/imgui.h
        deps/imgui/gui/imgui_demo.cpp
        deps/imgui/gui/imgui_draw.cpp
        deps/imgui/gui/imgui_impl_opengl3.cpp
        deps/imgui/gui/imgui_impl_opengl3.h
        deps/imgui/gui/imgui_impl_sdl.cpp
        deps/imgui/gui/imgui_impl_sdl.h
        deps/imgui/gui/imgui_internal.h
        deps/imgui/gui/imgui_tables.cpp
        deps/imgui/gui/imgui_widgets.cpp
        deps/imgui/gui/imstb_rectpack.h
        deps/imgui/gui/imstb_textedit.h
        deps/imgui/gui/imstb_truetype.h
        deps/imgui/gui/fonts/firacode.c
        deps/imgui/gui/fonts/firacode.h
        deps/imgui/gui/fonts/sourcesanspro.c
        deps/imgui/gui/fonts/sourcesanspro.h)
set(COMMON_SRC
        src/common/array.c
        src/common/array.h
        src/common/command.c
        src/common/command.h
        src/common/game.c
        src/common/game.h
        src/common/ui.c
        src/common/ui.h
        src/common/log.c
        src/common/log.h
        src/common/maze.c
        src/common/maze.h
        src/common/memory.c
        src/common/memory.h
        src/common/message.c
        src/common/message.h
        src/common/network.c
        src/common/network.h
        src/common/string.c
        src/common/string.h
        src/common/types.h
        src/common/utils.c
        src/common/utils.h)
set(SERVER_SRC
        src/server/command.c
        src/server/command.h
        src/server/ui.c
        src/server/ui.h
        src/server/main.c
        src/server/message.c
        src/server/message.h
        src/server/shared.c
        src/server/shared.h
        src/server/thread_ghosts_handler.c
        src/server/thread_ghosts_handler.h
        src/server/thread_tcp_acceptor.c
        src/server/thread_tcp_acceptor.h
        src/server/thread_tcp_listener.c
        src/server/thread_tcp_listener.h)
set(CLIENT_SRC
        src/client/command.c
        src/client/command.h
        src/client/ui.c
        src/client/ui.h
        src/client/main.c
        src/client/message.c
        src/client/message.h
        src/client/shared.c
        src/client/shared.h
        src/client/thread_multicast_game_listener.c
        src/client/thread_multicast_game_listener.h
        src/client/thread_multicast_general_listener.c
        src/client/thread_multicast_general_listener.h
        src/client/thread_tcp_listener.c
        src/client/thread_tcp_listener.h
        src/client/thread_udp_listener.c
        src/client/thread_udp_listener.h)

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(DEPS_LIBS Threads::Threads ${CMAKE_DL_LIBS})
endif()
find_package(Curses REQUIRED)
set(DEPS_TUI_LIBS ncurses)
find_package(SDL2 REQUIRED)
set(DEPS_GUI_LIBS ${SDL2_LIBRARIES})

set(INCLUDE_DIRS ${CURSES_INCLUDE_DIRS} deps src)
set(DEPS_GUI_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})

set(DEPS_TUI_DEFINITIONS GHOSTLAB_TUI=1)
set(DEPS_GUI_DEFINITIONS GHOSTLAB_GUI=1)
set(SERVER_DEFINITIONS GHOSTLAB_SERVER=1)
set(CLIENT_DEFINITIONS GHOSTLAB_CLIENT=1)

add_executable(server_tui ${SERVER_SRC} ${COMMON_SRC} ${DEPS_SRC} ${DEPS_TUI_SRC})
target_link_libraries(server_tui PRIVATE ${DEPS_LIBS} ${DEPS_TUI_LIBS})
target_include_directories(server_tui PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(server_tui PRIVATE ${SERVER_DEFINITIONS} ${DEPS_TUI_DEFINITIONS})

add_executable(client_tui ${CLIENT_SRC} ${COMMON_SRC} ${DEPS_SRC} ${DEPS_TUI_SRC})
target_link_libraries(client_tui PRIVATE ${DEPS_LIBS} ${DEPS_TUI_LIBS})
target_include_directories(client_tui PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(client_tui PRIVATE ${CLIENT_DEFINITIONS} ${DEPS_TUI_DEFINITIONS})

add_executable(server_gui ${SERVER_SRC} ${COMMON_SRC} ${DEPS_SRC} ${DEPS_GUI_SRC})
target_link_libraries(server_gui PRIVATE ${DEPS_LIBS} ${DEPS_GUI_LIBS})
target_include_directories(server_gui PRIVATE ${INCLUDE_DIRS} ${DEPS_GUI_INCLUDE_DIRS})
target_compile_definitions(server_gui PRIVATE ${SERVER_DEFINITIONS} ${DEPS_GUI_DEFINITIONS})

add_executable(client_gui ${CLIENT_SRC} ${COMMON_SRC} ${DEPS_SRC} ${DEPS_GUI_SRC})
target_link_libraries(client_gui PRIVATE ${DEPS_LIBS} ${DEPS_GUI_LIBS})
target_include_directories(client_gui PRIVATE ${INCLUDE_DIRS} ${DEPS_GUI_INCLUDE_DIRS})
target_compile_definitions(client_gui PRIVATE ${CLIENT_DEFINITIONS} ${DEPS_GUI_DEFINITIONS})
