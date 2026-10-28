/*
 * Experiment 4 - Part A: Mutual Exclusion using Semaphores in Pure C
 * Restroom synchronization problem (at most 4 capacity, men/women mutual exclusion)
 * SRM Institute of Science and Technology - OS Lab (21CSC202J)
 *
 * Compile: gcc e4a.c -o e4a_c -pthread
 * Run:     ./e4a_c
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t max_capacity;
sem_t empty_room;
sem_t turnstile;
sem_t mutex_print;
sem_t mutex_men, mutex_women;

int restroom = 0, menInside = 0, womenInside = 0;

typedef struct {
    char gender[10];
    int id;
} PersonArgs;

void enterGroup(int *count, sem_t *m)
{
    sem_wait(&turnstile);
    sem_wait(m);
    (*count)++;
    if (*count == 1) sem_wait(&empty_room);
    sem_post(m);
    sem_post(&turnstile);
}

void leaveGroup(int *count, sem_t *m)
{
    sem_wait(m);
    (*count)--;
    if (*count == 0) sem_post(&empty_room);
    sem_post(m);
}

void *person_thread(void *arg)
{
    PersonArgs *p = (PersonArgs *)arg;
    int is_man = (p->gender[0] == 'M');

    for (int visit = 0; visit < 2; visit++)
    {
        if (is_man) enterGroup(&menInside, &mutex_men);
        else        enterGroup(&womenInside, &mutex_women);

        sem_wait(&max_capacity);
        sem_wait(&mutex_print);
        restroom++;
        printf("%s %d has entered the Restroom, people inside: %d\n", p->gender, p->id, restroom);
        sem_post(&mutex_print);

        usleep(100000);

        sem_wait(&mutex_print);
        restroom--;
        printf("%s %d has exited the Restroom,  people inside: %d\n", p->gender, p->id, restroom);
        sem_post(&mutex_print);
        sem_post(&max_capacity);

        if (is_man) leaveGroup(&menInside, &mutex_men);
        else        leaveGroup(&womenInside, &mutex_women);

        usleep(50000);
    }
    pthread_exit(NULL);
}

int main()
{
    sem_init(&max_capacity, 0, 4);
    sem_init(&empty_room, 0, 1);
    sem_init(&turnstile, 0, 1);
    sem_init(&mutex_print, 0, 1);
    sem_init(&mutex_men, 0, 1);
    sem_init(&mutex_women, 0, 1);

    pthread_t threads[6];
    PersonArgs args[6] = {
        {"Woman", 1}, {"Woman", 2}, {"Woman", 3},
        {"Man", 1},   {"Man", 2},   {"Man", 3}
    };

    for (int i = 0; i < 6; i++)
        pthread_create(&threads[i], NULL, person_thread, &args[i]);

    for (int i = 0; i < 6; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&max_capacity);
    sem_destroy(&empty_room);
    sem_destroy(&turnstile);
    sem_destroy(&mutex_print);
    sem_destroy(&mutex_men);
    sem_destroy(&mutex_women);

    return 0;
}
