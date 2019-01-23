# ECE 4220 - Real-time Embedded Computing Final Project
Supervisory Control And Data Acquisition System (SCADA)

## Video URL
https://www.youtube.com/watch?v=GnmcNRo-Du8&t=7s

## Intro

SCADA system that simulate the substation. The system consists of N RTUs(represented by raspberrypi with accessory boards),
a console operator that reside in a local PC and a Historian database app.

RTUs provide periodic real-time updates of their corresponding sub-system status via LAN. RTUs can also take in commands
comeing from the console operator. The historian app is in charge of kepping the log of tne entire system.

server and client model was used so the system will be able to host as many RTU as possible.

## System Overview
![alt text](https://raw.githubusercontent.com/clpk8/ECE4220FinalProject/master/SystemOverview.png)

## Topic covered
* multi-taksing/threading
* kernel modules
* periodic and real-time tasks management
* task communication, cooperation and synchronization
* concurrency control (mutex and semaphore)
* network communication
* client/server model
* interrupts
