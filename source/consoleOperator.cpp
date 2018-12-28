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


#define MSG_SIZE 200            // message size

using namespace std;
int main(int argc, char* argv[]){
    //defines
    char signal[MSG_SIZE];
    char buffer[MSG_SIZE];
    char* temp;
    int dummy = system("mkfifo N_pipe2");//create named pipe
    int pipe_N_pipe2;
    if((pipe_N_pipe2 = open("N_pipe2",O_RDONLY)) < 0){
        cout << "N_pipe2 error" << endl;
        exit(-1);
    }
    
    //block untile receive
    if(read(pipe_N_pipe2,&signal,sizeof(signal)) < 0)
        cout << "N_pipe2 reading1 error\n" << endl;
    
    cout << "Signal is:" << signal<<endl;
    char delim[] = "|";//parse by token
    int RTU1, RTU2, port;
    temp = strtok(signal,delim);
    RTU1 = atoi(temp);
    temp = strtok(NULL, delim);
    RTU2 = atoi(temp);
    temp = strtok(NULL, delim);
    port = atoi(temp);

    //define for sockets
    struct sockaddr_in anybody;    // for the socket configuration
    int sock, n;
    int boolval = 1;            // for a socket option
    anybody.sin_family = AF_INET;        // symbol constant for Internet domain
    anybody.sin_port = htons(port);                // port field
    anybody.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
    if(sock < 0)
        cerr << "Error: socket" << endl;
    
    int length = sizeof(anybody);
    bind(sock, (struct sockaddr *)&anybody, length);
    // change socket permissions to allow broadcast
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
    {
        cerr << "Error setting socket options." << endl;
        //printf("Error setting socket options.\n");
        exit(-1);
    }
    
    int choice = 0;
    char ip[14];
    char ch;
    do
    {
        
        //menu to choose RTU1 or RTU2
        if(choice == 0){
            cout << "Which RTU you want to send to? 1 for ID: " << RTU1 << " 2 for ID: " << RTU2 << endl;
            while((ch=getchar()) != '\n' && ch!=EOF);
            scanf("%d",&choice);
            if(choice == 1){
                sprintf(ip, "128.206.19.%d",RTU1);
                cout << "ip is " << ip << endl;
                anybody.sin_addr.s_addr = inet_addr(ip);    // broadcast address (Lab)
                bzero(buffer,MSG_SIZE);    // sets all values to zero. memset() could be used
                cout << "LED1 or LED2" <<endl;
                cin >> buffer;
                
                // send message
                n = sendto(sock, buffer, sizeof(buffer)-1, 0, (const struct sockaddr *)&anybody, length);
                if(n < 0)
                    cout << "error" << endl;
            }
            else if(choice == 2){
                sprintf(ip, "128.206.19.%d",RTU2);
                cout << "ip is " << ip << endl;
                anybody.sin_addr.s_addr = inet_addr(ip);    // broadcast address (Lab)
                bzero(buffer,MSG_SIZE);    // sets all values to zero. memset() could be used
                cout << "LED1 or LED2" <<endl;
                cin >> buffer;
                // send message
                n = sendto(sock, buffer, sizeof(buffer)-1, 0, (const struct sockaddr *)&anybody, length);
                if(n < 0)
                    cout << "error" << endl;
            }
        }

        else{
            cout << "only 2 RTU connected!" << endl;
            choice = 0;
        }

        
    } while(choice != '!');//'!' to quit the program
    
}
