.PHONY: all server client distclean

CC = gcc
CCFLAGS = -Wall -Wno-unused-label -Wno-unused-value -Wno-implicit-function-declaration -std=gnu99

COMMON_SRC = \
        src/main/c/deps/imgui/imconfig.h \
        src/main/c/deps/imgui/imgui.cpp \
        src/main/c/deps/imgui/imgui.h \
        src/main/c/deps/imgui/imgui_demo.cpp \
        src/main/c/deps/imgui/imgui_draw.cpp \
        src/main/c/deps/imgui/imgui_internal.h \
        src/main/c/deps/imgui/imgui_tables.cpp \
        src/main/c/deps/imgui/imgui_widgets.cpp \
        src/main/c/deps/imgui/imstb_rectpack.h \
        src/main/c/deps/imgui/imstb_textedit.h \
        src/main/c/deps/imgui/imstb_truetype.h \
        src/main/c/deps/imtui/cimtui.cpp \
        src/main/c/deps/imtui/cimtui.h \
        src/main/c/deps/imtui/imtui.h \
        src/main/c/deps/imtui/imtui-impl-ncurses.cpp \
        src/main/c/deps/imtui/imtui-impl-ncurses.h \
        src/main/c/deps/imtui/imtui-impl-text.cpp \
        src/main/c/deps/imtui/imtui-impl-text.h \
        src/main/c/deps/cimgui/cimgui.cpp \
        src/main/c/deps/cimgui/cimgui.h \
        src/main/c/array.c \
        src/main/c/array.h \
        src/main/c/command.c \
        src/main/c/command.h \
        src/main/c/memory.c \
        src/main/c/memory.h \
        src/main/c/message.c \
        src/main/c/message.h \
        src/main/c/network.c \
        src/main/c/network.h \
        src/main/c/print.c \
        src/main/c/print.h \
        src/main/c/string.c \
        src/main/c/string.h \
        src/main/c/types.h \
        src/main/c/utils.c \
        src/main/c/utils.h

ifeq ($(shell uname), Linux)
	COMMON_LIB = -pthread
endif

all: server client

SERVER_SRC = \
        src/main/c/server.c \
        src/main/c/server_command.c \
        src/main/c/server_command.h \
        src/main/c/server_common.c \
        src/main/c/server_common.h \
        src/main/c/server_thread_client_listener.c \
        src/main/c/server_thread_client_listener.h \
        src/main/c/server_thread_clients_acceptor.c \
        src/main/c/server_thread_clients_acceptor.h \
        src/main/c/server_thread_game.c \
        src/main/c/server_thread_game.h \
        src/main/c/server_thread_multicast_sender.c \
        src/main/c/server_thread_multicast_sender.h

server:
	$(CC) $(CCFLAGS) $(COMMON_LIB) $(COMMON_SRC) $(SERVER_SRC) -DGHOSTLAB_SERVER -o server

CLIENT_SRC = \
        src/main/c/client.c \
        src/main/c/client_common.c \
        src/main/c/client_common.h \

client:
	$(CC) $(CCFLAGS) $(COMMON_LIB) $(COMMON_SRC) $(CLIENT_SRC) -DGHOSTLAB_CLIENT -o client

distclean:
	rm server client
