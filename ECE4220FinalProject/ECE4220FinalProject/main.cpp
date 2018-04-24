//
//  main.cpp
//  ECE4220FinalProject
//
//  Created by linChunbin on 4/24/18.
//  Copyright © 2018 linChunbin. All rights reserved.
//

#include <iostream>
#include <wiringPi.h>

using namespace std;
#define LED1  8        // wiringPi number corresponding to GPIO2.
#define LED2  9 //yellow
#define LED3  7 //green
#define LED4  21 //blue
#define BTN1  27 //BTN
#define BTN2  0
#define SW1   26
#define SW2   23

//event counter
volatile int eventCounter = 0;

void myInterrupt(void) {
    //eventCounter++;
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);
    
    delay(500);
    
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH);
    
    delay(500);
}

int main(int argc, const char * argv[]) {
    // insert code here...

    
    cout << "Hello, World!\n";
    // sets up the wiringPi library
    if (wiringPiSetup () < 0) {
       // fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        cerr<< "Not able to setup wiringpi"<<endl;
        return 1;
    }
    pinMode(LED1, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(LED2, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(LED3, OUTPUT);    // Configure GPIO2, which is the one connected to the red LED.
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(SW1, INPUT);
    pinMode(SW2, INPUT);


    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if ( wiringPiISR (BTN1, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }
    if ( wiringPiISR (BTN2, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }
    if ( wiringPiISR (SW1, INT_EDGE_BOTH, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }
    if ( wiringPiISR (SW2, INT_EDGE_BOTH, &myInterrupt) < 0 ) {
        cerr<<"Not able to setup IRS"<<endl;
        return 1;
    }

    while ( 1 ) {
        pullUpDnControl(BTN1,PUD_DOWN);//first set the push button's register down for input
        pullUpDnControl(BTN2,PUD_DOWN);//first set the push button's register down for input
        cout << eventCounter<<endl;
        eventCounter = 0;
        delay( 1000 ); // wait 1 second
    }


    return 0;
}
