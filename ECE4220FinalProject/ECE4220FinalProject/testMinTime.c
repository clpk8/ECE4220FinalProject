#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h>
#define MY_PRIORITY 51  // kernel is priority 50
#define BTN1  27 //BTN
#define BTN2  0


void *set1()
{

    struct sched_param param;
    param.sched_priority = MY_PRIORITY;  //set priority

    //set scheduling policy and parameters
    if(sched_setscheduler(0, SCHED_FIFO, &param) < 0)  //error check
    {
        printf("sched_setscheduler() failed\n\n");
        exit(-1);
    }

    //create timer object
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);

    //error check
    if(timer_fd < 0)
    {
        printf("Create timer failed.\n");
        exit(-1);
    }

    //create itimerspec
    struct itimerspec itval;

    //set initial time and interval time
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec =40000000;
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_nsec = 300000;

    //set the timer
    timerfd_settime(timer_fd, 0, &itval, NULL);
    uint64_t num_periods = 0;

    //read up to count bytes from file descriptor into the buffer
    if(read(timer_fd, &num_periods, sizeof(num_periods)) < 0)
    {
        printf("Error!\n");
        exit(-1);
    }

    //check missed window
    if(num_periods > 1)
    {
        puts("MISSED WINDOW");
        exit(-1);
    }

    //read sentences
    for(int i = 0; i < 1; i++)
    {
        digitalWrite(BTN1,HIGH);
        //read up to count bytes from file descriptor into the buffer
        if(read(timer_fd, &num_periods, sizeof(num_periods))<0)
        {
            printf("Error!\n");
            exit(-1);
        }
        digitalWrite(BTN1,LOW);


        //check missed windows
        if(num_periods > 1)
        {
            puts("MISSED WINDOW");
            exit(-1);
        }
    }

    pthread_exit(0);  //exit thread
}

void *set2()
{
    struct sched_param param;
    param.sched_priority = MY_PRIORITY;  //set priority

    //set scheduling policy and parameters
    if(sched_setscheduler(0, SCHED_FIFO, &param) < 0)  //error check
    {
        printf("sched_setscheduler() failed\n\n");
        exit(-1);
    }

    //create timer object
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);

    //error check
    if(timer_fd < 0)
    {
        printf("Create timer failed.\n");
        exit(-1);
    }

    //create itimerspec
    struct itimerspec itval;

    //set initial time and interval time
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec =40000000;
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_nsec = 300000;


    //set the timer
    timerfd_settime(timer_fd, 0, &itval, NULL);
    uint64_t num_periods = 0;

    //read up to count bytes from file descriptor into the buffer
    if(read(timer_fd, &num_periods, sizeof(num_periods)) < 0)
    {
        printf("Error!\n");
        exit(-1);
    }

    //check missed window
    if(num_periods > 1)
    {
        puts("MISSED WINDOW");
        exit(-1);
    }

    //read sentences
    for(int i = 0; i < 1; i++)
    {
        digitalWrite(BTN2,HIGH);
        //read up to count bytes from file descriptor into the buffer
        if(read(timer_fd, &num_periods, sizeof(num_periods))<0)
        {
            printf("Error!\n");
            exit(-1);
        }

        digitalWrite(BTN2,LOW);
        //check missed windows
        if(num_periods > 1)
        {
            puts("MISSED WINDOW");
            exit(-1);
        }
    }

    pthread_exit(0);  //exit thread
}


int main (void)
{

    if (wiringPiSetup () < 0) {
        // fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        cerr<< "Not able to setup wiringpi"<<endl;
        return -1;
    }
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);

    digitalWrite(BTN1,LOW);
    digitalWrite(BTN2,LOW);


    int i;
    pthread_t thread1,thread2;

    //create threads
    pthread_create(&thread1, NULL, (void*)&set1, NULL);
    pthread_create(&thread2, NULL, (void*)&set2, NULL);

    //join threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
