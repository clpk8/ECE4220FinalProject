//
//  main.cpp
//  ECE4220FinalProject
//
//  Created by linChunbin on 4/24/18.
//  Copyright © 2018 linChunbin. All rights reserved.
//

#include <iostream>
#include <wiringPi.h>
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>

using namespace std;
#define LED1  8        // wiringPi number corresponding to GPIO2.
#define LED2  9 //yellow
#define LED3  7 //green
#define LED4  21 //blue
#define BTN1  27 //BTN
#define BTN2  0
#define SW1   26
#define SW2   23
#define MSG_SIZE 40            // message size
int portNum;
time_t rawtime;
struct tm * timeinfo;

//event counter
volatile int eventCounter = 0;

void myInterrupt(void) {
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    //printf ("Current local time and date: %s", asctime(timeinfo));
    cout << "Current local time and tate is : " << asctime(timeinfo) << endl;
    //eventCounter++;
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);
    
    delay(500);
    
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH);
    
    delay(500);
}

void wiringPiSetup(){
    // sets up the wiringPi library
    if (wiringPiSetup () < 0) {
        // fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        cerr<< "Not able to setup wiringpi"<<endl;
        return 1;
    }
    pinMode(LED1, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(LED2, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(LED3, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(SW1, INPUT);
    pinMode(SW2, INPUT);
    
    
    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if ( wiringPiISR (BTN1, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }
    if ( wiringPiISR (BTN2, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }
    if ( wiringPiISR (SW1, INT_EDGE_BOTH, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }
    if ( wiringPiISR (SW2, INT_EDGE_BOTH, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }
}
int main(int argc, const char * argv[]) {
    wiringPiSetup();
    

    //makesure port number is provided
    if(argc < 2){
        
        printf("Error, Please enter the port number");
        exit(-1);
    }

    
    int sock, length, n;
    struct sockaddr_in server, broadcast, clint; //define structures
    char buf[MSG_SIZE]; //define buf
    socklen_t fromlen;
    struct ifreq ifr;
    char ip_address[13];
    //get IP
    /*AF_INET - to define IPv4 Address type.*/
    ifr.ifr_addr.sa_family = AF_INET;
    /*wlan0 - define the ifr_name - port name
     where network attached.*/
    memcpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    
    //set up the socket
    sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
    if (sock < 0)
        error("Opening socket");
    
    length = sizeof(broadcast);            // length of structure
    bzero(&broadcast,length);            // sets all values to zero. memset() could be used
    
    length = sizeof(server);            // length of structure
    bzero(&server,length);            // sets all values to zero. memset() could be used
    
    //initilize the server
    server.sin_family = AF_INET;        // symbol constant for Internet domain
    server.sin_addr.s_addr = INADDR_ANY;        // IP address of the machine on which
    // the server is running
    server.sin_port = htons(atoi(argv[1]));    // port number

    /*Accessing network interface information by
     passing address using ioctl.*/
    ioctl(sock, SIOCGIFADDR, &ifr);
    
    strcpy(ip_address,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    
    //parsing to get machine number
    printf("System IP Address is: %s\n",ip_address);
    char temp[13];
    strcpy(temp, ip_address);
    char* token = strtok(temp,c);
    while(token != NULL){
        strcpy(temp, token);
        token = strtok(NULL,c);
    }
    token = strtok(temp, s);
    int myMachine = atoi(token);
    
    // binds the socket to the address of the host and the port number
    if (bind(sock, (struct sockaddr *)&server, length) < 0)
        //        error("binding");
        printf("2");
    // change socket permissions to allow broadcast
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
    {
        printf("error setting socket options\n");
        exit(-1);
    }
    
    //set up broadcast
    broadcast.sin_addr.s_addr = inet_addr("128.206.19.255");
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(atoi(argv[1]));    // port number
    
    //get the length
    fromlen = sizeof(struct sockaddr_in);    // size of structure
    
    cout<<"RUTid is "<<myMachine<<endl;
    
    while ( 1 ) {
        pullUpDnControl(BTN1,PUD_DOWN);//first set the push button's register down for input
        pullUpDnControl(BTN2,PUD_DOWN);//first set the push button's register down for input
        cout << eventCounter<<endl;
        eventCounter = 0;
        delay( 1000 ); // wait 1 second
    }

    

    return 0;
}
