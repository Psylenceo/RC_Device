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

#define RX1 39
#define RX2 35

#define Aux1 33
#define Aux2 34
#define Aux3 14
#define Aux4 27
#define Aux5 25
#define Aux6 32
#define Aux7 4

#define Out1 0
#define Out2 12
#define Out3 15


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

//pin use, pin #, init'd, Detected, Seq pos, seq #, ontime
struct Pin{
    volatile uint8_t pin_use;               //0 not used, 1 input, 2 LED output, 3 RC PWM OUT
    volatile uint8_t PinNumber;
    volatile bool Initialized;              //has given port been initialized
    
    /* Section for inputs*/
    volatile uint8_t Port_Number;
    volatile bool Detected;                 //has given port triggered its interrupt
    volatile uint8_t Sequence_Position;     //if the ports are triggered in sequence, which port is 1st, 2nd, etc
    volatile uint8_t Measurement_Sequence;  //which state the port is in. Triggered, captured, or Reset
    volatile long On_Time;                  //current on time for he given port

    /* Section for LED's*/
    volatile double LED_Status; // should return frequency, i think
    const int LED_Channel; // 0 - 16
    const int LED_Frequency;
    const int LED_Resolution; // how many bits
    volatile uint8_t duty_cycle;


    /**********************************************************************
    * 
    * 
    * 
    * *******************************************************************/
    void InitializeRX()
    {
        pinMode(PinNumber,INPUT_PULLDOWN);
        Serial.println("Initializing....Input Port " + String(Port_Number) + " Interrupt");
            
        if(Port_Number == 1)
        {
            attachInterrupt(PinNumber,&Port_1_Input,CHANGE);
        } else if(Port_Number == 2)
        {
            attachInterrupt(PinNumber,&Port_2_Input,CHANGE);
        } else if(Port_Number == 3)
        {
            attachInterrupt(PinNumber,&Port_3_Input,CHANGE);
        }
        
        Initialized = 1;  
    }

    /**********************************************************************
    * 
    * 
    * 
    * *******************************************************************/
    void InitializeLED()
    {
        Serial.println("Initializing....Headlights");
        pinMode(PinNumber, OUTPUT);
        LED_Status = ledcSetup(LED_Channel, LED_Frequency, LED_Resolution);
        ledcAttachPin(PinNumber, LED_Channel);

        Initialized = 1;
    }

    /**********************************************************************
    * 
    * 
    * 
    * *******************************************************************/
    void Brightness()
    {
        if (duty_cycle > 100)ledcWrite(LED_Channel, 255);
        if (duty_cycle >= 0 && duty_cycle <= 100)ledcWrite(LED_Channel, duty_cycle * 2.5);
        if (duty_cycle < 0)ledcWrite(LED_Channel, 0);
    }

    /**********************************************************************
    * 
    * 
    * 
    * *******************************************************************/
    void InitializeAux()
    {
        if(pin_use == 1)InitializeRX();
        if(pin_use == 2)InitializeLED();
    }
};

extern Pin Port[2];
extern Pin Lights[3];
extern Pin Aux[3];
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

extern volatile int lastTime_RX[2]; //last sample time, sample delay

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