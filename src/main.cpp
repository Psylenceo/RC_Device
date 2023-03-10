#include <Global_Variables.h>
#include <Non_Lib_AsyncWebServer/Non_Lib_AsyncWebServer.h>
#include <SD_Card/SD_Card.h>
#include <Lights/Lights.h>

/**********************************************************************
 *
 *
 *
 * *******************************************************************/
#define STATUS_LED_PIN 2

Pin Aux[1] =
{
  //simple on and off lights appear to work with all 0's for settings????  
  {1,Aux1,0,3,0,0,0,0,0,0,0,0,0}
};

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
  Port[0].InitializeRX();
  Port[1].InitializeRX();

  Lights[0].InitializeLED();
  Lights[1].InitializeLED();
  Lights[2].InitializeLED();
  Lights[3].InitializeLED();

  Aux[0].InitializeAux();

  ms_loop = millis();
}

int sample = 0;
int sample2 = 0;

void loop()
{
  /*if ((Port[0].On_Time < (sample - (sample*.05))) || (Port[0].On_Time > ((sample*.05) + sample))) 
  {
    sample = Port[0].On_Time;
    Channel_Readout(0);
  }

  if ((Port[1].On_Time < (sample2 - (sample2*.05))) || (Port[1].On_Time > ((sample2*.05) + sample2))) 
  {
    sample2 = Port[1].On_Time;
    Channel_Readout(1);
  }*/

  if(Active_Webpage == 11)
  {
    Update_RX_Graph_Webpage();
  }

  if((Port[0].On_Time < 1450) || (Port[0].On_Time > 1565))
  {
    Lights[0].duty_cycle = 50;
    Lights[0].Brightness();
    if(Port[0].On_Time > 1565)
    {
      Lights[0].duty_cycle = 50;
      Lights[0].Brightness();
    }
  }
  if((Port[0].On_Time > 1450) && (Port[0].On_Time < 1565))
  {
    Lights[0].duty_cycle = 0;
    Lights[0].Brightness();
    Lights[1].duty_cycle = 0;
    Lights[1].Brightness();
  }

  if(Port[0].On_Time < 1450)
  {
    Lights[1].duty_cycle = 100;
    Lights[1].Brightness();
  }
  

  if(Port[1].On_Time > 1600)
  {
    //Lights[0].duty_cycle = 50;
    Lights[2].Brightness();
  }
  if(Port[1].On_Time < 1450)
  {
    //Lights[0].duty_cycle = 0;
    Lights[3].Brightness();
  }

  //nothing goes below here other than the status and debug stuff
  digitalWrite(STATUS_LED_PIN,!digitalRead(STATUS_LED_PIN));
  if(!Debug_Port_Connected)Debug_Port_Active_Check();
}