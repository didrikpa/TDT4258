#ifndef GAME_H
#define GAME_H

void gameloop();
long getmsDiff(struct timespec start, struct timespec stop);
long msSince(struct timespec prev);
int count();
int delete(int num);
void append(int num);
void gpio_handler(int signum);
int snake_get_gpio(void (*func)(int));

#endif
