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
#define BTN1  27 //BTN
#define BTN2  0
#define SW1   26
#define SW2   23

//event counter
volatile int eventCounter = 0;

void myInterrupt(void) {
    eventCounter++;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    // sets up the wiringPi library
    if (wiringPiSetup () < 0) {
        fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        return 1;
    }
    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if ( wiringPiISR (BTN1, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
        return 1;
    }
    
    while ( 1 ) {
        cout << "eventCounter"<<endl;
        eventCounter = 0;
        delay( 1000 ); // wait 1 second
    }
    
    
    return 0;
}
