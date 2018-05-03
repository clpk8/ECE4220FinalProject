#include "console.h"
#include <QApplication>

void *receiving( void * ptr);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Console w;
    w.show();


    pthread_t thread_rec;		// thread variable

    pthread_create(&thread_rec, NULL, receiving, (void*)&w);	// for receiving

    return a.exec();
}
void *receiving( void * ptr)
{
        Console* w;
        char *value;
        char delim[] = "|";

        w = (Console*) ptr;

        w->length = sizeof(struct sockaddr_in);		// size of structure

        while(1)
        {
            bzero(w->buffer,200);	// sets all values to zero. memset() could be used
            // receive message
            w->n = recvfrom(w->sock, w->buffer, 200, 0, (struct sockaddr *)&(w->from), &w->length);
            if(w->n < 0)
                cerr << "Error: recvfrom" << endl;


            value = strtok(w->buffer, delim);
            w->data.TimeStamp = value;

            value = strtok(NULL, delim);
            w->data.RTUid = atoi(value);

            value = strtok(NULL, delim);
            w->data.Switch1Status = atoi(value);

            value = strtok(NULL, delim);
            w->data.Switch2Status = atoi(value);

            value = strtok(NULL, delim);
            w->data.Button1Status = atoi(value);

            value = strtok(NULL, delim);
            w->data.Button2Status = atoi(value);

            value = strtok(NULL, delim);
            w->data.Voltage = atoi(value);


            value = strtok(NULL, delim);
            w->data.typeEventID = atoi(value);



            //ipID.push_back(w->data.typeEventID);


            cout << "This was received: " << w->buffer << endl;
        }

        pthread_exit(0);
   }
