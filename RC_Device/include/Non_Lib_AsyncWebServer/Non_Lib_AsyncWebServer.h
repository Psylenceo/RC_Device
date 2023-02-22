#ifndef Non_Lib_AsyncWebServer_H
#define Non_Lib_AsyncWebServer_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


/**********************************************************************
*
*
*
***********************************************************************/
void Init_WiFi();
void Web_Server_Handle();
void Web_Server_Event_Handle();
void Start_Server();

#endif