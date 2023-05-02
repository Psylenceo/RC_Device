#include <Arduino.h>
#include <FileSystems/storage.h>
#include <Non_Lib_AsyncWebServer/Non_Lib_AsyncWebServer.h>
#include <IO.h>

/**********************************************************************
 *
 *
 *
 * *******************************************************************/
#define STATUS_LED_PIN 2

#define Out1 0
#define Out2 12
#define Out3 15

#define Aux1 33
#define Aux2 34
#define Aux3 14
#define Aux4 27
#define Aux5 25
#define Aux6 32
#define Aux7 4

int ms_start = 0;
// capture the millis() just before loop starts and then update at the end of the
// loop after the loop check time.
int ms_loop = 0;
// The configured time between bebug readouts for how long the loop took.
int loop_check_time = 0;

Pin Lights[3] =
{
  //simple on and off lights appear to work with all 0's for settings????
  //first 3 are required, next 5 are inputs, last 5 are output settings
//{R,R   , name       ,0,X,X,X,X,X,status,CH,Hz,Bits,duty cycle}  
  {2,Out1,"Headlights",0,0,0,0,0,0,0,0,60,8,0},
  {2,Out2,"Tail-Lights",0,0,0,0,0,0,0,1,60,8,0},
  {2,Out3,"Reverse Lights",0,0,0,0,0,0,0,2,60,8,0}
};

Pin Aux[3] =
{
  //simple on and off lights appear to work with all 0's for settings????  
  {1,Aux1,"Aux 3",0,3,0,0,0,0,0,0,0,0,0},
  {2,Aux6,"Right-Turn",0,0,0,0,0,0,0,3,1,16,0},
  {2,Aux7,"Left-Turn",0,0,0,0,0,0,0,4,1,16,0}
};

bool reverse_trigger = 0;
bool reverse_trigger_seal = 0;

void setup() {
  // store initial power on CPU time
  ms_start = millis();

  // configure pin 2 (status LED) as output and set low (off)
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  // Configure programming / debug UART 0 to 115200 Baud
  Serial.begin(115200);
  // Check to see if debug port is connected waits 60sec then runs code with no debug
  while (!Serial.available()) {
    if (millis() > 5000) {
      break;
    }
  }
  if (Serial.available()) {
    Serial.read();
    Serial.println("\nDevice connected, sending status updates.");
  }

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

void loop() {
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

  if (Active_Webpage == 11) {
    Update_RX_Graph_Webpage();
  }

  if ((Port[0].On_Time < 1425) || (Port[0].On_Time > 1590)) {
    Lights[0].duty_cycle = 25;
    Lights[1].duty_cycle = 25;

    if (Port[0].On_Time > 1590) {
      reverse_trigger = 0;
      reverse_trigger_seal = 0;
    }
    if (Port[0].On_Time < 1450) {
      reverse_trigger = 1;

      Lights[0].duty_cycle = 25;
      Lights[1].duty_cycle = 100;

      if (reverse_trigger && reverse_trigger_seal) Lights[2].duty_cycle = 100;
    }
  }
  if ((Port[0].On_Time > 1425) && (Port[0].On_Time < 1590)) {
    Lights[0].duty_cycle = 0;
    Lights[1].duty_cycle = 0;
    Lights[2].duty_cycle = 0;
    if (reverse_trigger == 1) reverse_trigger_seal = 1;
  }

  if ((Port[1].On_Time > 1425) && (Port[1].On_Time < 1590)) {
    Aux[1].duty_cycle = 0;
    Aux[2].duty_cycle = 0;
  }
  if (Port[1].On_Time > 1600) //left
  {
    Aux[1].duty_cycle = 50;
    Aux[2].duty_cycle = 0;
  }
  if (Port[1].On_Time < 1425) //right
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
  digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
  //if(!Debug_Port_Connected)Debug_Port_Active_Check();
}