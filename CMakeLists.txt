cmake_minimum_required(VERSION 3.14)

project(ghostlab VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)

set(COMMON_SRC
        src/main/c/deps/imgui/imconfig.h
        src/main/c/deps/imgui/imgui.cpp
        src/main/c/deps/imgui/imgui.h
        src/main/c/deps/imgui/imgui_demo.cpp
        src/main/c/deps/imgui/imgui_draw.cpp
        src/main/c/deps/imgui/imgui_internal.h
        src/main/c/deps/imgui/imgui_tables.cpp
        src/main/c/deps/imgui/imgui_widgets.cpp
        src/main/c/deps/imgui/imstb_rectpack.h
        src/main/c/deps/imgui/imstb_textedit.h
        src/main/c/deps/imgui/imstb_truetype.h
        src/main/c/deps/imtui/cimtui.cpp
        src/main/c/deps/imtui/cimtui.h
        src/main/c/deps/imtui/imtui.h
        src/main/c/deps/imtui/imtui-impl-ncurses.cpp
        src/main/c/deps/imtui/imtui-impl-ncurses.h
        src/main/c/deps/imtui/imtui-impl-text.cpp
        src/main/c/deps/imtui/imtui-impl-text.h
        src/main/c/deps/cimgui/cimgui.cpp
        src/main/c/deps/cimgui/cimgui.h
        src/main/c/array.c
        src/main/c/array.h
        src/main/c/command.c
        src/main/c/command.h
        src/main/c/memory.c
        src/main/c/memory.h
        src/main/c/message.c
        src/main/c/message.h
        src/main/c/network.c
        src/main/c/network.h
        src/main/c/print.c
        src/main/c/print.h
        src/main/c/string.c
        src/main/c/string.h
        src/main/c/types.h
        src/main/c/utils.c
        src/main/c/utils.h)

find_package(Curses REQUIRED)
set(COMMON_LIB ${CURSES_LIBRARIES})
set(COMMON_INCLUDE_DIRS ${CURSES_INCLUDE_DIRS})

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(COMMON_LIB ${COMMON_LIB} Threads::Threads)
endif()

add_executable(server
        src/main/c/server.c
        src/main/c/server_command.c
        src/main/c/server_command.h
        src/main/c/server_common.c
        src/main/c/server_common.h
        src/main/c/server_thread_client_listener.c
        src/main/c/server_thread_client_listener.h
        src/main/c/server_thread_clients_acceptor.c
        src/main/c/server_thread_clients_acceptor.h
        src/main/c/server_thread_game.c
        src/main/c/server_thread_game.h
        src/main/c/server_thread_multicast_sender.c
        src/main/c/server_thread_multicast_sender.h
        ${COMMON_SRC})
target_link_directories(server PRIVATE /usr/local/opt/ncurses/lib)
target_link_libraries(server PRIVATE ${COMMON_LIB})
target_include_directories(server PRIVATE ${COMMON_INCLUDE_DIRS})
target_compile_definitions(server PRIVATE GHOSTLAB_SERVER)

add_executable(client
        src/main/c/client.c
        src/main/c/client_common.c
        src/main/c/client_common.h
        ${COMMON_SRC})
target_link_directories(client PRIVATE /usr/local/opt/ncurses/lib)
target_link_libraries(client PRIVATE ${COMMON_LIB})
target_include_directories(client PRIVATE ${COMMON_INCLUDE_DIRS})
target_compile_definitions(client PRIVATE GHOSTLAB_CLIENT)
