#include <Global_Variables.h>
#include <Non_Lib_AsyncWebServer/Non_Lib_AsyncWebServer.h>
#include <SD_Card/SD_Card.h>
#include <RC_Reciever/RC_Reciever.h>

/**********************************************************************
 *
 *
 *
 * *******************************************************************/
#define STATUS_LED_PIN 2

void setup()
{
  // store initial power on CPU time
  ms_start = millis();

  // configure pin 2 (status LED) as output and set low (off)
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  // Configure programming / debug UART 0 to 115200 Baud
  Serial.begin(115200);
  // Check to see if debug port is connected waits 60sec then runs code with no debug
  Debug_Init_Port_Check(); 

  // init File system
  // Initialize SD card, as it stores everything, especially the hardware config data.
  initSDCard(); 
  // Read the SD card file structure
  File_List();  


  Init_WiFi();               // Initialize Wifi
  Web_Server_Handle();       // Setup Async webserver requests
  Web_Server_Event_Handle(); // setup Async Webserver event handler
  Start_Server();            // Start the webserver

  Initialize_PWM_in_Timer();
  Initialize_port();


  ms_loop = millis();
}

int sample_min = 0;
int sample = 0;
int sample_max = 0;

void loop()
{
  if ((On_Time < (sample - (sample*.05))) || (On_Time > ((sample*.05) + sample))) 
  {
    sample = On_Time;
    Channel_Readout(sample);
  }


  //nothing goes below here other than the status and debug stuff
  digitalWrite(STATUS_LED_PIN,!digitalRead(STATUS_LED_PIN));
  if(!Debug_Port_Connected)Debug_Port_Active_Check();
}