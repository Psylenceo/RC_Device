#ifndef STORAGE_H
#define STORAGE_H

extern bool spiffs;
extern bool sd_card;

// size of the sd card
extern size_t cardSize;
// number of bytes used on teh SD Card
extern size_t cardused;
// Number of free space bytes on teh SD card
extern size_t cardFree;

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