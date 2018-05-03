#include "console.h"
#include "ui_console.h"


Console::Console(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
}

Console::~Console()
{
    delete ui;
}

void Console::on_pushButton_clicked()
{
    port = ui->PortNumber->text().toInt();
    cout << "port is " << port << endl;
    int boolval = 1;			// for a socket option
    anybody.sin_family = AF_INET;		// symbol constant for Internet domain
    anybody.sin_port = htons(port);				// port field
    anybody.sin_addr.s_addr = htonl(INADDR_ANY);

    sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
    if(sock < 0)
        cerr << "Error: socket" << endl;
        //error("Error: socket");

    int length = sizeof(anybody);
      bind(sock, (struct sockaddr *)&anybody, length);

      if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
       {
           cerr << "Error setting socket options." << endl;
           //printf("Error setting socket options.\n");
           exit(-1);
       }

      anybody.sin_addr.s_addr = inet_addr("128.206.19.255");	// broadcast address (Lab)


}
