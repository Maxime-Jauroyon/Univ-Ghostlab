cmake_minimum_required(VERSION 3.14)

project(ghostlab VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(COMMON_LIB "")
set(COMMON_SRC
        src/main/c/array.h
        src/main/c/message.c
        src/main/c/message.h
        src/main/c/types.h
        src/main/c/utils.c
        src/main/c/utils.h)

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(COMMON_LIB Threads::Threads)
endif()

add_executable(client src/main/c/client.c ${COMMON_SRC})
target_compile_definitions(client PRIVATE GHOSTLAB_CLIENT)
target_link_libraries(client PRIVATE ${COMMON_LIB})

add_executable(server src/main/c/server.c ${COMMON_SRC})
target_compile_definitions(server PRIVATE GHOSTLAB_SERVER)
target_link_libraries(server PRIVATE ${COMMON_LIB})
