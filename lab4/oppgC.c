#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define THREAD_COUNT 2
#define ITERATIONS 1000000
unsigned int semCount = 1;

long globalCounter = 0;
pthread_barrier_t barr;

sem_t mutex;


void* fn(void* args)
{
    long id = (long)args;
    long localCounter = 0;

    pthread_barrier_wait(&barr);

    for (long i = 0; i < ITERATIONS; i++)
    {
        sem_wait(&mutex);
        globalCounter++;
        sem_post(&mutex);
        localCounter++;

    }

    printf("Thread %ld: local = %ld, global = %ld\n", id, localCounter, globalCounter);

    return NULL;
}

int main()
{
    sem_init(&mutex, 0, 1);

    pthread_t threadHandles[THREAD_COUNT];

    pthread_barrier_init(&barr, NULL, THREAD_COUNT);

    for (long i = 0; i < THREAD_COUNT; i++)
        pthread_create(&threadHandles[i], NULL, fn, (void*)i);

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(threadHandles[i], NULL);


    return 0;
}
