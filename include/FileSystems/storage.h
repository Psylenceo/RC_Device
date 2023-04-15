#ifndef STORAGE_H
#define STORAGE_H

#include <FileSystems/storage.h>
#include <FS.h>
#include <SPIFFS.h>
#include <SD.h>

/**********************************************************************
 *
 *                          Local Variable Space
 *
 ***********************************************************************/
// SPIFFS initialization status
bool spiffs = false; 
// SD card initialization status
bool sd_card = false; 

String SD_Card_Msgs[8] = {
    "", // 0 - initialization message
    "", // 1 - card not mounted, card not found, card connected
    "", // 2 - card type
    "", // 3 - card size
    "", // 4 - used space
    "", // 5 - free space
    "", // 6 - debug: directories and file listing
    ""  // 7 - webpage: directories and file listing
};


bool initializeStorages();
String humanReadableSize(const size_t bytes);
String File_List(File root);
int printDirectory(File dir, int Dir_Level);

#endif // STORAGE_H