#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define N (10*1000*1000)

static uint64_t now(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec*1000000000ull + ts.tv_nsec;
}

int main(void){
    volatile uint64_t sink = 0;
    long hist[50] = {0};

    uint64_t a = now();
    for (int i = 0; i < N; i++) sink += now();
    uint64_t b = now();
    fprintf(stderr, "latency: %.2f ns\n", (double)(b - a)/N);

    for (int i = 0; i < N; i++){
        uint64_t t1 = now();
        uint64_t t2 = now();
        uint64_t ns = t2 - t1;
        if (ns < 50) hist[ns]++;
    }
    for (int i = 0; i < 50; i++) printf("%ld\n", hist[i]);
}