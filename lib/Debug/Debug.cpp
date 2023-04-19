//#include <Global_Variables.h>
#include <Arduino.h>

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
bool Debug_Port_Connected;
int ms_start = 0;            //capture the millis() at the start of setup and is used to report how long it took to setup the unit.
int ms_loop = 0;             //capture the millis() just before loop starts and then update at the end of the loop after the loop check time.
int loop_check_time = 5000;     //The configured time between bebug readouts for how long the loop took.

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
void Debug_Init_Port_Check(int timeout)
{
    while(!Serial.available())  
    {
      if(millis() > timeout)
      {
        break;
      }
    }
    if (Debug_Port_Active_Check()) Serial.println("Esp32 connected. Starting Configurations");
}

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
bool Debug_Port_Active_Check()
{
    if(Serial.available())
    {
        Serial.read();
        Debug_Port_Connected = 1;
        Serial.println("\nDevice connected, sending status updates.");
        return Debug_Port_Connected;
    } else {
      Debug_Port_Connected = 0;
    }
    return Debug_Port_Connected;
}

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
void Debug_Setup_Complete_Begin_Loop()
{
  Serial.println("Everything is configured and started in: " + String(millis() - ms_start) + "ms");
}

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
void Terminal_Loop_Time()
{
  if((millis() - ms_loop) > loop_check_time)
  {   
   // uptime and loop time time reading
    {
      if(millis() > 1000 && millis() < 60000)
      {
        Serial.println("UT: " + String((millis() - ms_start)/1000) + "s/LT: " + String((millis() - ms_loop - loop_check_time)) + "ms");
      }
      if(millis() > 60000 && millis() < 3600000)
      {
        Serial.println("UT: " + String((millis() - ms_start)/60000) + "m/LT: " + String((millis() - ms_loop - loop_check_time)) + "ms");
      }
      if(millis() > 3600000 && millis() < 86400000)
      {
        Serial.println("UT: " + String((millis() - ms_start)/3600000) + "H/LT: " + String((millis() - ms_loop - loop_check_time)) + "ms");
      }

      ms_loop = millis();
    }
  }
}