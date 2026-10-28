// Experiment 6: Dining Philosopher problem using a monitor (pthread mutex + condition variables)
// SRM Institute of Science and Technology - OS Lab (21CSC202J)
//
// Compile: g++ e6.cpp -o e6 -pthread
// Run:     ./e6

#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + N - 1) % N
#define RIGHT (phnum + 1) % N

// Philosopher index
int phil[N];
int times_to_eat = 2; // number of times each philosopher eats

class monitor {
    int state[N];
    pthread_cond_t phcond[N];
    pthread_mutex_t condLock;
public:
    void test(int phnum)
    {
        if (state[RIGHT] != EATING && state[LEFT] != EATING
            && state[phnum] == HUNGRY) {
            state[phnum] = EATING;
            pthread_cond_signal(&phcond[phnum]);
        }
    }

    void take_fork(int phnum)
    {
        pthread_mutex_lock(&condLock);
        state[phnum] = HUNGRY;
        test(phnum);
        while (state[phnum] != EATING)
            pthread_cond_wait(&phcond[phnum], &condLock);
        cout << "Philosopher " << phnum + 1 << " is Eating" << endl;
        pthread_mutex_unlock(&condLock);
    }

    void put_fork(int phnum)
    {
        pthread_mutex_lock(&condLock);
        state[phnum] = THINKING;
        cout << "Philosopher " << phnum + 1 << " finished eating, now thinking" << endl;
        test(RIGHT);
        test(LEFT);
        pthread_mutex_unlock(&condLock);
    }

    monitor()
    {
        for (int i = 0; i < N; i++)
            state[i] = THINKING;
        for (int i = 0; i < N; i++)
            pthread_cond_init(&phcond[i], NULL);
        pthread_mutex_init(&condLock, NULL);
    }

    ~monitor()
    {
        for (int i = 0; i < N; i++)
            pthread_cond_destroy(&phcond[i]);
        pthread_mutex_destroy(&condLock);
    }
} phil_object;

void *philosopher(void *arg)
{
    int i = *(int *)arg;
    for (int c = 0; c < times_to_eat; c++) {
        usleep(300000); // thinking
        phil_object.take_fork(i);
        usleep(300000); // eating
        phil_object.put_fork(i);
    }
    return NULL;
}

int main()
{
    pthread_t thread_id[N];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < N; i++)
        phil[i] = i;

    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], &attr, philosopher, &phil[i]);
        cout << "Philosopher " << i + 1 << " is thinking..." << endl;
    }

    for (int i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    pthread_attr_destroy(&attr);
    return 0;
}
