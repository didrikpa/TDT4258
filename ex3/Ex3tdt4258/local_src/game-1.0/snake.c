#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "snake.h"
#include "framebuffer.c"

uint16_t blue = COLOR(0b00000000, 0b000000000, 0b11110000);
uint16_t red = COLOR(0b11110000, 0b00000000, 0b00000000);

void gameloop(){
	int inPlay = 1;
	drawRect(160, 120, 5, 5, blue);

}

long getmsDiff(struct timespec start, struct timespec stop)
{
    long diff;
    diff = ((long) stop.tv_sec*1000 + stop.tv_nsec/1E6) 
     - ((long) start.tv_sec*1000 + start.tv_nsec/1E6);
    
    return diff;
}

// return elapsed time in millisecond since prev
long msSince(struct timespec prev)
{
    struct timespec now;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
    return getmsDiff(prev, now);
}