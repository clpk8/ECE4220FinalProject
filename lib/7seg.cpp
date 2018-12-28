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
    
    digitalWrite(DP, HIGH);
    int a[4] = {0};
    int i;
    
    for(i = 0; decimal > 0; i++){
        a[i] = decimal % 2;
        decimal = decimal / 2;
    }
    
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
