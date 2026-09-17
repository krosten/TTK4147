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

// Block until the timer thread ticks, so the scheduler does not spin while it
// waits for the next task to arrive.
static void wait_one_time_unit(void)
{
    pthread_mutex_lock(&timeMutex);
    pthread_cond_wait(&timeCond, &timeMutex);
    pthread_mutex_unlock(&timeMutex);
}

// Pick the best runnable task, ignoring tasks that are finished or have not
// arrived yet. Returns NULL when nothing is runnable at the moment.
static struct Task *select_task(struct Task **tasks, int taskCount, bool better(struct Task *, struct Task *))
{
    struct Task *best = NULL;

    for (int taskIndex = 0; taskIndex < taskCount; taskIndex++)
    {
        struct Task *task = tasks[taskIndex];

        if (task->state == finished || task->arrivalTime > globalTime)
            continue;

        if (best == NULL || better(task, best))
            best = task;
    }

    return best;
}

// Run the selected task to completion, the way a non-preemptive scheduler does.
// wait_for_rescheduling() returns as soon as the task reaches the finished
// state, so the timeout only acts as an upper bound.
static void run_until_finished(struct Task *task, int timeout)
{
    if (task->startTime == -1)
        task->startTime = globalTime;
    set_task_state(task, running);

    wait_for_rescheduling(timeout, task);
}

static int remaining_runtime(struct Task *task)
{
    return task->totalRuntime - task->currentRuntime;
}

// Response ratio = (W + S)/S, where W is the time spent waiting since arrival
// and S is the total runtime of the task.
static float response_ratio(struct Task *task)
{
    float w = (float)(globalTime - task->arrivalTime);
    float s = (float)task->totalRuntime;

    return (w + s) / s;
}

static bool earlier_arrival(struct Task *a, struct Task *b)
{
    return a->arrivalTime < b->arrivalTime;
}

static bool shorter_total_runtime(struct Task *a, struct Task *b)
{
    return a->totalRuntime < b->totalRuntime;
}

static bool higher_response_ratio(struct Task *a, struct Task *b)
{
    return response_ratio(a) > response_ratio(b);
}

static bool less_remaining_runtime(struct Task *a, struct Task *b)
{
    return remaining_runtime(a) < remaining_runtime(b);
}

void first_come_first_served(struct Task **tasks, int taskCount, int timeout)
{
    do
    {
        struct Task *taskToRun = select_task(tasks, taskCount, earlier_arrival);

        if (taskToRun == NULL) // Nothing has arrived yet
        {
            wait_one_time_unit();
            continue;
        }

        run_until_finished(taskToRun, timeout);

    } while (globalTime < timeout);
}

void shortest_process_next(struct Task **tasks, int taskCount, int timeout)
{
    do
    {
        struct Task *taskToRun = select_task(tasks, taskCount, shorter_total_runtime);

        if (taskToRun == NULL)
        {
            wait_one_time_unit();
            continue;
        }

        run_until_finished(taskToRun, timeout);

    } while (globalTime < timeout);
}

void highest_response_ratio_next(struct Task **tasks, int taskCount, int timeout)
{
    do
    {
        struct Task *taskToRun = select_task(tasks, taskCount, higher_response_ratio);

        if (taskToRun == NULL)
        {
            wait_one_time_unit();
            continue;
        }

        run_until_finished(taskToRun, timeout);

    } while (globalTime < timeout);
}

void shortest_remaining_time(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    do
    {
        // Reselect every quantum, so a newly arrived short task can preempt
        struct Task *taskToRun = select_task(tasks, taskCount, less_remaining_runtime);

        if (taskToRun == NULL)
        {
            wait_one_time_unit();
            continue;
        }

        if (taskToRun->startTime == -1)
            taskToRun->startTime = globalTime;
        set_task_state(taskToRun, running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, taskToRun);

        //  Check if the task is finished
        if (taskToRun->state != finished)
            set_task_state(taskToRun, preempted);

    } while (globalTime < timeout);
}

void feedback(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here
    const int maxLevel = taskCount - 1; // Bottom queue runs round-robin
    int level[taskCount];
    int seq[taskCount];
    int counter = 0;

    for (int i = 0; i < taskCount; i++) {
        level[i] = 0;
        seq[i] = -1; // Not queued yet
    }

    do {
        int best = -1;
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i]->state == finished || tasks[i]->arrivalTime >globalTime)
                continue;
            if (seq[i] == -1)
                seq[i] = counter++; // Just arrived, enters the top queue
            if (best == -1 || level[i] < level[best] ||
                (level[i] == level[best] && seq[i] < seq[best]))
                best = i;
        }
        if (best == -1) // Nothing runnable yet
        {
        wait_one_time_unit();
            continue;
        }

        struct Task *taskToRun = tasks[best];

        if (taskToRun->startTime == -1)
            taskToRun->startTime = globalTime;
        set_task_state(taskToRun, running);

        // Run for one quantum
        wait_for_rescheduling(quantum, taskToRun);

        if (taskToRun->state != finished)
        {
            set_task_state(taskToRun, preempted);

            // Demote one level, then to the back of that queue
            if (level[best] < maxLevel)
                level[best]++;
            seq[best] = counter++;
        }

    } while (globalTime < timeout);
}
