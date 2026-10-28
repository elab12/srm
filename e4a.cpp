// Experiment 4 - Part A: Mutual exclusion using semaphores: restroom problem
// - men and women may never be inside together
// - at most 4 people inside at a time
// SRM Institute of Science and Technology - OS Lab (21CSC202J)
//
// Compile: g++ e4a.cpp -o e4a -pthread
// Run:     ./e4a

#include <iostream>
#include <thread>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

sem_t max_capacity;   // at most 4 people inside
sem_t empty_room;     // held by whichever gender is currently inside
sem_t turnstile;      // keeps one gender from starving the other
sem_t mutex_print;    // protects the shared counter and the output
sem_t mutex_men, mutex_women;   // protect the two gender counters
int restroom = 0, menInside = 0, womenInside = 0;

void enterGroup(int &count, sem_t &m)
{
    sem_wait(&turnstile);
    sem_wait(&m);
    if (++count == 1) sem_wait(&empty_room);   // first one in locks the room
    sem_post(&m);
    sem_post(&turnstile);
}

void leaveGroup(int &count, sem_t &m)
{
    sem_wait(&m);
    if (--count == 0) sem_post(&empty_room);   // last one out unlocks it
    sem_post(&m);
}

void person(const char *who, int id)
{
    for (int visit = 0; visit < 2; visit++)
    {
        bool man = (who[0] == 'M');
        if (man) enterGroup(menInside, mutex_men);
        else     enterGroup(womenInside, mutex_women);

        sem_wait(&max_capacity);
        sem_wait(&mutex_print);
        restroom++;
        cout << who << " " << id << " has entered the Restroom, "
             << "people inside: " << restroom << endl;
        sem_post(&mutex_print);

        usleep(100000);                      // use the restroom

        sem_wait(&mutex_print);
        restroom--;
        cout << who << " " << id << " has exited the Restroom,  "
             << "people inside: " << restroom << endl;
        sem_post(&mutex_print);
        sem_post(&max_capacity);

        if (man) leaveGroup(menInside, mutex_men);
        else     leaveGroup(womenInside, mutex_women);

        usleep(50000);
    }
}

int main()
{
    sem_init(&max_capacity, 0, 4);
    sem_init(&empty_room, 0, 1);
    sem_init(&turnstile, 0, 1);
    sem_init(&mutex_print, 0, 1);
    sem_init(&mutex_men, 0, 1);
    sem_init(&mutex_women, 0, 1);

    vector<thread> t;
    for (int i = 1; i <= 3; i++) t.emplace_back(person, "Woman", i);
    for (int i = 1; i <= 3; i++) t.emplace_back(person, "Man", i);
    for (auto &x : t) x.join();

    sem_destroy(&max_capacity);
    sem_destroy(&empty_room);
    sem_destroy(&turnstile);
    sem_destroy(&mutex_print);
    sem_destroy(&mutex_men);
    sem_destroy(&mutex_women);

    return 0;
}
