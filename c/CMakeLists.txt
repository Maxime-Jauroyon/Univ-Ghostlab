cmake_minimum_required(VERSION 3.14)

project(ghostlab VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEPS_SRC
        deps/imgui/imconfig.h
        deps/imgui/imgui.cpp
        deps/imgui/imgui.h
        deps/imgui/imgui_demo.cpp
        deps/imgui/imgui_draw.cpp
        deps/imgui/imgui_internal.h
        deps/imgui/imgui_tables.cpp
        deps/imgui/imgui_widgets.cpp
        deps/imgui/imstb_rectpack.h
        deps/imgui/imstb_textedit.h
        deps/imgui/imstb_truetype.h
        deps/imtui/imtui.h
        deps/imtui/imtui-demo.cpp
        deps/imtui/imtui-demo.h
        deps/imtui/imtui-impl-ncurses.cpp
        deps/imtui/imtui-impl-ncurses.h
        deps/imtui/imtui-impl-text.cpp
        deps/imtui/imtui-impl-text.h
        deps/cimgui/cimgui.cpp
        deps/cimgui/cimgui.h
        deps/cimtui/cimtui.cpp
        deps/cimtui/cimtui.h)

set(COMMON_SRC
        src/common/array.c
        src/common/array.h
        src/common/command.c
        src/common/command.h
        src/common/gui.c
        src/common/gui.h
        src/common/log.c
        src/common/log.h
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
        src/server/main.c
        src/server/command.c
        src/server/command.h
        src/server/shared.c
        src/server/shared.h
        src/server/thread_connection.c
        src/server/thread_connection.h
        src/server/thread_matches.c
        src/server/thread_matches.h
        src/server/thread_multicast.c
        src/server/thread_multicast.h
        src/server/thread_tcp.c
        src/server/thread_tcp.h)

set(CLIENT_SRC
        src/client/main.c
        src/client/command.c
        src/client/command.h
        src/client/message.c
        src/client/message.h
        src/client/shared.c
        src/client/shared.h
        src/client/thread_multicast.c
        src/client/thread_multicast.h
        src/client/thread_udp.c
        src/client/thread_udp.h)

set(LIBS ncurses)
set(INCLUDE_DIRS ${CURSES_INCLUDE_DIRS} deps src)

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(LIBS ${LIBS} Threads::Threads)
endif()

add_executable(server
        ${SERVER_SRC}
        ${COMMON_SRC}
        ${DEPS_SRC})
target_link_libraries(server PRIVATE ${LIBS})
target_include_directories(server PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(server PRIVATE GHOSTLAB_SERVER)

add_executable(client
        ${CLIENT_SRC}
        ${COMMON_SRC}
        ${DEPS_SRC})
target_link_libraries(client PRIVATE ${LIBS})
target_include_directories(client PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(client PRIVATE GHOSTLAB_CLIENT)
