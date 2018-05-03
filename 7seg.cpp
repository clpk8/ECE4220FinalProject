#include <iostream>
#include <wiringPi.h>
#include <unistd.h>

using namespace std;
#define A 5
#define B 6
#define C 25
#define D 2
#define DP 29
void sevenSeg(int decimal){
    //decimal to binary
    
    int a, b, c, d;
    a = num%2;
    num/=2;
    b = num%2;
    num/=2;
    c = num%2;
    num/=2;
    d = num%2;
    
    digitalWrite(A, a);
    digitalWrite(B, b);
    digitalWrite(C, c);
    digitalWrite(D, d);
    
    if(!ledEnabled){
        digitalWrite(DP, 1);
        ledEnabled = 1;
    }
}

int main()
{
    cout << "Hello, World!";
    
    if (wiringPiSetup () < 0) {
        // fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        cerr<< "Not able to setup wiringpi"<<endl;
        return -1;
    }
    pinMode(DP, OUTPUT);
    pinMode(A, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(B, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    
    
    //digitalWrite(DP, LOW);
    digitalWrite(A,LOW);
    digitalWrite(B,LOW);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    int num;
    
    while(1){
        cin >> num;
        sevenSeg(num);
        
    }
    
    return 0;
}
