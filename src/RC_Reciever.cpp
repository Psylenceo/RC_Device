#include <Global_Variables.h>
#include <RC_Reciever/RC_Reciever.h>

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
hw_timer_t * PWM_Input_Timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint8_t Measurement_Sequence = 0;   //which state the port is in. Triggered, captured, or Reset
uint8_t Input_Pin = port1;
volatile long Period = 0;           //time between start of each sample
volatile uint8_t Sequence_Position = 0;      //if the ports are triggered in sequence, which port is 1st, 2nd, etc
volatile uint8_t Detected_Port_Count = 0;   //how many active ports triggered their interrupt
volatile bool Detected = 0;                  //has given port triggered its interrupt
volatile long On_Time = 0;          //current on time for he given port

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void IRAM_ATTR Port_1_Input()
{
  portENTER_CRITICAL_ISR(&timerMux);

  if(Measurement_Sequence == 0 && digitalRead(Input_Pin))
  {
    Measurement_Sequence = 1;
    Period = timerReadMicros(PWM_Input_Timer);
    if(Sequence_Position == 0)
    {
      Detected_Port_Count++;
      Sequence_Position = Detected_Port_Count;
      Detected = 1;
    } else if(Sequence_Position == 1)
    {
      timerRestart(PWM_Input_Timer);
    }
  }

  if(Measurement_Sequence == 1 && !digitalRead(Input_Pin))
  {
    Measurement_Sequence = 0;
    On_Time = timerReadMicros(PWM_Input_Timer);
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
void Initialize_port(void)
{
    pinMode(Input_Pin,INPUT_PULLDOWN);
    Serial.println("Initializing....Input Port 1 Interrupt");
    attachInterrupt(Input_Pin,&Port_1_Input,CHANGE);   
}

/**********************************************************************
 * 
 * 
 * 
 * *******************************************************************/
void Channel_Readout(int Channel)
{
  Serial.println("Ch1 time = " + String(On_Time) + " ms");
}