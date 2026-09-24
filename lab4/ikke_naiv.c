#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define filosofer 5

pthread_t filosoferHandles[filosofer];
sem_t gaffelHandles[filosofer];
int Eaten[filosofer];

pthread_barrier_t barr;

int get_venstre_gaffel(int id);
int give_venstre_gaffel(int id);
int get_hoyre_gaffel(int id);
int give_hoyre_gaffel(int id);
int philosopher(int id);
int eat();

int main()
{
    // Init
    pthread_barrier_init(&barr, NULL, filosofer);

    for(int i = 0; i < filosofer; i++)
    {
        sem_init(&gaffelHandles[i], 0, 1);
        pthread_create(&filosoferHandles[i], NULL, (void*)philosopher, (void*)(long)i);
        Eaten[i] = 0;
        printf("Philosopher %d created\n", i);
    }

    // Wait for threads to finish
    for(int i = 0; i < filosofer; i++)
    {
        pthread_join(filosoferHandles[i], NULL);
        printf("Philosopher %d joined\n", i);
    }

    // End
    pthread_barrier_destroy(&barr);
    for(int i = 0; i < filosofer; i++)
    {
        sem_destroy(&gaffelHandles[i]);
    }

    return 0;
}

int philosopher(int id)
{
    int all_philosophers_have_eaten = 0;
    pthread_barrier_wait(&barr);

    while(!all_philosophers_have_eaten)
    {
        get_venstre_gaffel(id);
        printf("Philosopher %d has taken the left fork\n", id);
        get_hoyre_gaffel(id);
        printf("Philosopher %d has taken the right fork\n", id);

        eat();

        give_venstre_gaffel(id);
        printf("Philosopher %d has given the left fork\n", id);
        give_hoyre_gaffel(id);
        printf("Philosopher %d has given the right fork\n", id);

        Eaten[id] = 1;
        printf("Philosopher %d has eaten\n", id);
        for(int i = 0; i < filosofer; i++)
        {
            if(!Eaten[i])
            {
                all_philosophers_have_eaten = 0;
                break;
            }
            else
                all_philosophers_have_eaten = 1;
        }
    }

    return 0;
}

int get_venstre_gaffel(int id)
{
    sem_t* gaffel = NULL;
    int gaffel_id = id-1;

    if(gaffel_id < 0)
        gaffel_id = filosofer-1;

    gaffel = &gaffelHandles[gaffel_id];

    get_hoyre_gaffel(id);
    give_hoyre_gaffel(id);
    sem_wait(gaffel);

    return 0;
}

int give_venstre_gaffel(int id)
{
    sem_t* gaffel = NULL;
    int gaffel_id = id-1;

    if(gaffel_id < 0)
        gaffel_id = filosofer-1;

    gaffel = &gaffelHandles[gaffel_id];

    sem_post(gaffel);

    return 0;
}

int get_hoyre_gaffel(int id)
{
    sem_t* gaffel = NULL;
    int gaffel_id = id+1;

    if(gaffel_id > filosofer-1)
        gaffel_id = 0;

    gaffel = &gaffelHandles[gaffel_id];

    sem_wait(gaffel);
        
    return 0;
}

int give_hoyre_gaffel(int id)
{
    sem_t* gaffel = NULL;
    int gaffel_id = id+1;

    if(gaffel_id > filosofer-1)
        gaffel_id = 0;

    gaffel = &gaffelHandles[gaffel_id];

    sem_post(gaffel);
        
    return 0;
}

int eat()
{
    int time = 1000;
    usleep(time);
    return 0;
}