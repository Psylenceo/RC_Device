#include <Global_Variables.h>
#include <RC_Reciever/RC_Reciever.h>

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

uint8_t Inputs = 0;                //how many incomming channels
uint8_t Detected_Port_Count = 0;   //how many active ports triggered their interrupt
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

  if(Port[2].Measurement_Sequence == 0 && digitalRead(Port[2].PinNumber))
  {
    Port[2].Measurement_Sequence = 1;
    //Port[2].Period = timerReadMicros(PWM_Input_Timer);
    if(Port[2].Sequence_Position == 0)
    {
      Detected_Port_Count++;
      Port[2].Sequence_Position = Detected_Port_Count;
      Port[2].Detected = 1;
    } else if(Port[2].Sequence_Position == 1)
    {
      timerRestart(PWM_Input_Timer);
    }
  }

  if(Port[2].Measurement_Sequence == 1 && !digitalRead(Port[2].PinNumber))
  {
    Port[2].Measurement_Sequence = 0;
    Port[2].On_Time = timerReadMicros(PWM_Input_Timer);
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
  RX_Channel_values["Channels"] = String(Detected_Port_Count);
  //RX_Channel_values["THROTTLEP"] = String((Port[0].On_Time - 1000) / 10);
  RX_Channel_values["THROTTLE"] = String(Port[0].On_Time);
  //RX_Channel_values["STEERINGP"] = String((Port[1].On_Time - 1000) / 10);
  RX_Channel_values["STEERING"] = String(Port[1].On_Time);
  /*if(Inputs > 2 && Port[3].Enable && Port[3].Initialized)
  {
    RX_Channel_values["CHAN3P"] = String((Port[3].On_Time - 1000) / 10);
    RX_Channel_values["CHAN3"] = String(Port[3].On_Time);
  }
  if(Inputs > 3 && Port[4].Enable && Port[4].Initialized)
  {
    RX_Channel_values["CHAN4P"] = String((Port[4].On_Time - 1000) / 10);
    RX_Channel_values["CHAN4"] = String(Port[4].On_Time);
  }
  if(Inputs > 4 && Port[5].Enable && Port[5].Initialized)
  {
    RX_Channel_values["CHAN5P"] = String((Port[5].On_Time - 1000) / 10);
    RX_Channel_values["CHAN5"] = String(Port[5].On_Time);
  }
  if(Inputs > 5 && Port[6].Enable && Port[6].Initialized)
  {
    RX_Channel_values["CHAN6P"] = String((Port[6].On_Time - 1000) / 10);
    RX_Channel_values["CHAN6"] = String(Port[6].On_Time);
  }
  if(Inputs > 6 && Port[7].Enable && Port[7].Initialized)
  {
    RX_Channel_values["CHAN7P"] = String((Port[7].On_Time - 1000) / 10);
    RX_Channel_values["CHAN7"] = String(Port[7].On_Time);
  }
  if(Inputs > 7 && Port[8].Enable && Port[8].Initialized)
  {
    RX_Channel_values["CHAN8P"] = String((Port[8].On_Time - 1000) / 10);
    RX_Channel_values["CHAN8"] = String(Port[8].On_Time);
  }*/
  String jsonString = JSON.stringify(RX_Channel_values);
  return jsonString; 
}