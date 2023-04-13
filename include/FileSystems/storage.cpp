#include <Global_Variables.h>
#include "SPI.h"
#include <FileSystems/Storage.h>
#include "FS.h"
#include "SPIFFS.h"
#include "SD.h"

/**********************************************************************
 *
 *                          Local Variable Space
 *
 ***********************************************************************/
File spiffs_root;
File sd_root;

bool *spiffs = 0;
bool *sd_card = 0;

unsigned long *spiffsSize = 0;
unsigned long *spiffsUsed = 0;
unsigned long *spiffsFree = 0;

unsigned long *cardSize = 0;
unsigned long *cardUsed = 0;
unsigned long *cardFree = 0;

String SD_Card_Msgs[8] = {
    "", // 0-initialization message
    "", // 1-card not mounted, card not found, card connected
    "", // 2-card type
    "", // 3-card size
    "", // 4-used space
    "", // 5-free space
    "", // 6-debug-directories and file listing
    ""  // 7-webpage-directories and file listing
};

/**********************************************************************
 *
 *                       Initialize Storages
 *
 ***********************************************************************/

bool