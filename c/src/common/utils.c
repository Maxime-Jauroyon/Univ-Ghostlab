#include <common/utils.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#ifdef __APPLE__
#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#else
#include <endian.h>
#include <arpa/inet.h>
#define htonll(x) htobe64(x)
#define ntohll(x) be64toh(x)
#endif
#include <common/array.h>
#include <time.h>

static struct timespec g_start_ticks;

uint16_t gl_string_to_uint16(const uint8_t *n, gl_conversion_type_t conversion_type) {
    uint16_t r =
        (uint16_t)n[1] << 8 | // Put `n[1]` on the 8 to 16 bits of `r`;
        (uint16_t)n[0]; // Put `n[0]` on the 0 to 8 bits of `r`;
    
    if (conversion_type == GL_CONVERSION_TYPE_AUTOMATIC) { // NOLINT
        r = ntohs(r);
    } else if (conversion_type == GL_CONVERSION_TYPE_BIG_ENDIAN) {
        r = be16toh(r);
    } else if (conversion_type == GL_CONVERSION_TYPE_LITTLE_ENDIAN) {
        r = le16toh(r);
    }
    
    return r;
}

uint32_t gl_string_to_uint32(const uint8_t *n, gl_conversion_type_t conversion_type) {
    uint32_t r =
        (uint32_t)n[3] << 24 | // Put `n[3]` on the 24 to 32 bits of `r`;
        (uint32_t)n[2] << 16 | // Put `n[2]` on the 16 to 24 bits of `r`;
        (uint32_t)n[1] << 8 | // Put `n[1]` on the 8 to 16 bits of `r`;
        (uint32_t)n[0]; // Put `n[0]` on the 0 to 8 bits of `r`;
    
    if (conversion_type == GL_CONVERSION_TYPE_AUTOMATIC) { // NOLINT
        r = ntohl(r);
    } else if (conversion_type == GL_CONVERSION_TYPE_BIG_ENDIAN) {
        r = be32toh(r);
    } else if (conversion_type == GL_CONVERSION_TYPE_LITTLE_ENDIAN) {
        r = le32toh(r);
    }
    
    return r;
}

uint64_t gl_string_to_uint64(const uint8_t *n, gl_conversion_type_t conversion_type) {
    uint64_t r =
        (uint64_t)n[7] << 56 | // Put `n[7]` on the 56 to 64 bits of `r`;
        (uint64_t)n[6] << 48 | // Put `n[6]` on the 48 to 56 bits of `r`;
        (uint64_t)n[5] << 40 | // Put `n[5]` on the 40 to 48 bits of `r`;
        (uint64_t)n[4] << 32 | // Put `n[4]` on the 32 to 40 bits of `r`;
        (uint64_t)n[2] << 16 | // Put `n[3]` on the 24 to 32 bits of `r`;
        (uint32_t)n[2] << 16 | // Put `n[2]` on the 16 to 24 bits of `r`;
        (uint64_t)n[1] << 8 | // Put `n[1]` on the 8 to 16 bits of `r`;
        (uint64_t)n[0]; // Put `n[0]` on the 0 to 8 bits of `r`;
    
    if (conversion_type == GL_CONVERSION_TYPE_AUTOMATIC) { // NOLINT
        r = ntohll(r);
    } else if (conversion_type == GL_CONVERSION_TYPE_BIG_ENDIAN) {
        r = be64toh(r);
    } else if (conversion_type == GL_CONVERSION_TYPE_LITTLE_ENDIAN) {
        r = le64toh(r);
    }
    
    return r;
}

int32_t gl_uint8_write(uint8_t **buf, const uint8_t *n) {
    gl_array_push(*buf, *n);
    
    return 1;
}

int32_t gl_uint16_write(uint8_t **buf, const uint16_t *n, gl_conversion_type_t conversion_type) {
    uint16_t v;
    
    if (conversion_type == GL_CONVERSION_TYPE_LITTLE_ENDIAN) {
        v = htole16(*n);
    } else if (conversion_type == GL_CONVERSION_TYPE_BIG_ENDIAN) { // NOLINT
        v = htobe16(*n);
    } else {
        v = htons(*n);
    }
    
    uint8_t *v_array = (uint8_t *)&v;
    gl_array_push(*buf, v_array[0]);
    gl_array_push(*buf, v_array[1]);
    
    return 2;
}

int32_t gl_uint32_write(uint8_t **buf, const uint32_t *n, gl_conversion_type_t conversion_type) {
    uint32_t v;
    
    if (conversion_type == GL_CONVERSION_TYPE_LITTLE_ENDIAN) {
        v = htole32(*n);
    } else if (conversion_type == GL_CONVERSION_TYPE_BIG_ENDIAN) { // NOLINT
        v = htobe32(*n);
    } else {
        v = htonl(*n);
    }
    
    uint8_t *v_array = (uint8_t *)&v;
    gl_array_push(*buf, v_array[0]);
    gl_array_push(*buf, v_array[1]);
    gl_array_push(*buf, v_array[2]);
    gl_array_push(*buf, v_array[3]);
    
    return 4;
}

int32_t gl_uint64_write(uint8_t **buf, const uint64_t *n, gl_conversion_type_t conversion_type) {
    uint64_t v;
    
    if (conversion_type == GL_CONVERSION_TYPE_LITTLE_ENDIAN) {
        v = htole64(*n);
    } else if (conversion_type == GL_CONVERSION_TYPE_BIG_ENDIAN) { // NOLINT
        v = htobe64(*n);
    } else {
        v = htonll(*n);
    }
    
    uint8_t *v_array = (uint8_t *)&v;
    gl_array_push(*buf, v_array[0]);
    gl_array_push(*buf, v_array[1]);
    gl_array_push(*buf, v_array[2]);
    gl_array_push(*buf, v_array[3]);
    gl_array_push(*buf, v_array[4]);
    gl_array_push(*buf, v_array[5]);
    gl_array_push(*buf, v_array[6]);
    gl_array_push(*buf, v_array[7]);
    
    return 8;
}

int32_t gl_uint8_recv(int32_t fd, uint8_t *n) {
    return (int32_t)recv(fd, n, 1, 0);
}

int32_t gl_uint8_array_recv_until_separator(int32_t fd, uint8_t **dst, uint8_t *last_c, uint16_t max_size, bool precise_size, bool allow_spaces, const uint8_t *buf, uint32_t *i) {
    uint8_t c = 0;
    bool first = true;
    
    while (allow_spaces ? !gl_is_terminator(c) : !gl_is_separator_or_terminator(c)) {
        if (!first) {
            gl_array_push(*dst, c);
            gl_assert(gl_array_get_size(*dst) <= max_size);
        }
        
        if (buf) {
            c = buf[(*i)++];
        } else {
            gl_assert(gl_uint8_recv(fd, &c)  > 0);
        }
        first = false;
    }
    
    gl_assert(!precise_size || gl_array_get_size(*dst) == max_size);
    
    *last_c = c;
    
    return gl_array_get_header(*dst)->size;
}

uint32_t gl_rand(uint32_t min, uint32_t max) {
    if (min > max) {
        return 0;
    }
    if (max + 1 - min == 0) {
        return 0;
    }
    
    return min + rand() % (max + 1 - min);
}

bool gl_is_ip_valid(const char *src) {
    int32_t distance_from_last_point = 0;
    int32_t total_num_points = 0;
    for (uint32_t i = 0; i < strlen(src); i++) {
        if (isdigit(src[i]) && distance_from_last_point < 3) {
            distance_from_last_point++;
            continue;
        } else if (src[i] == '.' && distance_from_last_point > 0 && total_num_points < 3) {
            distance_from_last_point = 0;
            total_num_points++;
            continue;
        } else {
            return false;
        }
    }
    
    return true;
}

bool gl_is_number_valid(const char *src, uint32_t size) {
    if (strlen(src) != size) {
        return false;
    }
    
    for (uint32_t i = 0; i < strlen(src); i++) {
        if (isdigit(src[i])) {
            continue;
        } else {
            return false;
        }
    }
    
    return true;
}

// Code from: https://github.com/libsdl-org/SDL/blob/main/src/timer/unix/SDL_systimer.c
void gl_start_ticking() {
    clock_gettime(CLOCK_MONOTONIC, &g_start_ticks);
}

// Code from: https://github.com/libsdl-org/SDL/blob/main/src/timer/unix/SDL_systimer.c
uint32_t gl_get_ticks() {
    uint32_t ticks;
    struct timespec now;
    
    clock_gettime(CLOCK_MONOTONIC, &now);
    ticks = (now.tv_sec - g_start_ticks.tv_sec) * 1000 + (now.tv_nsec - g_start_ticks.tv_nsec) / 1000000;
    return (ticks);
}

// Code from: https://github.com/libsdl-org/SDL/blob/main/src/timer/unix/SDL_systimer.c
void gl_sleep(uint32_t ms) {
    int was_error;
    
    struct timespec elapsed, tv;
    elapsed.tv_sec = ms / 1000;
    elapsed.tv_nsec = (ms % 1000) * 1000000;
    
    do {
        errno = 0;
        
        tv.tv_sec = elapsed.tv_sec;
        tv.tv_nsec = elapsed.tv_nsec;
        
        was_error = nanosleep(&tv, &elapsed);
    } while (was_error && (errno == EINTR));
}
