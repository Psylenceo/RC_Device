#ifndef Global_Variables_H
#define Global_Variables_H

#include <Arduino.h>
#include <Debug/Debug.h>

/**********************************************************************
 * 
 *                      Global Defines
 * 
 * *******************************************************************/

/**********************************************************************
 *              Declare all global variables here using
 *      extern xxxxxxxx <type of variable> <variable name>
 *      Then initialize it in the cpp file that modifies the variable
 * *******************************************************************/


/**********************************************************************
 * 
 *                      Globals for Debug
 * 
 * *******************************************************************/
//Bit that indicates if the USB uart comm port is connected and the unit has recieved
//any signal through the port
extern bool Debug_Port_Connected;
//capture the millis() at the start of setup and is used to report how long it took 
//to setup the unit.
extern int ms_start;            
//capture the millis() just before loop starts and then update at the end of the 
//loop after the loop check time.
extern int ms_loop;             
//The configured time between bebug readouts for how long the loop took.
extern int loop_check_time;     
/**********************************************************************
 * 
 *                      Globals for Webpages
 * 
 * *******************************************************************/
extern volatile int Active_Webpage; //0-index
                                    //10-dashboard
                                        //11-RX monitor
                                        //12-sim
                                        //13-imu
                                        //14-GPS
                                    //20-model memory
                                    //30-hardware settings
                                    //40-functions
                                    //50-Developement
                                        //51-webpage upload

/**********************************************************************
 * 
 *                      Globals for SD card
 * 
 * *******************************************************************/
#define SD_CS 5

//bits that indicate if certain base files were detected during SD card file scan
extern bool SD_detection[4];
//size of the sd card
extern unsigned long cardSize;
//number of bytes used on teh SD Card
extern unsigned long cardused;
//Number of free space bytes on teh SD card
extern unsigned long cardFree;


#endif