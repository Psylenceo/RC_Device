#include <Global_Variables.h>
//#include <RC_Reciever/RC_Reciever.h>
#include <IO.h>

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
hw_timer_t * PWM_Input_Timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

Pin Port[2] =
{
    {1,RX1,"Throttle",0,1,0,0,0,0,0,0,0,0,0},
    {1,RX2,"Steering",0,2,0,0,0,0,0,0,0,0,0}
};

JSONVar RX_Channel_values;

//uint8_t Inputs = 0;                //how many incomming channels
volatile uint8_t Detected_Port_Count = 0;   //how many active ports triggered their interrupt
long Period = 0;                   //time between start of each sample

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void IRAM_ATTR Port_1_Input()
{
  portENTER_CRITICAL_ISR(&timerMux);

  if(Port[0].Measurement_Sequence == 0 && digitalRead(Port[0].PinNumber))
  {
    Port[0].Measurement_Sequence = 1;
    Period = timerReadMicros(PWM_Input_Timer);
    if(Port[0].Sequence_Position == 0)
    {
      Detected_Port_Count++;
      Port[0].Sequence_Position = Detected_Port_Count;
      Port[0].Detected = 1;
    } else if(Port[0].Sequence_Position == 1)
    {
      timerRestart(PWM_Input_Timer);
    }
  }

  if(Port[0].Measurement_Sequence == 1 && !digitalRead(Port[0].PinNumber))
  {
    Port[0].Measurement_Sequence = 0;
    Port[0].On_Time = timerReadMicros(PWM_Input_Timer);
  }
  
  portEXIT_CRITICAL_ISR(&timerMux);
}

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void IRAM_ATTR Port_2_Input()
{
  portENTER_CRITICAL_ISR(&timerMux);

  if(Port[1].Measurement_Sequence == 0 && digitalRead(Port[1].PinNumber))
  {
    Port[1].Measurement_Sequence = 1;
    //Port[1].Period = timerReadMicros(PWM_Input_Timer);
    if(Port[1].Sequence_Position == 0)
    {
      Detected_Port_Count++;
      Port[1].Sequence_Position = Detected_Port_Count;
      Port[1].Detected = 1;
    } else if(Port[1].Sequence_Position == 1)
    {
      timerRestart(PWM_Input_Timer);
    }
  }

  if(Port[1].Measurement_Sequence == 1 && !digitalRead(Port[1].PinNumber))
  {
    Port[1].Measurement_Sequence = 0;
    Port[1].On_Time = timerReadMicros(PWM_Input_Timer);
  }
  
  portEXIT_CRITICAL_ISR(&timerMux);
}

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void IRAM_ATTR Port_3_Input()
{
  portENTER_CRITICAL_ISR(&timerMux);

  if(Aux[0].Measurement_Sequence == 0 && digitalRead(Aux[0].PinNumber))
  {
    Aux[0].Measurement_Sequence = 1;
    //Aux[0].Period = timerReadMicros(PWM_Input_Timer);
    if(Aux[0].Sequence_Position == 0)
    {
      Detected_Port_Count++;
      Aux[0].Sequence_Position = Detected_Port_Count;
      Aux[0].Detected = 1;
    } else if(Aux[0].Sequence_Position == 1)
    {
      timerRestart(PWM_Input_Timer);
    }
  }

  if(Aux[0].Measurement_Sequence == 1 && !digitalRead(Aux[0].PinNumber))
  {
    Aux[0].Measurement_Sequence = 0;
    Aux[0].On_Time = timerReadMicros(PWM_Input_Timer);
  }
  
  portEXIT_CRITICAL_ISR(&timerMux);
}

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void Initialize_PWM_in_Timer(void)
{
    Serial.println("Initializing....RC Reciver Input Timer.");
    PWM_Input_Timer = timerBegin(0,80,true);    
}

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void Channel_Readout(int Channel)
{
  Serial.println("Ch" + String(Port[Channel].Port_Number) + " time = " + String(Port[Channel].On_Time) + " ms");
}

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
String Update_RX_JSON()
{
  //portENTER_CRITICAL_ISR(&timerMux);
  RX_Channel_values["Channels"] = String(Detected_Port_Count);
  //uint8_t temp = Detected_Port_Count;
  //for(int i=0;temp;i++){
  char buffer[20];
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, Port[0].Name);
  RX_Channel_values["ch1"] = buffer;
  RX_Channel_values["ch1value"] = String(Port[0].On_Time);
  RX_Channel_values["ch1minRange"] = String(0);
  RX_Channel_values["ch1maxRange"] = String(0);
  RX_Channel_values["ch1minDeadZone"] = String(0);
  RX_Channel_values["ch1maxDeadZone"] = String(0);
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, Port[1].Name);
  RX_Channel_values["ch2"] = buffer;
  RX_Channel_values["ch2value"] = String(Port[1].On_Time);
  RX_Channel_values["ch2minRange"] = String(0);
  RX_Channel_values["ch2maxRange"] = String(0);
  RX_Channel_values["ch2minDeadZone"] = String(0);
  RX_Channel_values["ch2maxDeadZone"] = String(0);
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, Aux[0].Name);
  RX_Channel_values["ch3"] = buffer;
  RX_Channel_values["ch3value"] = String(Aux[0].On_Time);
  RX_Channel_values["ch3minRange"] = String(0);
  RX_Channel_values["ch3maxRange"] = String(0);
  RX_Channel_values["ch3minDeadZone"] = String(0);
  RX_Channel_values["ch3maxDeadZone"] = String(0);
  //}
  //portEXIT_CRITICAL_ISR(&timerMux);
  
  String jsonString = JSON.stringify(RX_Channel_values);
  return jsonString; 
}