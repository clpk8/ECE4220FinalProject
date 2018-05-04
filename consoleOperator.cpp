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
    char signal[MSG_SIZE];
    char buffer[MSG_SIZE];
    char* temp;
    int dummy = system("mkfifo N_pipe2");

    int pipe_N_pipe2;
    if((pipe_N_pipe2 = open("N_pipe2",O_RDONLY)) < 0){
        cout << "N_pipe2 error" << endl;
        exit(-1);
    }

//	cout << "1" << endl;    
    if(read(pipe_N_pipe2,&signal,sizeof(signal)) < 0)
        cout << "N_pipe2 reading1 error\n" << endl;

    cout << "Signal is:" << signal<<endl;
    char delim[] = "|";
	cout << "1" << endl;	
	cout << signal << endl;    
    int RTU1, RTU2, port;
    temp = strtok(signal,delim);
	cout << temp << endl;
    RTU1 = atoi(temp);
  	cout << RTU1 << endl;  
    temp = strtok(NULL, delim);
    RTU2 = atoi(temp);
    	cout << RTU2 << endl;
    temp = strtok(NULL, delim);
    port = atoi(temp);
    	cout << port << endl;
    
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
        if(choice == 1){
            cout << "Which RTU you want to send to? 1 for ID: " << RTU1 << " 2 for ID: " << RTU2 << endl;
            cin >> choice;
            if(choice == 1){
                //  while((ch=getchar()) != '\n' && ch!=EOF);
                sprintf(ip, "128.206.19.%d",RTU1);
                cout << "ip is " << ip << endl;
                anybody.sin_addr.s_addr = inet_addr(ip);    // broadcast address (Lab)
                // bzero: to "clean up" the buffer. The messages aren't always the same length.
                bzero(buffer,MSG_SIZE);    // sets all values to zero. memset() could be used
                //cleanbuffer(&buffer);
                cout << "LED1 or LED2" <<endl;
                // fgets(buffer,MSG_SIZE-1,stdin); // MSG_SIZE-1 'cause a null character is added
                cin >> buffer;
                
                
                // send message to anyone there...
                n = sendto(sock, buffer, sizeof(buffer)-1, 0, (const struct sockaddr *)&anybody, length);
                if(n < 0)
                    cout << "error" << endl;
            }
            else if(choice == 2){
                //  while((ch=getchar()) != '\n' && ch!=EOF);
                sprintf(ip, "128.206.19.%d",RTU2);
                cout << "ip is " << ip << endl;
                anybody.sin_addr.s_addr = inet_addr(ip);    // broadcast address (Lab)
                // bzero: to "clean up" the buffer. The messages aren't always the same length.
                bzero(buffer,MSG_SIZE);    // sets all values to zero. memset() could be used
                //cleanbuffer(&buffer);
                cout << "LED1 or LED2" <<endl;
                //fgets(buffer,MSG_SIZE-1,stdin); // MSG_SIZE-1 'cause a null character is added
                cin >> buffer;
                // send message to anyone there...
                n = sendto(sock, buffer, sizeof(buffer)-1, 0, (const struct sockaddr *)&anybody, length);
                if(n < 0)
                    cout << "error" << endl;
        }
        
        
        else{
            cout << "only 2 RTU connected!" << endl;
            choice = 0;
        }
        
        
        
    } while(choice != '!');
    
}
