#ifndef Global_Variables_H
#define Global_Variables_H

#include <Arduino.h>
#include <Debug/Debug.h>
#include <Arduino_JSON.h>
#include <RC_Reciever/RC_Reciever.h>

/**********************************************************************
 *
 *                      Global Defines
 *
 * *******************************************************************/
#define SD_CS 5

#define port1 39
#define port2 35
#define port3 33

#define Aux4 32
#define Aux5 14
#define Aux6 27
#define Aux7 25
#define Aux8 33

#define Out1 0
#define Out2 12
#define Out3 15
#define Out4 4

/**********************************************************************
 *              Declare all global variables here using
 *      extern xxxxxxxx <type of variable> <variable name>
 *      Then initialize it in the cpp file that modifies the variable
 * *******************************************************************/

/**********************************************************************
 *
 *                      Global pin interface variables
 *
 * *******************************************************************/
//pin#,Detected,Seq pos,Detected cnt, seq #, ontime, period
struct Pin{
    uint8_t Input_Pin;
    uint8_t Port_Number;
    volatile bool Detected;                 //has given port triggered its interrupt
    volatile uint8_t Sequence_Position;     //if the ports are triggered in sequence, which port is 1st, 2nd, etc
    volatile uint8_t Detected_Port_Count;   //how many active ports triggered their interrupt
    volatile uint8_t Measurement_Sequence;  //which state the port is in. Triggered, captured, or Reset
    volatile long On_Time;                  //current on time for he given port
    volatile long Period;                   //time between start of each sample

    /**********************************************************************
    * 
    * 
    * 
    * *******************************************************************/
    void Initialize()
    {
        pinMode(Input_Pin,INPUT_PULLDOWN);
        
        if(Input_Pin == port1)
        {
            Serial.println("Initializing....Input  Port 1 Interrupt");
            attachInterrupt(Input_Pin,&Port_1_Input,CHANGE);
        } else if(Input_Pin == port2)
        {
            Serial.println("Initializing....Input  Port 2 Interrupt");
            attachInterrupt(Input_Pin,&Port_2_Input,CHANGE);
        }  
    }
};

extern Pin Port[4];

/**********************************************************************
 *
 *                      Globals for Debug
 *
 * *******************************************************************/
// Bit that indicates if the USB uart comm port is connected and the unit has recieved
// any signal through the port
extern bool Debug_Port_Connected;
// capture the millis() at the start of setup and is used to report how long it took
// to setup the unit.
extern int ms_start;
// capture the millis() just before loop starts and then update at the end of the
// loop after the loop check time.
extern int ms_loop;
// The configured time between bebug readouts for how long the loop took.
extern int loop_check_time;
/**********************************************************************
 *
 *                      Globals for Webpages
 *
 * *******************************************************************/
extern volatile int Active_Webpage; // 0-index
                                    // 10-dashboard
                                    // 11-RX monitor
                                    // 12-sim
                                    // 13-imu
                                    // 14-GPS
// 20-model memory
// 30-hardware settings
// 40-functions
// 50-Developement
// 51-webpage upload

/**********************************************************************
 *
 *                      Globals for SD card
 *
 * *******************************************************************/
// bits that indicate if certain base files were detected during SD card file scan
extern bool SD_detection[4];
// size of the sd card
extern unsigned long cardSize;
// number of bytes used on teh SD Card
extern unsigned long cardused;
// Number of free space bytes on teh SD card
extern unsigned long cardFree;

#endif