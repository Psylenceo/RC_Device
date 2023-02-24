#include <Global_Variables.h>
#include <RC_Reciever/RC_Reciever.h>

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
hw_timer_t * PWM_Input_Timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

Pin Port[4] =
{
    {0,0,0,0,0,0,0,0},
    {port1,1,0,0,0,0,0,0},
    {port2,2,0,0,0,0,0,0},
    {port3,3,0,0,0,0,0,0}
};

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void IRAM_ATTR Port_1_Input()
{
  portENTER_CRITICAL_ISR(&timerMux);

  if(Port[1].Measurement_Sequence == 0 && digitalRead(Port[1].Input_Pin))
  {
    Port[1].Measurement_Sequence = 1;
    Port[1].Period = timerReadMicros(PWM_Input_Timer);
    if(Port[1].Sequence_Position == 0)
    {
      Port[1].Detected_Port_Count++;
      Port[1].Sequence_Position = Port[1].Detected_Port_Count;
      Port[1].Detected = 1;
    } else if(Port[1].Sequence_Position == 1)
    {
      timerRestart(PWM_Input_Timer);
    }
  }

  if(Port[1].Measurement_Sequence == 1 && !digitalRead(Port[1].Input_Pin))
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
void IRAM_ATTR Port_2_Input()
{
  portENTER_CRITICAL_ISR(&timerMux);

  if(Port[2].Measurement_Sequence == 0 && digitalRead(Port[2].Input_Pin))
  {
    Port[2].Measurement_Sequence = 1;
    Port[2].Period = timerReadMicros(PWM_Input_Timer);
    if(Port[2].Sequence_Position == 0)
    {
      Port[0].Detected_Port_Count++;
      Port[2].Sequence_Position = Port[0].Detected_Port_Count;
      Port[2].Detected = 1;
    } else if(Port[2].Sequence_Position == 1)
    {
      timerRestart(PWM_Input_Timer);
    }
  }

  if(Port[2].Measurement_Sequence == 1 && !digitalRead(Port[2].Input_Pin))
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