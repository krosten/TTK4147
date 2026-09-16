#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "scheduling.h"
#include "schedulers.h"

void set_task_state(struct Task *task, enum taskState taskNewState)
{
    pthread_mutex_lock(&taskStateMutex);
    task->state = taskNewState;
    pthread_mutex_unlock(&taskStateMutex);
}

void wait_for_rescheduling(int quantum, struct Task *task)
{
    int startTime;
    int waitTime;

    pthread_mutex_lock(&timeMutex);
    startTime = globalTime;
    pthread_mutex_unlock(&timeMutex);

    do
    {
        pthread_mutex_lock(&timeMutex);
        pthread_cond_wait(&timeCond, &timeMutex);
        waitTime = globalTime - startTime;
        pthread_mutex_unlock(&timeMutex);
    } while (task->state != finished && waitTime < quantum);

    usleep(timeUnitUs / 100);
}

void round_robin(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, tasks[taskIndex]);

        //  Check if the task is finished
        if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        }

        // Find the next task to run
        taskIndex = (taskIndex + 1) % taskCount;

    } while (globalTime < timeout);
}

// Implement your schedulers here!
void first_come_first_served(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here

        // Her kommer første spor av kristian:
    struct Task* taskToRun = tasks[0]; // Init task pointer

    do{
        // Find the earliest unfinished Task
        for(int taskIndex = 1; taskIndex < taskCount; taskIndex++){
            if(tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime){
                continue;
            }

            if(tasks[taskIndex]->arrivalTime < taskToRun->arrivalTime){
                struct Task* taskToRun = tasks[taskIndex];
            }
        }

        if(taskToRun == finished || taskToRun->arrivalTime > globalTime){ // New task was not available yet
            continue;
        }

        if (taskToRun->startTime == -1)
            taskToRun->startTime = globalTime; // Vet ikke hvorfor de gjør det i RR når det står at man ikke skal gjøre det i readme, men de gjør det i eksempelet så ¯\(ツ)/¯
        set_task_state(taskToRun, running);

        while(taskToRun->state != finished){} // Fant ingen form for signaling mekanisme uten quantum for at en task er ferdig, så her er en busy wait istedenfor, spør Joachim om noe finnes hvis det ikke funker

    } while (globalTime < timeout);
}

void shortest_process_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
    struct Task* taskToRun = tasks[0]; // Init task pointer

    do{
        // Find the shortest unfinished Task
        for(int taskIndex = 1; taskIndex < taskCount; taskIndex++){
            if(tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime){
                continue;
            }

            if(tasks[taskIndex]->totalRuntime < taskToRun->totalRuntime){
                struct Task* taskToRun = tasks[taskIndex];
            }
        }

        if(taskToRun == finished || taskToRun->arrivalTime > globalTime){ // New task was not available yet
            continue;
        }

        if (taskToRun->startTime == -1)
            taskToRun->startTime = globalTime;
        set_task_state(taskToRun, running);

        while(taskToRun->state != finished){} // Fant ingen form for signaling mekanisme uten quantum for at en task er ferdig, så her er en busy wait istedenfor, spør Joachim om noe finnes hvis det ikke funker

    } while (globalTime < timeout);
}

void highest_response_ratio_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
        // Highest response ratio = (W + S)/S
        // W = hvor lenge en prosess har ventet
        // S = Hvor lang tid det tar for prosessen å fullføre

    struct Task* taskToRun = tasks[0]; // Init task pointer

    do{
        // Find the Task with highest ratio
        for(int taskIndex = 1; taskIndex < taskCount; taskIndex++){
            if(tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime){
                continue;
            }

            // Calculate ratios
            int taskToRunW = globalTime - taskToRun->arrivalTime;
            int taskToRunS = taskToRun->totalRuntime;
            float taskToRunRatio = ((float)taskToRunW + (float)taskToRunS) / (float)taskToRunS;

            int taskW = globalTime - tasks[taskIndex]->arrivalTime;
            int taskS = tasks[taskIndex]->totalRuntime;
            float taskRatio = ((float)taskW + (float)taskS) / (float)taskS;

            if(taskToRunRatio < taskRatio){
                struct Task* taskToRun = tasks[taskIndex];
            }
        }

        if(taskToRun == finished || taskToRun->arrivalTime > globalTime){ // New task was not available yet
            continue;
        }

        if (taskToRun->startTime == -1)
            taskToRun->startTime = globalTime;
        set_task_state(taskToRun, running);

        while(taskToRun->state != finished){} // Fant ingen form for signaling mekanisme uten quantum for at en task er ferdig, så her er en busy wait istedenfor, spør Joachim om noe finnes hvis det ikke funker

    } while (globalTime < timeout);
}
void shortest_remaining_time(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here

    struct Task* taskToRun = tasks[0]; // Init task pointer

    do{
        // Find the Task with shortest time remaining
        for(int taskIndex = 1; taskIndex < taskCount; taskIndex++){
            if(tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime){
                continue;
            }

            // Calculate remaining time
            int taskToRunRemaining = taskToRun->totalRuntime - taskToRun->currentRuntime;
            int taskRemaining = tasks[taskIndex]->totalRuntime - tasks[taskIndex]->currentRuntime;

            if(taskToRunRemaining > taskRemaining){
                struct Task* taskToRun = tasks[taskIndex];
            }
        }

        if(taskToRun == finished || taskToRun->arrivalTime > globalTime){ // New task was not available yet
            continue;
        }

        if (taskToRun->startTime == -1)
            taskToRun->startTime = globalTime;
        set_task_state(taskToRun, running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, taskToRun);

        //  Check if the task is finished
        if (taskToRun->state == finished)
        {
        }
        else
        {
            set_task_state(taskToRun, preempted);
        }

    } while (globalTime < timeout);
}
void feedback(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here

    // Har ikke laget forventninger av hvordan noen av schedulerene vil kjøres, så det må du gjøre for å sammenligne med output.
    // Har ikke testet noe som helst btw, bare simulert det i hodet, så lykke til med debuggingen!
    // Har også spart denne til deg, lykke til ;)
}
