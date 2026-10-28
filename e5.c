/*
 * Experiment 5: Reader-Writer problem (readers have priority) using semaphores
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e5.c -o e5 -pthread
 * Run:     ./e5
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t m;              /* protects read_count */
sem_t w;              /* writers' exclusive access to the resource */
int read_count = 0;
int data_val = 0;     /* the shared resource */

void *writer(void *arg)
{
    int id = *(int *)arg;
    sem_wait(&w);
    data_val++;
    printf("Writer %d wrote data = %d\n", id, data_val);
    sleep(1);
    sem_post(&w);
    return NULL;
}

void *reader(void *arg)
{
    int id = *(int *)arg;
    sem_wait(&m);
    read_count++;
    if (read_count == 1)
        sem_wait(&w);          /* first reader locks out writers */
    sem_post(&m);

    printf("Reader %d read data = %d (readers inside: %d)\n",
           id, data_val, read_count);
    sleep(1);

    sem_wait(&m);
    read_count--;
    if (read_count == 0)
        sem_post(&w);          /* last reader lets writers in */
    sem_post(&m);
    return NULL;
}

int main()
{
    pthread_t r[3], wr[2];
    int rid[3] = {1, 2, 3}, wid[2] = {1, 2};

    sem_init(&m, 0, 1);
    sem_init(&w, 0, 1);

    pthread_create(&r[0], NULL, reader, &rid[0]);
    pthread_create(&wr[0], NULL, writer, &wid[0]);
    pthread_create(&r[1], NULL, reader, &rid[1]);
    pthread_create(&r[2], NULL, reader, &rid[2]);
    pthread_create(&wr[1], NULL, writer, &wid[1]);

    for (int i = 0; i < 3; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(wr[i], NULL);

    sem_destroy(&m);
    sem_destroy(&w);
    return 0;
}
