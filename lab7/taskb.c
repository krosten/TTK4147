#include <pthread.h>
#include <wiringPi.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

#include <sched.h>

int set_cpu(int cpu_id)
{
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return EINVAL;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


typedef struct {
    int pinIn;
    int pinOut;
    int cpu_id;
} responseTaskArgs;

void io_init( void ) {
    wiringPiSetup();

    pinMode (PININ1, INPUT); pinMode (PINOUT1, OUTPUT);
    pinMode (PININ2, INPUT); pinMode (PINOUT2, OUTPUT);
    pinMode (PININ3, INPUT); pinMode (PINOUT3, OUTPUT);

    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

void* responseTask(void* arg)
{
    responseTaskArgs* args = (responseTaskArgs*)arg;

    set_cpu(args->cpu_id)

    while (1) {
        while (digitalRead(args->pinIn) == HIGH);
            digitalWrite(args->pinOut, LOW);

        while (digitalRead(args->pinIn) == LOW);
            digitalWrite(args->pinOut, HIGH);
    }

    return NULL;
}

int main(void)
{
    io_init();

    responseTaskArgs args[3] = {
        { PININ1, PINOUT1, 0},
        { PININ2, PINOUT2, 0},
        { PININ3, PINOUT3, 0},
    };
    pthread_t threads[3];

    for (int i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, responseTask, &args[i]);

    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
