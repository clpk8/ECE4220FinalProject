#include <iostream>
#include <wiringPi.h>
#include <unistd.h>

using namespace std;
#define A 5
#define B 6
#define C 25
#define D 2
#define DP 4
int main()
{
    cout << "Hello, World!";
    
    if (wiringPiSetup () < 0) {
        // fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        cerr<< "Not able to setup wiringpi"<<endl;
        return -1;
    }
    
    pinMode(A, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(B, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(DP, OUTPUT);
    digitalWrite(DP, LOW);
    digitalWrite(A,LOW);
    digitalWrite(B,LOW);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    
    
    cout << "1" << endl;
    digitalWrite(DP, HIGH);

    
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,HIGH);
    
    sleep(5);
    cout << "2" << endl;

    digitalWrite(A,HIGH);
    digitalWrite(B,LOW);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    
    sleep(5);
    cout << "3" << endl;

    digitalWrite(A,LOW);
    digitalWrite(B,LOW);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    sleep(5);
    
    return 0;
}
