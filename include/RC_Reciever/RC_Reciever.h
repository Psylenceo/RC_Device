#ifndef RC_Reciever_H
#define RC_Reciever_H

/**********************************************************************
 * 
 *                  variables
 * 
 * *******************************************************************/

/**********************************************************************
 * 
 *                  Interrupt functions
 * 
 * *******************************************************************/
void IRAM_ATTR Port_1_Input();
void IRAM_ATTR Port_2_Input();
void IRAM_ATTR Port_3_Input();

/**********************************************************************
 * 
 *                  functions
 * 
 * *******************************************************************/
void Initialize_PWM_in_Timer(void);
void Channel_Readout(int Channel);
String Update_RX_JSON();

#endif