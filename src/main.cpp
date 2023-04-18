#include <Global_Variables.h>
#include <Non_Lib_AsyncWebServer/Non_Lib_AsyncWebServer.h>
#include <Lights/Lights.h>
#include <IO.h>

/**********************************************************************
 *
 *
 *
 * *******************************************************************/
#define STATUS_LED_PIN 2

Pin Aux[3] =
{
  //simple on and off lights appear to work with all 0's for settings????  
  {1,Aux1,"Aux 3",0,3,0,0,0,0,0,0,0,0,0},
  {2,Aux6,"Right-Turn",0,0,0,0,0,0,0,3,1,16,0},
  {2,Aux7,"Left-Turn",0,0,0,0,0,0,0,4,1,16,0}
};

bool reverse_trigger = 0;
bool reverse_trigger_seal = 0;

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
  // Initialize storage systems, as it stores everything, especially the hardware config data.
  initializeStorage(); 
  // Read the file structure of from the storage systems
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

  Aux[0].InitializeAux();
  Aux[1].InitializeAux();
  Aux[2].InitializeAux();

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

  if((Port[0].On_Time < 1425) || (Port[0].On_Time > 1590))
  {
    Lights[0].duty_cycle = 25;
    Lights[1].duty_cycle = 25;

    if(Port[0].On_Time > 1590)
    {
      reverse_trigger = 0;
      reverse_trigger_seal = 0;
    }
    if(Port[0].On_Time < 1450)
    {
      reverse_trigger = 1;

      Lights[0].duty_cycle = 25;
      Lights[1].duty_cycle = 100;

      if (reverse_trigger && reverse_trigger_seal) Lights[2].duty_cycle = 100;
    }
  }
  if((Port[0].On_Time > 1425) && (Port[0].On_Time < 1590))
  {
    Lights[0].duty_cycle = 0;
    Lights[1].duty_cycle = 0;
    Lights[2].duty_cycle = 0;
    if(reverse_trigger == 1) reverse_trigger_seal = 1;    
  }
  
  if((Port[1].On_Time > 1425) && (Port[1].On_Time < 1590))
  {
    Aux[1].duty_cycle = 0;
    Aux[2].duty_cycle = 0;
  }
  if(Port[1].On_Time > 1600) //left
  {
    Aux[1].duty_cycle = 50;
    Aux[2].duty_cycle = 0;
  }
  if(Port[1].On_Time < 1425) //right
  {
    Aux[1].duty_cycle = 0;    
    Aux[2].duty_cycle = 50;    
  }

  Lights[0].Brightness();
  Lights[1].Brightness();
  Lights[2].Brightness();
  Aux[1].Brightness();
  Aux[2].Brightness();

  //nothing goes below here other than the status and debug stuff
  digitalWrite(STATUS_LED_PIN,!digitalRead(STATUS_LED_PIN));
  if(!Debug_Port_Connected)Debug_Port_Active_Check();
}