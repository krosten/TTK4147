#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define N (10*1000*1000)


static inline uint64_t rdtsc(void){
    uint64_t val;
    __asm__ volatile("isb; mrs %0, cntvct_el0; isb" : "=r"(val) :: "memory");
    return val;
}

static double ns_per_tick(void){
    uint64_t f;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(f));
    return 1e9 / (double)f;
}

static uint64_t now(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec*1000000000ull + ts.tv_nsec;
}

int main(void){
    volatile uint64_t sink = 0;
    long hist[50] = {0};
    double npt = ns_per_tick();

    fprintf(stderr, "ns per tick: %.3f\n", npt);

    uint64_t a = now();
    for (int i = 0; i < N; i++) sink += rdtsc();
    uint64_t b = now();
    fprintf(stderr, "latency: %.2f ns\n", (double)(b - a)/N);

    for (int i = 0; i < N; i++){
        uint64_t t1 = rdtsc();
        uint64_t t2 = rdtsc();
        uint64_t ns = (uint64_t)((t2 - t1) * npt);
        if (ns < 50) hist[ns]++;
    }
    for (int i = 0; i < 50; i++) printf("%ld\n", hist[i]);
}