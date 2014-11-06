Remote Control Decoding and Interrupts (Lab05)
==============================================

#Purpose

#Day 1 Activities

Below is a picture of how the MSP430 was attached to the IR receiver.  

PICTURE OF SETUP

First hooking up the IR receiver, pressing a button, and then recording the changes in CCS in the 0x200 - 0x0400 range.  Notice, this range was cleared before a button was pressed to that all changes could be clearly seen highlighted in yellow.  A screenshot of the RAM range can be seen below: 

RAM RANGE SCREENSHOTs


Screenshot of logic analyzer after button was pressed: 


LOGIC ANALYZER SCREENSHOT.  

**A Few Quick Questions**

ANNOTATED PICTURE: 

1. How long did it take the timer to roll over? 
65.535ms.  Just take the timer count and multiply it by 0xFFFF.  
2. How long does each timer count last for? 
1us.  The clock frequency is 8MHz, but the ID_3 is being used, which increases the period or length of a count by 8.  
3. Annotate the wave picture to show where lines 32-34 and 36-38 are being executed: 

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/2.%20Pictures/Annotate%20Waveform.PNG "func location")



##IR Data Packets: 
  
|                           | measured,from logic analyzer | Timer A   Counts (calculated) | Timer A   Counts (from CCS) |
|---------------------------|------------------------------|-------------------------------|-----------------------------|
|                           | seconds                      | counts                        | counts                      |
| Start,logic 0 half-pulse  | 0.009218                     | 9218                          | 9042                        |
| Start,logic 1 half-pulse  | 0.00453125                   | 4531                          | 4485                        |
| Data 1,logic 0 half-pulse | 0.00059375                   | 594                           | 583                         |
| Data 1,logic 1 half-pulse | 0.0016875                    | 1688                          | 1612                        |
| Data 0,logic 0 half-pulse | 0.00063                      | 630                           | 637                         |
| Data 0,logic 1 half-pulse | 0.000546875                  | 547                           | 490                         |
| Stop,logic 0 half-pulse   | 0.0005816                    | 582                           | 586                         |
| Stop logic 1 half-pulse   | 0.0396875                    | 39688                         |                             |

Note that the number 99.9999426697% refers to all of the values within 5 standard deviations of the mean.  Therefore, to find the range for which we will check to check what portion of the range we are on, 5 standard deviations on other side of the mean value will be accepted to recognize that portion of the wave.  This will capture 99.9999426697% of all intended portions of the wave which we are looking for at that moment.  These ranges are shown below in the table after an experiment was run to gather 10 samples of the waveforms described above.  


|                  | Start logic 0 half-pulse | Start logic 1   half-pulse | Data 1   logic 0 half-pulse | Data 1   logic 1 half-pulse | Data 0   logic 0 half-pulse | Data 0   logic 1 half-pulse | Stop logic 0   half-pulse | Stop   logic 1 half-pulse |
|------------------|--------------------------|----------------------------|-----------------------------|-----------------------------|-----------------------------|-----------------------------|---------------------------|---------------------------|
|                  | 9042                     | 4485                       | 640                         | 1612                        | 640                         | 490                         | 586                       | 39251                     |
|                  | 9037                     | 4490                       | 634                         | 1614                        | 634                         | 493                         | 587                       | 39250                     |
|                  | 9037                     | 4487                       | 631                         | 1612                        | 631                         | 488                         | 582                       | 39254                     |
|                  | 9041                     | 4489                       | 637                         | 1619                        | 637                         | 492                         | 583                       | 39256                     |
|                  | 9042                     | 4490                       | 634                         | 1617                        | 634                         | 493                         | 580                       | 39246                     |
|                  | 9037                     | 4486                       | 640                         | 1616                        | 640                         | 484                         | 590                       | 39249                     |
|                  | 9041                     | 4491                       | 633                         | 1614                        | 633                         | 486                         | 591                       | 39250                     |
|                  | 9042                     | 4487                       | 638                         | 1615                        | 638                         | 492                         | 589                       | 39251                     |
|                  | 9044                     | 4488                       | 640                         | 1612                        | 640                         | 493                         | 588                       | 39251                     |
|                  | 9039                     | 4488                       | 638                         | 1614                        | 638                         | 488                         | 587                       | 39252                     |
| std_dev        | 2.529822128              | 1.91195072                 | 3.274480451                 | 2.321398046                 | 3.274480451                 | 3.247221034                 | 3.591656999               | 2.708012802               |
| average        | 9040.2                   | 4488.1                     | 636.5                       | 1614.5                      | 636.5                       | 489.9                       | 586.3                     | 39251                     |
|                  |                          |                            |                             |                             |                             |                             |                           |                           |
| Lower Bound | 9028                     | 4479                       | 621                         | 1603                        | 621                         | 474                         | 569                       | 39238                     |
| Upper Bound   | 9052                     | 4497                       | 652                         | 1626                        | 652                         | 506                         | 604                       | 39264                     |


| Button | code (not including start and stop bits) |
|--------|------------------------------------------|
| 0      |                                          |
| 1      |                                          |
| 2      |                                          |
| 3      |                                          |
| Power  |                                          |
| VOL +  |                                          |
| VOL -  |                                          |
| CH +   |                                          |
| CH -   |                                          |


#Basic Functionality

#B Functionality

#A Functionality

#Documentation: 
NONE
