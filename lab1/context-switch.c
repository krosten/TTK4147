#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sched.h>

#define N       (1000*1000)
#define NS_MAX  10000

static uint64_t now(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec*1000000000ull + ts.tv_nsec;
}

int main(void){
    static long hist[NS_MAX];
    long over = 0;
    uint64_t min = UINT64_MAX;

    for (int i = 0; i < N; i++){
        uint64_t t1 = now();
        sched_yield();
        uint64_t t2 = now();

        uint64_t ns = t2 - t1;
        if (ns < min) min = ns;
        if (ns < NS_MAX) hist[ns]++;
        else             over++;
    }

    fprintf(stderr, "minste: %lu ns, utenfor histogram: %ld av %d\n",
            min, over, N);

    for (int i = 0; i < NS_MAX; i++) printf("%d %ld\n", i, hist[i]);
}