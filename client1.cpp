#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>

#define MSG_SIZE 200			// message size

using namespace std;

struct sockaddr_in anybody;	// for the socket configuration

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// Receiving thr: constantly waits for messages. Whatever is received is displayed.
void *receiving(void *ptr);

int main(int argc, char *argv[])
{
    char signal[MSG_SIZE] = "2018-05-02 21:24:32|29|0|0|0|0|0|0|";
    int sock, n;
    //  unsigned int length = sizeof(struct sockaddr_in);	// size of structure
    //	char buffer[MSG_SIZE];		// to store received messages or messages to be sent.
    //	LogData buffer;	// to store received messages or messages to be sent.
    int boolval = 1;			// for a socket option
    pthread_t thread_rec;		// thread variable

    if(argc != 2)
    {
        cout << "usage: " << argv[0] << " port" << endl;
        exit(1);
    }

    anybody.sin_family = AF_INET;		// symbol constant for Internet domain
    anybody.sin_port = htons(atoi(argv[1]));				// port field
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

    anybody.sin_addr.s_addr = inet_addr("128.206.19.255");	// broadcast address (Lab)

    pthread_create(&thread_rec, NULL, receiving, (void *)&sock);	// for receiving

    cout << "This programs displays whatever it receives." << endl;
    cout << "It also transmits whatever the user types, max. 40 char. (! to exit):" << endl;

    //printf("This programs displays whatever it receives.\n");
    //printf("It also transmits whatever the user types, max. 40 char. (! to exit):\n");
    do
    {
        if(signal[0] != '!')
        {
            // send message to anyone there...
            n = sendto(sock, signal, strlen(signal), 0, (const struct sockaddr *)&anybody, length);
            if(n < 0)
                error("Error: sendto");
        }

        // bzero: to "clean up" the buffer. The messages aren't always the same length.
        bzero(signal,MSG_SIZE);	// sets all values to zero. memset() could be used
        //cleanbuffer(&buffer);
        fgets(signal,MSG_SIZE-1,stdin); // MSG_SIZE-1 'cause a null character is added


    } while(signal[0] != '!');

    close(sock);			// close socket.
    return 0;
}

void *receiving(void *ptr)
{
    int *sock, n;
    char buffer[MSG_SIZE];	// to store received messages or messages to be sent.

    int EventID;
    int Switch1Status, Switch2Status, Button1Status, Button2Status;
    int Voltage;
    string EventOccuried;
    string TimeStamp;
    int PowerFlag;
    char *value;
    int Event;

    char delim[] = "|";

    sock = (int *)ptr;		// socket identifier
    unsigned int length = sizeof(struct sockaddr_in);		// size of structure

    struct sockaddr_in from;

    while(1)
    {
        bzero(buffer,MSG_SIZE);	// sets all values to zero. memset() could be used
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
        EventID = atoi(value);

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

        cout << "This was received: " << TimeStamp << " " << EventID << " " << Switch1Status << " " << Switch2Status << " " << Button1Status << " " << Button2Status << " " << Voltage << " " << EventOccuried << endl;
    }

    pthread_exit(0);
}
