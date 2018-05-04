#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>
#include <algorithm>
#include <sqlite3.h>
#include <semaphore.h>
#include <fcntl.h>


sem_t semaphore;

#define MSG_SIZE 200            // message size

using namespace std;

struct sockaddr_in anybody;    // for the socket configuration

sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;
int syncFlag = 0;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}



vector<int> ipID;

// Receiving thr: constantly waits for messages. Whatever is received is displayed.
void *receiving(void *ptr);

int main(int argc, char *argv[])
{
    sem_init(&semaphore, 0, -2);
    
    
    
    /* Open database */
    rc = sqlite3_open("SCADA", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    
    char signal[MSG_SIZE] = "2018-05-02 21:24:32|29|0|0|0|0|0|0|";
    int sock, n;
    //  unsigned int length = sizeof(struct sockaddr_in);    // size of structure
    //    char buffer[MSG_SIZE];        // to store received messages or messages to be sent.
    //    LogData buffer;    // to store received messages or messages to be sent.
    int boolval = 1;            // for a socket option
    pthread_t thread_rec;        // thread variable
    
    if(argc != 2)
    {
        cout << "usage: " << argv[0] << " port" << endl;
        exit(1);
    }
    
    anybody.sin_family = AF_INET;        // symbol constant for Internet domain
    anybody.sin_port = htons(atoi(argv[1]));                // port field
    anybody.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
    if(sock < 0)
        cerr << "Error: socket" << endl;
    //error("Error: socket");
    
    int length = sizeof(anybody);
    bind(sock, (struct sockaddr *)&anybody, length);
    //         if (n < 0)
    //        {
    //            cerr << "Error binding socket." << endl;
    //            //printf("Error binding socket.\n");
    //            exit(-1);
    //        }
    
    // change socket permissions to allow broadcast
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
    {
        cerr << "Error setting socket options." << endl;
        //printf("Error setting socket options.\n");
        exit(-1);
    }
    
    
    anybody.sin_addr.s_addr = inet_addr("128.206.19.255");    // broadcast address (Lab)
    
    
    cout << "This programs displays whatever it receives." << endl;
    cout << "It also transmits whatever the user types, max. 40 char. (! to exit):" << endl;
    
    
    n = sendto(sock, signal, strlen(signal), 0, (const struct sockaddr *)&anybody, length);
    if(n < 0)
        error("Error: sendto");
    pthread_create(&thread_rec, NULL, receiving, (void *)&sock);    // for receiving

    
    sem_wait(&semaphore);
    while(1){
        if(syncFlag == 2){
            sprintf(signal,"%d|%d|%d|",ipID[0],ipID[1],atoi(argv[1]));
            int pipe_N_pipe2;
		//cout << "signal is " << signal << endl;
            if((pipe_N_pipe2 = open("N_pipe2",O_WRONLY)) < 0){
                printf("N_pipe2 error");
                exit(-1);
            }
            
        //    cout << "Signal sended is " << signal << endl;
            if(write(pipe_N_pipe2,&signal,sizeof(signal)) != sizeof(signal)){
                printf("N_pipe2 writing error\n");
                exit(-1);
            }
            syncFlag++;
            
        }
    }
    
    
    //close(sock);            // close socket.
    return 0;
}

void *receiving(void *ptr)
{
    int *sock, n, i;
    char buffer[MSG_SIZE];    // to store received messages or messages to be sent.
    
    int RTUID;
    int Switch1Status, Switch2Status, Button1Status, Button2Status;
    int Voltage;
    string EventOccuried;
    string TimeStamp;
    int PowerFlag;
    char *value;
    int Event;
    
    char delim[] = "|";
    
    sock = (int *)ptr;        // socket identifier
    unsigned int length = sizeof(struct sockaddr_in);        // size of structure
    
    struct sockaddr_in from;
    
    while(1)
    {
        bzero(buffer,MSG_SIZE);    // sets all values to zero. memset() could be used
        // receive message
        
        n = recvfrom(*sock, buffer, 100, 0, (struct sockaddr *)&anybody, &length);
        if(n < 0)
            cerr << "Error: recvfrom" << endl;
        //error("Error: recvfrom");
        
        //cout << "This was received: " << buffer << endl;
        //printf("This was received: %s\n", buffer);
        
        value = strtok(buffer, delim);
        TimeStamp = value;
        
        value = strtok(NULL, delim);
        RTUID = atoi(value);
        
        value = strtok(NULL, delim);
        Switch1Status = atoi(value);
        
        value = strtok(NULL, delim);
        Switch2Status = atoi(value);
        
        value = strtok(NULL, delim);
        Button1Status = atoi(value);
        
        value = strtok(NULL, delim);
        Button2Status = atoi(value);
        
        value = strtok(NULL, delim);
        Voltage = atoi(value);
        
        
        value = strtok(NULL, delim);
        Event = atoi(value);
        
        
        
        if(find(ipID.begin(), ipID.end(), RTUID) == ipID.end()){
            ipID.push_back(RTUID);
            sem_post(&semaphore);
            cout << "1" << endl;
            syncFlag++;
        }
        
        
        
        //        cout << Event << endl << endl;
        
        switch(Event)
        {
            case 0: EventOccuried = "S10FF";
                break;
            case 1: EventOccuried = "S1ON";
                break;
            case 2: EventOccuried = "S2OFF";
                break;
            case 3: EventOccuried = "S2ON";
                break;
            case 4: EventOccuried = "B1OFF";
                break;
            case 5: EventOccuried = "B1ON";
                break;
            case 6: EventOccuried = "B2OFF";
                break;
            case 7: EventOccuried = "B2ON";
                break;
            case 8: EventOccuried = "ADCBOUND";
                break;
            case 9: EventOccuried = "ADCPOWER";
                break;
            case 10: EventOccuried = "LED1ON";
                break;
            case 11: EventOccuried = "LED1OFF";
                break;
            case 12: EventOccuried = "LED2ON";
                break;
            case 13: EventOccuried = "LED2OFF";
                break;
            case 14: EventOccuried = "REGULAR";
                break;
        }
        
        //value = strtok(NULL, delim);
        //PowerFlag = atoi(value);
        
        cout << "This was received: " << TimeStamp << " " << RTUID << " " << Switch1Status << " " << Switch2Status << " " << Button1Status << " " << Button2Status << " " << Voltage << " " << EventOccuried << endl;
        cout << "Vector: " << ipID[0] << endl;
        cout << "Vector: " << ipID[1] << endl;
        
        
        asprintf(&sql, "insert into RTUEventLog (TimeStamp, RTUID, Switch1Status, Switch2Status, Button1Status, Button2Status, Voltage, EventOccuried) values('%s',%d,%d,%d,%d,%d,%d,'%s');",TimeStamp.c_str(),RTUID,Switch1Status,Switch2Status,Button1Status,Button2Status,Voltage,EventOccuried.c_str());
        
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }else{
            fprintf(stdout, "Records created successfully\n");
        }
        
    }
    
    pthread_exit(0);
}










