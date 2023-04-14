#ifndef STORAGE_H
#define STORAGE_H

bool initializeStorages();

String humanReadableSize(const size_t bytes);

void File_List();

int printDirectory(File dir, int Dir_Level);

#endif // STORAGE_H