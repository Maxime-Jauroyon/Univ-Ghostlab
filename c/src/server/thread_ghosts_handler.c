#include <server/thread_ghosts_handler.h>
#include <stdlib.h>
#include <pthread.h>
#include <common/log.h>
#include <common/game.h>
#include <common/array.h>
#include <common/utils.h>
#include <server/shared.h>

static uint32_t *g_ghosts_pos = 0;

void sleep_worker(pthread_mutex_t *lock, pthread_cond_t *cond) {
    uint32_t time_to_wait = gl_rand(30, 90);
    
    struct timespec now_time;
    clock_gettime(CLOCK_REALTIME, &now_time);
    struct timespec duration = {now_time.tv_sec + time_to_wait, 0}; // NOLINT
    
    pthread_mutex_lock(lock);
    pthread_cond_timedwait(cond, lock, &duration);
    pthread_mutex_unlock(lock);
}

void cleanup_worker(void *cleanup_handle_ptr) {
    pthread_mutex_t *mutex = cleanup_handle_ptr;
    pthread_mutex_unlock(mutex);
    
    gl_array_free(g_ghosts_pos);
    
    gl_log_push("ghosts handler thread stopped.\n");
}

void *gl_thread_ghosts_handler_main(void *user_data) {
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    
    pthread_cleanup_push(cleanup_worker, &lock); // NOLINT
    
    gl_log_push("ghosts handler thread started.\n");
    
    while (!g_should_quit) {
        sleep_worker(&lock, &cond);
        
        pthread_mutex_lock(g_main_mutex);
        
        for (uint32_t i = 0; i < gl_array_get_size(g_games); i++) {
            if (!g_games[i].started) {
                continue;
            }
            
            uint32_t num_to_move = (rand() % 2 == 0 ? 1 : 0) + rand() % (gl_array_get_size(g_games[i].ghosts) + 1);
            
            uint32_t k = 0;
            while (gl_array_get_size(g_ghosts_pos) != num_to_move && k < 10) {
                uint32_t pos = rand() % gl_array_get_size(g_games[i].ghosts);
                bool exists = false;
                for (uint32_t j = 0; j < gl_array_get_size(g_ghosts_pos); j++) {
                    if (g_ghosts_pos[j] == pos) {
                        exists = true;
                        break;
                    }
                }
                
                if (!exists) {
                    gl_array_push(g_ghosts_pos, pos);
                }
                
                k++;
            }
    
            for (uint32_t j = 0; j < gl_array_get_size(g_ghosts_pos); j++) {
                gl_game_move_ghost(&g_games[i], j);
            }
    
            gl_array_free(g_ghosts_pos);
        }
        
        pthread_mutex_unlock(g_main_mutex);
    }
    
    pthread_cleanup_pop(1); // NOLINT
    
    return 0;
}
