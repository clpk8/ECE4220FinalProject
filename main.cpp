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
#include <sys/timerfd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdlib.h>     /* atoi */
#include <chrono>
#include <pthread.h>
#include <math.h>


using namespace std;

// wiringPi number corresponding to GPIO2.
#define LED1  8//red
#define LED2  9 //yellow
#define LED3  7 //green
#define LED4  21 // blue
#define BTN1  27 //BTN1
#define BTN2  0 //BTN2
#define SW1   26
#define SW2   23
//define for 7-seg display
#define A 5
#define B 6
#define C 25
#define D 2
#define DP 29


int portNum;//store the argument value
//flags used to indicate the status of LEDs
int LED1Flag;
int LED2Flag;
//----------------Time used for rebounce ------------------
//push button might triger many time because of pressure
struct timeval interruptTimeB1, lastInterruptTimeB1;
struct timeval interruptTimeB2, lastInterruptTimeB2;

//---------------------------ADC----------------------------
#define SPI_CHANNEL         0// 0 or 1
#define SPI_SPEED     2000000    // Max speed is 3.6 MHz when VDD = 5 V
#define ADC_CHANNEL       3    // Between 0 and 3

//simple seven segment display program
void sevenSeg(int decimal){
    //We've only used the seven segment display for the status of buttons and switches
    if(decimal < 8){
        
        //decimal to binary
        digitalWrite(DP, HIGH);
        int a[4] = {0};
        int i;
        for(i = 0; decimal > 0; i++){
            a[i] = decimal % 2;
            decimal = decimal / 2;
        }
        //MSB first LSB last
        for(i = 3; i>= 0; i--){
            if(i == 3)
                digitalWrite(D, a[i]);
            else if(i == 2)
                digitalWrite(C, a[i]);
            else if(i == 1)
                digitalWrite(B, a[i]);
            else if(i == 0)
                digitalWrite(A, a[i]);
        }
    }
}

//Enums used for tracking event
//can be seeing as "EVENT ID" from 0 to 14
enum typeEvent{
    S1OFF,
    S1ON,
    S2OFF,
    S2ON,
    B1OFF,
    B1ON,
    B2OFF,
    B2ON,
    ADCBOUND,//ADC out of bound
    ADCPOWER,//ADC no power
    LED1ON,
    LED1OFF,
    LED2ON,
    LED2OFF,
    REGULAR//Regultory 1 second update
};

//structure created to store all the information needed to be logged
struct LogData
{
    //--------------------Used to store/get times-------------------
    char timeBuffer [26];
    time_t rawtime;
    struct tm * timeinfo;
    struct timeval tvl;
    
    //--------------Basic information need to be logged--------------
    int RTUid;
    //buttons, True for on and open. False for closed and off
    bool S1;
    bool S2;
    bool B1;
    bool B2;
    unsigned short Voltage;
    typeEvent typeEventID;
    char sendBuffer[200];
};

//RTU classes
class RTU{
private:
    LogData RTULogData;//Log data
public:
    //public methods, getters, setters and constructors
    int returnTypeEvent(){return RTULogData.typeEventID;}
    string getSendBuffer();
    int count[4] = {0,0,0,0};
    RTU();
    void setTime();
    void setRTUid(int id);
    void setStatus(int choice, bool change);
    void setVoltage(unsigned short V);
    void setTypeEvent(typeEvent str);
    void print();
    void clearTypeEvent();
    void concatBuffer();
    
};
//concatenate a buffer with data separated by "|"
void RTU::concatBuffer(){
    bzero(RTULogData.sendBuffer, 200);
    char token = '|';
    sprintf(RTULogData.sendBuffer, "%s%c%d%c%d%c%d%c%d%c%d%c%d%c%u%c",RTULogData.timeBuffer,token, RTULogData.RTUid,token,RTULogData.S1,token,RTULogData.S2,token,RTULogData.B1,token,RTULogData.B2,token,RTULogData.Voltage,token, RTULogData.typeEventID,token);
    //vertify the buffer
    cout << "send buffer is " << RTULogData.sendBuffer << endl;
}
//getter for send buffer
string RTU::getSendBuffer(){
    string str(RTULogData.sendBuffer);
    return str;
}
//clear the event for regular update
void RTU::clearTypeEvent(){
    RTULogData.typeEventID = REGULAR;
}

//print all the informations
void RTU::print(){
    cout << "Status for S1,S2,B1,B2:" << RTULogData.S1 << " " << RTULogData.S2 << " " << RTULogData.B1 << " " << RTULogData.B2 << " " << endl;
    cout << "Status for S1,S2,B1,B2:" << count[2] << " " << count[3] << " " << count[0] << " " << count[1] << " " << endl;
    
    cout << "Voltage value is: " << RTULogData.Voltage << endl;
    cout << "The event happened is " << RTULogData.typeEventID << endl;
    cout << "Time stamp : " << RTULogData.timeBuffer << endl;
    cout << "my buffer is " << RTULogData.sendBuffer << endl;
}
//set the time of event
void RTU::setTime(){
    char tempBuffer[26];//store the time string
    int millisec;
    gettimeofday(&RTULogData.tvl, NULL);//get time of date
    //used to calculate the time to millsecond
    millisec = lrint(RTULogData.tvl.tv_usec / 1000.0); //round to nearst millsecond
    if(millisec >= 1000){
        millisec -= 1000;
        RTULogData.tvl.tv_sec++;
    }
    //format the time for our database
    RTULogData.timeinfo = localtime (&RTULogData.tvl.tv_sec);
    strftime(tempBuffer, sizeof(tempBuffer), "%Y-%m-%d %H:%M:%S", RTULogData.timeinfo);
    sprintf(RTULogData.timeBuffer, "%s.%03d",tempBuffer,millisec);

}
//ID setter
void RTU::setRTUid(int id){
    RTULogData.RTUid = id;
}
//button/switch setter
void RTU::setStatus(int choice, bool change){
    switch (choice) {
        case 1:
            RTULogData.B1 = change;
            break;
        case 2:
            RTULogData.B2 = change;
            break;
        case 3:
            RTULogData.S1 = change;
            break;
        case 4:
            RTULogData.S2 = change;
            break;
        default:
            cout << "set status failed" << endl;
            break;
    }
}
//voltage setter
void RTU::setVoltage(unsigned short V){
    RTULogData.Voltage = V;
}
//type event setter
void RTU::setTypeEvent(typeEvent id){
    RTULogData.typeEventID = id;
}
//constructor
RTU::RTU(){
    cout << "Globel RTU initilize"<<endl;
}

//event counter
volatile int eventCounter = 0;

RTU r1; //initilize R1 as a globel variable

//example adc program
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

//socket class
class socketObj{
private:
    //-----------------------used for getting ip and RTUid------------------------------
    struct ifreq ifr;
    char ip_address[13];
    const char s[2] = " ";
    const char c[2] = ".";
    int myMachine;

    //----------------------------parameters for socket-----------------------------
    int sock;
    int length;
    int n;
    int port;
    struct sockaddr_in server;
    struct sockaddr_in client;
    struct sockaddr_in clientCommand;
    RTU r1;
    char buf[7];//command from console operator
    socklen_t fromlen;
public:
    int getRTUID();
    void getPort(int num);
    void setupSocket();
    void send(RTU r1);
    void getIP();
    string receiveFrom();
};
//get the command from console operator
string socketObj::receiveFrom(){
    n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&clientCommand, &fromlen);
    if(n < 0)
        cout << "Receive error " << endl;
    
    string str(buf);
    return str;
}
//get RTU id
int socketObj::getRTUID(){
    return myMachine;
}
//get the IP dynamically
void socketObj::getIP(){
    //get IP
    /*AF_INET - to define IPv4 Address type.*/
    ifr.ifr_addr.sa_family = AF_INET;
    /*wlan0 - define the ifr_name - port name
     where network attached.*/
    memcpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    
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
    myMachine = atoi(token);
}
//get the port number
void socketObj::getPort(int num){
    port = num;
}
//socket setup
void socketObj::setupSocket(){
    int boolval = 1; //use for socket option, to allow broadcast
    sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
    if (sock < 0){
        cerr<<"create socker error"<<endl;
        
    }
    
    length = sizeof(server);            // length of structure
    bzero(&server,length);            // sets all values to zero. memset() could be used
    
    //initilize the server
    server.sin_family = AF_INET;        // symbol constant for Internet domain
    server.sin_addr.s_addr = INADDR_ANY;        // IP address of the machine on which
    // the server is running
    server.sin_port = htons(port);    // port number
    
    
    
    // binds the socket to the address of the host and the port number
    if (bind(sock, (struct sockaddr *)&server, length) < 0){
        cerr<<"bind Error"<<endl;
    }
    // change socket permissions to allow broadcast
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
    {
        cerr<<"setup Socket Error"<<endl;
    }
    fromlen = sizeof(struct sockaddr_in);    // size of structure
    
    
    n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client, &fromlen);
    if(n < 0)
        cerr << "receive error " << endl;
    
    //get the length
}

//send the buffer from RTU class to socket
void socketObj::send(RTU r1){
    string temp = r1.getSendBuffer();
    char *tempBuf = (char*)temp.c_str();
    n = sendto(sock,tempBuf, 200, 0, (struct sockaddr *)&client, fromlen);
    if(n < 0 )
        cout << "send error" << endl;
    
}

socketObj s1;//constructed socket object

//-----------------------------turn LED threads---------------------------------
void *turnLEDS(void* ptr){
    cout << "Thread turnLEDS initilzied" << endl;
    while(1){
        string command = s1.receiveFrom();
        //   cout << "Command is "<< command;
        string led1("LED1");
        string led2("LED2");
        
        if(command.compare(led1) == 0 ){
            if(LED1Flag == 0){
                digitalWrite(LED1,HIGH);
                r1.setTime();
                r1.setTypeEvent(LED1ON);
            //    r1.print();
                r1.concatBuffer();
                s1.send(r1);
                sevenSeg(r1.returnTypeEvent());
                LED1Flag = 1;
            }
            else if(LED1Flag == 1){
                digitalWrite(LED1,LOW);
                r1.setTime();
                r1.setTypeEvent(LED1OFF);
              //  r1.print();
                r1.concatBuffer();
                s1.send(r1);
                sevenSeg(r1.returnTypeEvent());
                LED1Flag = 0;
            }
        }
        else if(command.compare(led2) == 0){
            if(LED2Flag == 0){
                digitalWrite(LED2,HIGH);
                r1.setTime();
                r1.setTypeEvent(LED2ON);
             //   r1.print();
                r1.concatBuffer();
                s1.send(r1);
                sevenSeg(r1.returnTypeEvent());
                LED2Flag = 1;
            }
            else if(LED2Flag == 1){
                digitalWrite(LED2,LOW);
                r1.setTime();
                r1.setTypeEvent(LED2OFF);
            //    r1.print();
                r1.concatBuffer();
                s1.send(r1);
                sevenSeg(r1.returnTypeEvent());
                LED2Flag = 0;
            }
        }
        else
            cout << "Command received is "<< command << endl;
    }
    
}

//-----------------------------Button and Switch Iterrupt handler---------------------------
void B1Interrupt() {
    gettimeofday(&interruptTimeB1, NULL);
    cout << "interrupt happened" << endl;
    if((interruptTimeB1.tv_sec - lastInterruptTimeB1.tv_sec)*1000000 + (interruptTimeB1.tv_usec - lastInterruptTimeB1.tv_usec) > 250000){
        r1.setTime();
        r1.count[0]++;
        //odd is on
        if(r1.count[0] %2 == 1){
            r1.setStatus(1, true);
            r1.setTypeEvent(B1ON);
        }
        else{
            r1.setStatus(1, false);
            r1.setTypeEvent(B1OFF);
        }
     //   r1.print();
        r1.concatBuffer();

        s1.send(r1);
        cout << "event is " << r1.returnTypeEvent() << endl;
        sevenSeg(r1.returnTypeEvent());

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
            r1.setTypeEvent(B2ON);
        }
        else{
            r1.setStatus(2, false);
            r1.setTypeEvent(B2OFF);
        }
    //    r1.print();
        r1.concatBuffer();

        s1.send(r1);
        sevenSeg(r1.returnTypeEvent());

        
        
    }
    
    lastInterruptTimeB2.tv_usec = interruptTimeB2.tv_usec;
    
}
void S1Interrupt() {
    
    r1.setTime();
    r1.count[2]++;
    if(r1.count[2] %2 == 1){
        r1.setStatus(3, true);
        r1.setTypeEvent(S1ON);
    }
    else{
        r1.setStatus(3, false);
        r1.setTypeEvent(S1OFF);
    }
 //   r1.print();
    r1.concatBuffer();

    s1.send(r1);
    sevenSeg(r1.returnTypeEvent());

    
}
void S2Interrupt() {
    
    
    r1.setTime();
    r1.count[3]++;
    if(r1.count[3] %2 == 1){
        r1.setStatus(4, true);
        r1.setTypeEvent(S2ON);
        
    }
    else{
        r1.setStatus(4, false);
        r1.setTypeEvent(S2OFF);
    }
 //   r1.print();
    r1.concatBuffer();

    s1.send(r1);
    sevenSeg(r1.returnTypeEvent());
    
}


//----------------------------Reading ADC thread-------------------------------
void *readingADC(void* ptr){
    uint16_t ADCvalue;
    uint16_t PADCvalue = 0;
    // Configure the SPI
    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0) {
        cout << "wiringPiSPISetup failed" << endl;
        //return -1 ;
    }
    
    //set up scheduler
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
        cout << "Readfile " << endl;
    }
    
    if(num_periods > 1){
        cout << "MISSED WINDLW " << endl;
    }
    
    //set bounds and flags
    int adcUpperBound = 350;
    int adcLowerBound = 150;
    int i = 0;
    int noPowerFlag = 0;
    int adcBoundFlag = 0;

    while(1){
        long check1 = read(timer_fd, &num_periods, sizeof(num_periods));
        if(check1 < 0){
            cout << "Readfile " << endl;
        }
        
        if(num_periods > 1){
            cout << "Readfile " << endl;
        }
        
        //read and set values
        ADCvalue = get_ADC(ADC_CHANNEL);
        r1.setVoltage(ADCvalue);
        
        
        //flags used to make sure the event only being logged once
        //other wise there will be so many events
        if(ADCvalue == PADCvalue){
            if(noPowerFlag == 0){
                r1.setTime();
                r1.setTypeEvent(ADCPOWER);
                r1.concatBuffer();
                sevenSeg(r1.returnTypeEvent());
                s1.send(r1);
                noPowerFlag = 1;
            }
        }
        else{
            PADCvalue = ADCvalue;
            noPowerFlag = 0;
            if(ADCvalue > adcUpperBound || ADCvalue < adcLowerBound){
                if(adcBoundFlag == 0){
                    r1.setTime();
                    r1.setTypeEvent(ADCBOUND);
                    r1.concatBuffer();
                    s1.send(r1);
                    sevenSeg(r1.returnTypeEvent());
                    adcBoundFlag = 1;
                }
            }
            else
                adcBoundFlag = 0;
        }

    }
    
}


//-----------------------------wiringpi setup and pinmode-------------------------
int setupWiringPiFunction() {
    // sets up the wiringPi library
    if (wiringPiSetup () < 0) {
        // fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        cerr<< "Not able to setup wiringpi"<<endl;
        return -1;
    }
    
    pinMode(LED1, OUTPUT);    // Configure GPIO2, which is the one connected to the LED.
    pinMode(LED2, OUTPUT);    // Configure GPIO2, which is the one connected to the LED.
    pinMode(LED3, OUTPUT);    // Configure GPIO2, which is the one connected to the LED.
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(SW1, INPUT);
    pinMode(SW2, INPUT);
    
    //configure 7-seg display
    pinMode(DP, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    
    
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);
    
    digitalWrite(A,LOW);
    digitalWrite(B,LOW);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    
    
    LED1Flag = 0;
    LED2Flag = 0;
    
    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    
    //-----------------------wiringpi GPIO interrupt setup -------------------------
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


int main(int argc, const char * argv[]) {
    
    
    //set up socket
    s1.getPort(atoi(argv[1]));
    s1.setupSocket();
    s1.getIP();
    //RTU r1;
    if(setupWiringPiFunction() < 0 ){
        cerr << "Error setup RUT" << endl;
        return -1;
    }
    //used for rebounse
    gettimeofday(&lastInterruptTimeB1, NULL);
    gettimeofday(&lastInterruptTimeB2, NULL);
    
    //makesure port number is provided
    if(argc < 2){
        
        cout << "Please enter port number" << endl;
        return -1;
    }
    
    r1.setRTUid(s1.getRTUID());//dynamically get ID
    
    //create threads
    pthread_t adcReading, receiveThread;
    pthread_create(&adcReading, NULL, readingADC, NULL);
    pthread_create(&receiveThread, NULL, turnLEDS, NULL);
    
    // ---------------------set up real time task -------------------------
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
    itval.it_interval.tv_sec = 1;
    itval.it_interval.tv_nsec = 0;//period of 100 ms
    
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_nsec = 100;//start a little bit late first time
    
    timerfd_settime(timer_fd, 0, &itval, NULL);
    
    //read to get it sync
    uint64_t num_periods = 0;
    long check1 = read(timer_fd, &num_periods, sizeof(num_periods));
    if(check1 < 0){
        cout << "Readfile " << endl;
    }
    
    if(num_periods > 1){
        cout << "MISSED WINDLW " << endl;
    }
    
    
    while ( 1 ) {
        
        long check1 = read(timer_fd, &num_periods, sizeof(num_periods));
        if(check1 < 0){
            cout << "Readfile " << endl;
        }
        
        if(num_periods > 1){
            cout << "Readfile " << endl;
        }
        //periodic update
        r1.setTime();
        r1.clearTypeEvent();
        r1.concatBuffer();
        s1.send(r1);
        eventCounter = 0;
    }
    
    
    
    return 0;
}
