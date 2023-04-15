#ifndef SD_CARD_H
#define SD_CARD_H

/**********************************************************************
 * 
 *  
 * 
 * *******************************************************************/
bool initSDCard();
String listFiles();
void File_List();
String humanReadableSize(const size_t bytes);

#endif