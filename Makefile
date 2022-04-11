.PHONY: all distclean server client

CC = gcc
CCFLAGS = -Wall -std=gnu99

COMMON_SRC = \
	src/main/c/array.c \
    src/main/c/array.h \
    src/main/c/message.c \
    src/main/c/message.h \
    src/main/c/string.c \
    src/main/c/string.h \
    src/main/c/types.h \
    src/main/c/utils.c \
    src/main/c/utils.h

ifeq ($(shell uname), Linux)
	COMMON_LIB = -pthread
endif

all: server client

server:
	$(CC) $(CCFLAGS) $(COMMON_LIB) $(COMMON_SRC) src/main/c/server.c -DGHOSTLAB_SERVER -o server

client:
	$(CC) $(CCFLAGS) $(COMMON_LIB) $(COMMON_SRC) src/main/c/client.c -DGHOSTLAB_CLIENT -o client

distclean:
	rm server client
