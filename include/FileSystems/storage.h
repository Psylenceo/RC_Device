#ifndef STORAGE_H
#define STORAGE_H

#include <SPIFFS.h>
#include "FS.h"
#include "SD.h"

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
bool initializeStorage();
String listFiles();
void File_List();
String humanReadableSize(const size_t bytes);

#endif