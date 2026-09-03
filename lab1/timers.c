#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>

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
    struct tms tb;
    volatile uint64_t sink = 0;
    long hist[50] = {0};
    double npt = 1e9 / (double)sysconf(_SC_CLK_TCK);  /* ns per tikk */
    long over = 0;

    fprintf(stderr, "CLK_TCK: %ld, ns per tick: %.0f\n",
            sysconf(_SC_CLK_TCK), npt);

    uint64_t a = now();
    for (int i = 0; i < N; i++) sink += (uint64_t)times(&tb);
    uint64_t b = now();
    fprintf(stderr, "latency: %.2f ns\n", (double)(b - a)/N);

    for (int i = 0; i < N; i++){
        uint64_t t1 = (uint64_t)times(&tb);
        uint64_t t2 = (uint64_t)times(&tb);
        /* bins på 1 ms, ellers havner alt i bin 0 */
        uint64_t bin = (uint64_t)((t2 - t1) * npt / 1e6);
        if (bin < 50) hist[bin]++;
        else          over++;
    }
    fprintf(stderr, "utenfor: %ld\n", over);

    for (int i = 0; i < 50; i++) printf("%d %ld\n", i, hist[i]);
}