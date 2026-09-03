#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/times.h>


void busy_wait(int sec){

    static clock_t st_time;
    static clock_t en_time;
    static struct tms st_cpu;
    static struct tms en_cpu;

    st_time = times(&st_cpu);

    while((en_time - st_time) < sec*100){
        en_time = times(&en_cpu);
    }
}

int main(){

    busy_wait(1);
    return 0;
}