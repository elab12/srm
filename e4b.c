/*
 * Experiment 4 - Part B: Mutual Exclusion using Monitor pattern in Pure C
 * Implemented using pthread_mutex_t and pthread_cond_t
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e4b.c -o e4b_c -pthread
 * Run:     ./e4b_c
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int sharedResource;
    pthread_mutex_t mtx;
    pthread_cond_t condVar;
} NinjaMonitor;

NinjaMonitor monitor;

void monitor_init(NinjaMonitor *m) {
    m->sharedResource = 0;
    pthread_mutex_init(&m->mtx, NULL);
    pthread_cond_init(&m->condVar, NULL);
}

int monitor_access(NinjaMonitor *m) {
    pthread_mutex_lock(&m->mtx);
    while (m->sharedResource == 0) {
        pthread_cond_wait(&m->condVar, &m->mtx);
    }
    int val = m->sharedResource;
    m->sharedResource = 0;
    pthread_mutex_unlock(&m->mtx);
    return val;
}

void monitor_update(NinjaMonitor *m, int new_val) {
    pthread_mutex_lock(&m->mtx);
    m->sharedResource = new_val;
    pthread_cond_signal(&m->condVar);
    pthread_mutex_unlock(&m->mtx);
}

void *ninja_thread(void *arg) {
    int value = monitor_access(&monitor);
    printf("Ninja thread read shared resource: %d\n", value);
    pthread_exit(NULL);
}

int main() {
    pthread_t t;
    monitor_init(&monitor);

    pthread_create(&t, NULL, ninja_thread, NULL);

    sleep(1);
    monitor_update(&monitor, 67);

    pthread_join(t, NULL);

    pthread_mutex_destroy(&monitor.mtx);
    pthread_cond_destroy(&monitor.condVar);
    return 0;
}
