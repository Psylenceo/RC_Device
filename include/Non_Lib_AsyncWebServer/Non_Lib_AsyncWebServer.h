#ifndef Non_Lib_AsyncWebServer_H
#define Non_Lib_AsyncWebServer_H

// names obvious?
extern uint8_t Active_Webpage;

/**********************************************************************
*
*
*
***********************************************************************/
void Init_WiFi();
void Web_Server_Handle();
void Web_Server_Event_Handle();
void Start_Server();
void Update_RX_Graph_Webpage();

#endif