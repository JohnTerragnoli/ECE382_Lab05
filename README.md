Remote Control Decoding and Interrupts (Lab05)
==============================================

#Purpose

To hook up an IR receiver to the MSP430 to control interaction between the MSP430 and a remote.  

#Day 1 Activities

Below is a picture of how the MSP430 was attached to the IR receiver.  
![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/2.%20Pictures/IR%20and%20MSP430%20Wireup.JPG "Wireup")


First hooking up the IR receiver, pressing a button, and then recording the changes in CCS in the 0x200 - 0x0400 range.  Notice, this range was cleared before a button was pressed to that all changes could be clearly seen highlighted in yellow.  A screenshot of the RAM range can be seen below: 


This is the RAM range after it was cleared.
![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/2.%20Pictures/Reset%20Memory.PNG "RAM Clear")

This is the RAM range after the program was hit, a button on the remote was pressed, and the program was paused.  

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/2.%20Pictures/0x0200-0x400%20changed.PNG "RAM Changed")

This is the variables tab after the pause button was hit: 

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/2.%20Pictures/TIME0_changed%20after%20button%20hit.PNG "Variables Changed")


Screenshot of logic analyzer after button was pressed: 

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/2.%20Pictures/General%20Logic%20Analyzer%20Screenshot.JPG "Logic Analyzer")

Notice, this is the basic packet of information which is sent from the remote to the IR receiver, which is relayed to the MSP430.  

These last screenshots were done just to get the timing down between running Code Composer Studio, the Logic Analyzer, and the remote. 


**A Few Quick Questions**

ANNOTATED PICTURE: 

1. How long did it take the timer to roll over? 
65.535ms.  Just take the timer count and multiply it by 0xFFFF.  
2. How long does each timer count last for? 
1us.  The clock frequency is 8MHz, but the ID_3 is being used, which increases the period or length of a count by 8.  
3. Annotate the wave picture to show where lines 32-34 and 36-38 are being executed: 

![alt tag](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/2.%20Pictures/Annotate%20Waveform.PNG "func location")

At first I did this on the oscilliscope, but then I found it was easier just to use the logic analyzer.  Screenshots of the oscilliscope can be produced upon request.  

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
| Stop logic 1 half-pulse   | 0.0396875                    | 39688                         | 39251                       |

Note that the number 99.9999426697% refers to all of the values within 5 standard deviations of the mean, as noted by this article on [Standard Deviation](http://en.wikipedia.org/wiki/Standard_deviation#Rules_for_normally_distributed_data).  Therefore, to find the range for which we will check to check what portion of the range we are on, 5 standard deviations on other side of the mean value will be accepted to recognize that portion of the wave.  This will capture 99.9999426697% of all intended portions of the wave which we are looking for at that moment.  These ranges are shown below in the table after an experiment was run to gather 10 samples of the waveforms described above.  



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

Note: An issue which came up was having decimals in the upper and lower bounds.  The idea was to take the ceiling of the decimal calculated for the lower bound and the floor of the decimal calculated for the upper bound.  This was done so to ensure that the values recognized by the program are defintely within 5 standard deviations.  If the decimals were kept, which first of all is impossible, or if the floor and ceiling was taken, then a value on the fringes might be accepted by the computer as a certain value when in fact it is not.  It is a small precaution, but it took minimal effort and was done anyway.  


| Button | code (not including start and stop bits) | In Hex   |
|--------|--------------------------------------------|----------|
| 0      | 0000 0010 1111 1101   0000 0000 1111 1111  | 02FD00FF |
| 1      | 0000   0010 1111 1101 1000 0000 0111 1111  | 02FD807F |
| 2      | 0000   0010 1111 1101 0100 0000 1011 1111  | 02FD40BF |
| 3      | 0000   0010 1111 1101 1100 0000 0011 1111  | 02FDC03F |
| Power  | 0000   0010 1111 1101 0100 1000 1011 0111  | 02FD48B7 |
| VOL +  | 0000   0010 1111 1101 0101 1000 1010 0111  | 02FD58A7 |
| VOL -  | 0000   0010 1111 1101 0111 1000 1000 0111  | 02FD7887 |
| CH +   | 0000   0010 1111 1101 1101 1000 0010 0111  | 02FDD827 |
| CH -   | 0000 0010 1111 1101   1111 1000 0000 0111  | 02FDF807 |


##Important Files: 
The Excel file with all of the forumulas needed to compute the above tabels can be seen [here](https://github.com/JohnTerragnoli/ECE382_Lab05/blob/master/3.%20Misc%20Files/Calculations.xlsx).

They can also be seen [here](https://drive.google.com/file/d/0Bymb7kjtbzuqdEJ0S1BzbEdnRW8/view?usp=sharing).  

I added two links because I wanted the Excel file to be on github, except I am not 100% certin if the file can be accessed by other people.  Just two safety nets :)

Also, here is the copy of the [test code](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/1.%20Code/test5.c) written by Dr. Coulston.  I did nothing in this section, not even modify the header, because I did not contribute to it in any way.  

#Day 2 Functionality
All of this was proven in basic functionality during class to Captain Trimble.  It probably would have been better to take baby steps but this was not done.  


#Basic Functionality

Design Process: 

this is just doodleing.  I will write down a more detailed version of the process later.  

1. To construct this code, I first started out by filling in the numbers specific to my remote in the header file.  This included the code for the buttons, the length of the different types of pulses, and calculating the min and max pulse lengths for each typte. 
2. Then, I desired to test this by writing a quick if statement in main, just to see if I could use the variables declared in the header properly.  The conditional statement included the value for the ZERO button.  I planned to have it change the value of a variable.  This did not work. I then remembered that I changed the name of the header file when I copied it in and the reference at the top of the C file did not match the new name.  It was good I included this before I moved on, as it would be confusing to deal with multiple errors at the same time.  
3. Then I wanted to construct the first interrupt.  I realized from the skeleton code that this method would be called everytime there was a pin change and the interrupt was not in the middle of operating. I also noticed that if the final value was 0, or option switch 0 was chosen, the a falling edge had occured. This is exactly what is needed to sense the falling edge, which is the first hint that a packet has been received.  I therefore wrote the code for decyphering the packet in this interrupt.  I then created if statements corresponding to the desired range of either 1's or 0's.  Within these statements I simply changed a test variable.  I did this to test if I could receive an IR at all and have the program know it.  After monitoring the value of the test variable in the "memory browser" of code composer studio.  It worked!
4. In this section I also added code which turned off the timer so that it was not counting while the signal is being received.  
5. Then I altered the body of the if statements to ensure that the packet is received. I did this by rotating the bits of the packet variable left and adding "OR"ing with either a 0 or a 1, to reflect the signal which just came in.  In this manner, all of the bits were received by the MSP430.  I tested this out by running the program and clicking on of the buttons on the remote.  I then checked the memory browser again to check what value was recorded.  Honestly, at this point I did not expect it to work, but it did, and that was fine by me.  
6. Then decided that the if statements in the main loop should be surrounded by another if statement, one which tells knows if a complete signal has been received.  I just used the pseudo-boolean variable available newIRPacket to determine if this occured.  I set the variable to false in the middle of this if statment and then set the variable to true after the second interrupt method occured. This is so that the computer does not try to decode the signal before it is competely done receiving it.  
7. Then I tried receiving a signal and it did not work.  I tried many different things, such as re-checking my code, making sure I reset the interrupt flag, and if my time variables were defined correctly.  Then I wondered if the range predetermined by the skeleton code of plus/minus 100 was wide enough.  I tried changing it to 150, and when I tested the code again, it worked! To do this I had to set the breakpoint to the end of the second interrupt.  After verifying that the signal was caught correctly, I tried adding the light business to the if statements in main.  
8. Adding the functionality for the lights was not difficult at all, since it was done in a previous example in class.  No problems were encountered when adding the toggling lights. 
9. When I ran the program, I noticed that the light would occilate between on and off if the buttons were held down.  At first this was worrisome, however, I realized this might be a good thing when I am trying to get the block to move around for A functionality. 

NOTE: Excel was used, along with the data from day 1 calculations to find out the average lengths of 0s and 1s being sent.  


The code for this basic functionality can be seen here: 

[C code](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/1.%20Code/basic_start5.c)

[header file](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/1.%20Code/basic_start5.h)

##Analysis/Conclusion: 

Overall, basic functionality was pretty easy but time consuming as it took me a while to understand the specific commands.  Once they were understood, I was able to work with them fairly easily.  The only difficult part of this lab was to accept an IR signal and decode it propertly.  Everything else was done before.  I would say that this was accomplished quite successfully and adapts to the conventions used quite well.  For the conventions given, 0s and 1s are determined by the length of the high pulse.  This made the process slightly more difficult, but now that it is done,this system can be reused for other projects.  


#A Functionality

The purpose of A functionality was to use the remote to play etch-a-sketch (Lab 04) on the LCD screen. 


**Design Process**
1. I started working on this by copying my .asm code from Lab 04 and ensuring they were all linked to the same header, so that the files could interact.
2. Then, I relized that it would probably be easier to just call the subroutines needed by the main file by mentioning them with "exter" statements at the top of the code.  
3. Then I added the intializing lines from Lab 4 which just set up the variables used to keep track of the color and location of the box.  This included setting up the LCD screen.  
4. Then, I pressed the build button to ensure there were no errors and I played the program.  A block was initialized on the middle of the LCD.  
5. Then, all there was to do was to change which buttons are being sensed in the main method.  I changed them to directions and ZERO button which was to be used for color.  Ideally this would work just like required functionality.  However, when I played the program, the block would only move one square and then become unresponsive.  After spending many hours on this issue I brought it into EI, but after spending many hours there none of my instructors could fix it.  They mentioned that I might want to try changing around the initializing statements like they did at the beginning, but this made no difference.  We also tried different locations for drawing the block, clearing the screen, and clearing the variables.  Nothing worked so the project was eventually abaondoned.  
6. What is most interesting about this issue is that if the right buttons sets as constants in the code, the block would move around the screen.  Also, during required functionality, the signal was properly decoded.  Unfortunately, when the code was mixed together, it did not work.  I also tried changing the width of the range of accepted signal lengths, but this did not change anything.  After spending about 12 hours on this problem I decided to quit so that I could study for the GR.  


Conclusion: 
Unfortunatetly, nothing was achieved.  A functionality was not attained.  For curiosity's sake though, I have included the code used in attempting functionality.  

[C Code](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/1.%20Code/A.c)

[Header File](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/1.%20Code/A.h) 

[Assembly Code](https://raw.githubusercontent.com/JohnTerragnoli/ECE382_Lab05/master/1.%20Code/drawing.asm)

#Documentation: 
C2C Hunter Her explained to me to use a falling edge on the logic analyzer and where to look in CCS for the count values. 
C2C Erik Thompson explained how to use the second interrupt to delay time and how to rotate the bits to record them properly in the first interrupt.  
