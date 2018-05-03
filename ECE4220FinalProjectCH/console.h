#ifndef CONSOLE_H
#define CONSOLE_H

#include <QMainWindow>
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
#include <QCoreApplication>
//std::vector<int> ipID;

using namespace std;

typedef struct logData
{
    string TimeStamp;
    int RTUid;
    //buttons, True for on and open. False for closed and off
    bool Switch1Status;
    bool Switch2Status;
    bool Button1Status;
    bool Button2Status;
    int Voltage;
    int typeEventID;
}LogData;


namespace Ui {
class Console;
}

class Console : public QMainWindow
{
    Q_OBJECT

public:
    friend class sending;
    explicit Console(QWidget *parent = 0);
     LogData data;
     //static void *receiving();
     unsigned int length;
     int sock;
     int n;
     int port;
     char buffer[200];
     struct sockaddr_in anybody;
     struct sockaddr_in from;
    ~Console();

private slots:
    void on_pushButton_clicked();

private:


    Ui::Console *ui;
};


#endif // CONSOLE_H
