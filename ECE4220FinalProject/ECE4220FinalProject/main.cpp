//
//  main.cpp
//  ECE4220FinalProject
//
//  Created by linChunbin on 4/24/18.
//  Copyright © 2018 linChunbin. All rights reserved.
//

#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdlib.h>     /* atoi */
#include <chrono>
#include <pthread.h>


using namespace std;
#define LED1  8        // wiringPi number corresponding to GPIO2.
#define LED2  9 //yellow
#define LED3  7 //green
#define LED4  21 // blue
#define BTN1  27 //BTN
#define BTN2  0
#define SW1   26
#define SW2   23
#define MSG_SIZE 40            // message size
int portNum;
//----------------Time used for rebounce ------------------
struct timeval interruptTimeB1, lastInterruptTimeB1;
struct timeval interruptTimeB2, lastInterruptTimeB2;

//---------------------------ADC----------------------------
#define SPI_CHANNEL         0// 0 or 1
#define SPI_SPEED     2000000    // Max speed is 3.6 MHz when VDD = 5 V
#define ADC_CHANNEL       3    // Between 0 and 3

using namespace std;
class RTU{
private:
    time_t rawtime;
    struct tm * timeinfo;
    int RTUid;
    //buttons, True for on and open. False for closed and off
    bool S1;
    bool S2;
    bool B1;
    bool B2;
    unsigned short Voltage;
    string typeEvent;
public:
    int count[4] = {0,0,0,0};
    RTU();
    void setTime();
    void setRTUid(int id);
    void setStatus(int choice, bool change);
    void setVoltage(unsigned short V);
    void setTypeEvent(string str);
    void print();

};
void RTU::print(){
    cout << "Status for S1,S2,B1,B2:" << S1 << " " << S2 << " " << B1 << " " << B2 << " " << endl;
    cout << "Status for S1,S2,B1,B2:" << count[2] << " " << count[3] << " " << count[0] << " " << count[1] << " " << endl;

    cout << "The event happened is " << typeEvent << endl;
}
void RTU::setTime(){
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    const int TIME_STRING_LENGTH = 20;
    char buffer [TIME_STRING_LENGTH];
    strftime(buffer, TIME_STRING_LENGTH, "%Y-%m-%d %H:%M:%S", timeinfo);
    cout << "Current local time and date: " << asctime(timeinfo) << endl;
    cout << buffer << endl;
}
void RTU::setRTUid(int id){
    RTUid = id;
}
void RTU::setStatus(int choice, bool change){
    switch (choice) {
        case 1:
            B1 = change;
            break;
        case 2:
            B2 = change;
            break;
        case 3:
            S1 = change;
            break;
        case 4:
            S2 = change;
            break;
        default:
            cout << "set status failed" << endl;
            break;
    }
}
void RTU::setVoltage(unsigned short V){
    Voltage = V;
}
void RTU::setTypeEvent(string str){
    typeEvent = str;
}
RTU::RTU(){
    cout << "Globel RTU initilize"<<endl;
}

//event counter
volatile int eventCounter = 0;




RTU r1;
void B1Interrupt() {
    gettimeofday(&interruptTimeB1, NULL);
    cout << "interrupt happened" << endl;
    if((interruptTimeB1.tv_sec - lastInterruptTimeB1.tv_sec)*1000000 + (interruptTimeB1.tv_usec - lastInterruptTimeB1.tv_usec) > 250000){
        r1.setTime();
        r1.count[0]++;
        //odd is on
        if(r1.count[0] %2 == 1){
            r1.setStatus(1, true);
            r1.setTypeEvent("Button is turn on");
        }
        else{
            r1.setStatus(1, false);
            r1.setTypeEvent("Button is turn off");
        }
        //printf ("Current local time and date: %s", asctime(timeinfo));
        //  cout << "Current local time and tate is : " << asctime(timeinfo);
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
    lastInterruptTimeB1.tv_usec = interruptTimeB1.tv_usec;

}
void B2Interrupt() {

    gettimeofday(&interruptTimeB2, NULL);
    cout << "interrupt happened" << endl;
    if(((interruptTimeB2.tv_sec - lastInterruptTimeB2.tv_sec)*1000000 + (interruptTimeB2.tv_usec - lastInterruptTimeB2.tv_usec) > 250000)){
        r1.setTime();
        r1.count[1]++;
        if(r1.count[1] %2 == 1){
            r1.setStatus(2, true);
            r1.setTypeEvent("Button 2 is turn on");
        }
        else{
            r1.setStatus(2, false);
            r1.setTypeEvent("Button 2 is turn off");
        }
        //printf ("Current local time and date: %s", asctime(timeinfo));
        //  cout << "Current local time and tate is : " << asctime(timeinfo);
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

    lastInterruptTimeB2.tv_usec = interruptTimeB2.tv_usec;

}
void S1Interrupt() {

    r1.setTime();
    r1.count[2]++;
    if(r1.count[2] %2 == 1){
        r1.setStatus(3, true);
        r1.setTypeEvent("Switch 1 is turn on");
    }
    else{
        r1.setStatus(3, false);
        r1.setTypeEvent("Switch 1 is turn off");
    }
    //printf ("Current local time and date: %s", asctime(timeinfo));
    //  cout << "Current local time and tate is : " << asctime(timeinfo);
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
void S2Interrupt() {

    r1.setTime();
    r1.count[3]++;
    if(r1.count[3] %2 == 1){
        r1.setStatus(4, true);
        r1.setTypeEvent("Switch 2 is turn on");
    }
    else{
        r1.setStatus(4, false);
        r1.setTypeEvent("Switch 2 is turn off");
    }
    //printf ("Current local time and date: %s", asctime(timeinfo));
    //  cout << "Current local time and tate is : " << asctime(timeinfo);
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


int setupWiringPiFunction() {
    // sets up the wiringPi library
    if (wiringPiSetup () < 0) {
        // fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        cerr<< "Not able to setup wiringpi"<<endl;
        return -1;
    }
    pinMode(LED1, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(LED2, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(LED3, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(SW1, INPUT);
    pinMode(SW2, INPUT);


    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if ( wiringPiISR (BTN1, INT_EDGE_FALLING, &B1Interrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return -1;
    }
    if ( wiringPiISR (BTN2, INT_EDGE_FALLING, &B2Interrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return -1;
    }
    if ( wiringPiISR (SW1, INT_EDGE_BOTH, &S1Interrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return -1;
    }
    if ( wiringPiISR (SW2, INT_EDGE_BOTH, &S2Interrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return -1;
    }
}

uint16_t get_ADC(int ADC_chan)
{
    uint8_t spiData[3];
    spiData[0] = 0b00000001; // Contains the Start Bit
    spiData[1] = 0b10000000 | (ADC_chan << 4);    // Mode and Channel: M0XX0000
    // M = 1 ==> single ended
    // XX: channel selection: 00, 01, 10 or 11
    spiData[2] = 0;    // "Don't care", this value doesn't matter.
    
    // The next function performs a simultaneous write/read transaction over the selected
    // SPI bus. Data that was in the spiData buffer is overwritten by data returned from
    // the SPI bus.
    wiringPiSPIDataRW(SPI_CHANNEL, spiData, 3);
    
    // spiData[1] and spiData[2] now have the result (2 bits and 8 bits, respectively)
    
    return ((spiData[1] << 8) | spiData[2]);
}

void readingADC(void* ptr){
    uint16_t ADCvalue;
    
    // Configure the SPI
    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0) {
        cout << "wiringPiSPISetup failed" << endl;
        //return -1 ;
    }
    
    
    struct sched_param param;
    param.sched_priority = 51;
    int check = sched_setscheduler(0, SCHED_FIFO, &param); //using FIFO
    //check error
    
    if(check < 0){
        cout << "Error assignning priority" << endl;
    }
    
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if(timer_fd < 0){
        cout << "Create timer error" << endl;
        //exit(-1);
    }
    //set timmer
    struct itimerspec itval;
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec = 100000000;//period of 100 ms
    
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_nsec = 100;//start a little bit late first time
    
    timerfd_settime(timer_fd, 0, &itval, NULL);
    
    //read to get it sync
    uint64_t num_periods = 0;
    long check1 = read(timer_fd, &num_periods, sizeof(num_periods));
    if(check1 < 0){
        printf("Readfile\n");
    }
    
    if(num_periods > 1){
        puts("MISSED WINDOW1\n");
    }
    

    while(1){
        long check1 = read(timer_fd, &num_periods, sizeof(num_periods));
        if(check1 < 0){
            printf("Readfile\n");
        }
        
        if(num_periods > 1){
            puts("MISSED WINDOW2\n");
        }
        
        ADCvalue = get_ADC(ADC_CHANNEL);
        cout<< "ADC Value: " << ADCvalue << endl;
        fprintf(fp,"%d\n",ADCvalue);
        fflush(stdout);
     //   r1.setTime();
     //   r1.setVoltage(ADCvalue);
     //   r1.setTypeEvent("ADC volatege")
        //usleep(1000);
    }
    
}
int main(int argc, const char * argv[]) {

    //RTU r1;
    if(setupWiringPiFunction() < 0 ){
        cerr << "Error setup RUT" << endl;
        return -1;
    }

    gettimeofday(&lastInterruptTimeB1, NULL);
    gettimeofday(&lastInterruptTimeB2, NULL);

    //makesure port number is provided
    if(argc < 2){

        cout << "Please enter port number" << endl;
        return -1;
    }


    int sock, length, n;
    int boolval = 1; //use for socket option, to allow broadcast
    struct sockaddr_in server, broadcast, clint; //define structures
    char buf[MSG_SIZE]; //define buf
    socklen_t fromlen;
    struct ifreq ifr;
    char ip_address[13];
    const char s[2] = " ";
    const char c[2] = ".";
    //get IP
    /*AF_INET - to define IPv4 Address type.*/
    ifr.ifr_addr.sa_family = AF_INET;
    /*wlan0 - define the ifr_name - port name
     where network attached.*/
    memcpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);

    //set up the socket
    sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
    if (sock < 0){
        cerr<<"create socker error"<<endl;
        return -1;

    }

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
    cout << "System IP Address is : "<< ip_address << endl;
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
    if (bind(sock, (struct sockaddr *)&server, length) < 0){
        cerr<<"bind Error"<<endl;
        return -1;
    }

    // change socket permissions to allow broadcast
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
    {
        cerr<<"setup Socket Error"<<endl;
        return -1;
    }

    //set up broadcast
    broadcast.sin_addr.s_addr = inet_addr("128.206.19.255");
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(atoi(argv[1]));    // port number

    //get the length
    fromlen = sizeof(struct sockaddr_in);    // size of structure

    cout<<"RUT id is "<<myMachine<<endl;

    while ( 1 ) {
        r1.print();
        pullUpDnControl(BTN1,PUD_DOWN);//first set the push button's register down for input
        pullUpDnControl(BTN2,PUD_DOWN);//first set the push button's register down for input
        cout << eventCounter<<endl;
        eventCounter = 0;
        delay( 1000 ); // wait 1 second
    }



    return 0;
}
