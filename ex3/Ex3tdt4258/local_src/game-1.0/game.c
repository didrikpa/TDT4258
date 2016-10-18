#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <signal.h>


#include "game.h"
#include "framebuffer.c"

#define WAIT_TIME 1000000000



struct node{
    int x;
    int y;
    struct node *next;
};

struct node *temp;
struct node *root;
struct node *body;
struct node *tail;
struct node *head;
struct node *apple;
int descr;

static uint16_t red = COLOR(0b11111, 0b000000, 0b00000);
static uint16_t green = COLOR(0b00000, 0b111111, 0b00000);
static uint16_t black = COLOR(0b00000, 0b000000, 0b00000);
static uint16_t white = COLOR(0b11111, 0b111111, 0b11111);
int input;
int description;
int direction;








int main(int argc, char *argv[]){
    printf("Hello World, I'm game!\n");
    
    setupFB();
    
    gameloop();

        
    exit(EXIT_SUCCESS);
}


void gameloop() { 
    srand( time(NULL));
    
    root = (struct node *) malloc(sizeof(struct node));
    body = (struct node *) malloc(sizeof(struct node));
    tail = (struct node *) malloc(sizeof(struct node));
    head = (struct node *) malloc(sizeof(struct node));
    temp = (struct node *) malloc(sizeof(struct node));
    apple = (struct node *) malloc(sizeof(struct node));


    int h = 5;
    int x = 100;
    int y = 120;
    root->x = 160;
    root->y = 120;
    body->x = 154;
    body->y = 120;
    tail->x = 0;
    tail->y = 0;
    head->x = 0;
    head->y = 0;
    // apple->x = 130;
    // apple ->y = 120;


    tail->next = root;
    head->next = body;
    root->next = body;

    bool go = true;
    int moving = 1;
    drawRect(0,0,320, 240, &white);
    drawRect(4,6, 306, 228, &black);
    drawRect(root->x, root->y, h, h, &green);
    drawRect(body->x, body->y, h, h, &green);
    drawApple(tail);


    struct timespec lastTime;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastTime);



    //while (nanosleep(&sleeptime, &sleeptime) && errno == EINTR);
    getGpio();
        while(go){
            while (msSince(lastTime)<50);
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastTime);
            head->next->next =  (struct node *) malloc(sizeof(struct node));
            if (direction == 5){
                break;
            }
            if(direction != moving+2 && direction != moving-2 && direction != moving){
                if (direction != 0){
                    moving = direction;
                }
            }

            if (moving == 1){
                head->next->next->x = head->next->x-6;
                head->next->next->y = head->next->y;
            }
            else if(moving == 2){
                head->next->next->x = head->next->x;
                head->next->next->y = head->next->y-6;
            }
            else if(moving == 3){
                head->next->next->x = head->next->x+6;
                head->next->next->y = head->next->y;
            }
            else if(moving == 4){
                head->next->next->x = head->next->x;
                head->next->next->y = head->next->y+6;
            }
            if (checkScreen(head->next->next->x, head->next->next->y) == green)
            {
                printf("You fuckin wanku55\n");
                break;
            }
            head->next = head->next->next;

            drawRect(head->next->x, head->next->y, h, h, &green);
            if(head->next->x >=308 || head->next->y >=235 || head->next->x <= 2 || head->next->y <= 5){
                printf("you fucking wankuh\n");
                break;
            }
            if(head->next->x != apple->x || head->next->y != apple->y){
                drawRect(tail->next->x, tail->next->y, h, h, &black);
                temp = tail->next->next;
                free(tail->next);
                tail->next = temp;
            }
            else{
               drawApple(tail);
            }
        }

}

void drawApple(struct node *tail){
    int tempY = rand() % 38 +1;
    int tempX = rand() % 51;
    tempY = (tempY*6);
    tempX = (tempX*6)+4;
    apple->y  = tempY;
    apple->x = tempX;
    struct node *tempnode;
    tempnode = tail;
    if(checkScreen(tempX, tempY) == green){
        drawApple(tail);
    }
    drawRect(apple->x, apple->y, 5, 5, &red);
}

// return difference in milliseconds from start to stop
long getmsDiff(struct timespec start, struct timespec stop){
    long diff;
    diff = ((long) stop.tv_sec*1000 + stop.tv_nsec/1E6) 
     - ((long) start.tv_sec*1000 + start.tv_nsec/1E6);
    
    return diff;
}

// return elapsed time in millisecond since prev
long msSince(struct timespec prev){
    struct timespec now;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
    return getmsDiff(prev, now);
}

void gamepad_handler(int signo){
    //les fra driver
    read(description, &input, 1);
    printf("%d\n", input);
    printf("KUK\n");
    if(input == 1){
        direction = 1;
    }
    else if(input == 2){
        direction = 2;
    }
    else if(input == 4){
        direction = 3;
    }
    else if(input == 8){
        direction = 4;
    }
    else if( input == 64){
        direction = 5;
    }
    else{
        direction = 0;
        }
}

int getGpio(){ //setup async
    int oflags;
    printf("Setting up gpio\n");
    input = 0;
    description = open("dev/driver-gamepad", O_RDONLY);
    if(signal(SIGIO, &gamepad_handler) == SIG_ERR){
        //feilhåndtering
    }
    oflags = fcntl(description, F_GETFL);
    if(oflags == -1){
        //feilhndtering
    }
    int err = fcntl(description, F_SETOWN, getpid());
    if (err == -1)
    {
        /* code  gfeilhåndtering*/
    }
    err = fcntl(description, F_SETFL, oflags | FASYNC);
    if (err == -1)
    {
        /* code *///feilhåndtering
    }
    //close(description);
}




