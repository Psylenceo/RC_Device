#ifndef Debug_H
#define Debug_H

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
void Debug_Init_Port_Check(int timeout = 60000);
bool Debug_Port_Active_Check();

void Terminal_Loop_Time();
void Debug_Setup_Complete_Begin_Loop();


#endif