#ifndef STORAGE_H
#define STORAGE_H

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