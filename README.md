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
  
| Pulse                     | Duration (ms) | Timer A counts |
|---------------------------|---------------|----------------|
| Start logic 0 half-pulse  |               |                |
| Start logic 1 half-pulse  |               |                |
| Data 1 logic 0 half-pulse |               |                |
| Data 1 logic 1 half-pulse |               |                |
| Data 0 logic 0 half-pulse |               |                |
| Data 0 logic 1 half-pulse |               |                |
| Stop logic 0 half-pulse   |               |                |

Note that the number 99.9999426697% refers to all of the values within 5 standard deviations of the mean.  Therefore, to find the range for which we will check to check what portion of the range we are on, 5 standard deviations on other side of the mean value will be accepted to recognize that portion of the wave.  This will capture 99.9999426697% of all intended portions of the wave which we are looking for at that moment.  These ranges are shown below in the table after an experiment was run to gather 10 samples of the waveforms described above.  



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
