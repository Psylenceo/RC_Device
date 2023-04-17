#include <Global_Variables.h>
#include "SPI.h"

/**********************************************************************
 *
 *                          Local Variable Space
 *
 ***********************************************************************/
File root;
File config;

bool SD_detection[4] = {
    0, //SD card present
    0, //index.html detected
    0, //index.css detected
    0 //index.js detected
};

bool spiffs = false;
bool sd_card = false;

size_t cardSize = 0;
size_t cardused = 0;
size_t cardFree = 0;

String storage_Msgs[8] = {
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
 *                          local functions
 *
 ***********************************************************************/
int printDirectory(File dir, int Dir_Level);
void Webpage_SD_File_Directory(File dir, int Dir_Level);

/**********************************************************************
 *
 *                       Initialize SD Card
 *
 ***********************************************************************/
bool initializeStorage()
{
    // Initialize SPIFFS and set flag
    spiffs = SPIFFS.begin(true);
    
    // sdSPI.begin(18,19,23,SD_CS);
    digitalWrite(5, HIGH);
    storage_Msgs[0] = "Initializing....SD CARD";
    // SPI.begin();
    if (!SD.begin(SD_CS)) // Initilize SD card and check if Init was succesful. If not stop.
    {
        storage_Msgs[1] = "Card Mount Failed";

        if (Debug_Port_Connected) // if debug port is open send message then stop.
        {
        }
        else
        { // if debug port not connected stop and wait for port open
            while (!Debug_Port_Connected)
            {
                Debug_Port_Active_Check(); // check if debug port is active, if it is loop will exit.
            }
        }

        for (int timeout=0;;timeout++)
        {
            if (SD.begin(SD_CS))
                break;
            delay(150);
            if (timeout == 3){
              SD.end();
              return 0;
            }
        }; // stop program since no SD available
    }

    SD_detection[0] = 1;
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) // verify mounted SD Card is connected
    {
        storage_Msgs[1] = "No SD card attached";
        if (Debug_Port_Connected) // if debug port is open send message then stop.
        {
        }
        else
        { // if debug port not connected stop and wait for port open
            while (!Debug_Port_Connected)
            {
                Debug_Port_Active_Check(); // check if debug port is active, if it is loop will exit.
            }
        }
        while (1)
            ; // stop program since no SD available
    }

    storage_Msgs[1] = "SD Card Detected!";

    // Store what kind of SD card is mounted for debug messages
    storage_Msgs[2] = "SD Card Type: ";
    if (cardType == CARD_MMC)
    {
        storage_Msgs[2] += "MMC";
    }
    else if (cardType == CARD_SD)
    {
        storage_Msgs[2] += "SDSC";
    }
    else if (cardType == CARD_SDHC)
    {
        storage_Msgs[2] += "SDHC";
    }
    else
    {
        storage_Msgs[2] += "UNKNOWN";
    }

    // calculate SD card size, used space, and free space then store each of them along with a message for debug output
    if(spiffs)cardSize = SPIFFS.totalBytes();
    if(sd_card)cardSize = SD.cardSize();
    storage_Msgs[3] = "SD Card Size: " + humanReadableSize(cardSize);
    if(spiffs)cardused = SPIFFS.usedBytes();
    if(sd_card)cardused = SD.usedBytes();
    storage_Msgs[4] = "SD Card used bytes: " + humanReadableSize(cardused);
    cardFree = cardSize - cardused;
    storage_Msgs[5] = "SD Card Free bytes: " + humanReadableSize(cardFree);

    return 0;
}

/**********************************************************************
 *
 *
 *
 ***********************************************************************/
void File_List()
{
  if(spiffs) root = SPIFFS.open("/");
  if(sd_card) root = SD.open("/");
  storage_Msgs[7] = "<table>\n<tr>\n<th align='left'>Name</th>\n<th align='left'>Size</th>\n<th></th>\n<th></th>\n</tr>\n";
  printDirectory(root, 0);
  // If(!SD.exists("listfiles"))
  root.close();
  storage_Msgs[7] += "</table>";
  Serial.print(storage_Msgs[6]);
}

/**********************************************************************
 *
 *
 *
 ***********************************************************************/
int printDirectory(File dir, int Dir_Level)
{
  while (true)
  {

    File entry = dir.openNextFile();
    if (!entry)
    {
      // No more files
      // Serial.println("**nomorefiles**");
      break;
    }

    storage_Msgs[7] += "<tr align='left'>\n\t<td>";
    for (uint8_t i = 0; i < Dir_Level; i++)
    {
      //Serial.print('\t');
       storage_Msgs[6] += '\t';
       storage_Msgs[7] += ".../ ";
    }

    //Serial.print(entry.name());
    if(storage_Msgs[6] == "") {
        storage_Msgs[6] = entry.name();
    } else {
        storage_Msgs[6] += entry.name();
    }
    storage_Msgs[7] += String(entry.name());

    if(String(entry.name()) == "index.html"){
        SD_detection[1] = 1;
        Serial.println("Index.html found!");
    }

    if(String(entry.name()) == "index.css"){
        SD_detection[2] = 1;
        Serial.println("Index.css found!");
    }

    if(String(entry.name()) == "index.js"){
       SD_detection[3] = 1;
       Serial.println("Index.js found!");
    }

    if (entry.isDirectory())
    {
      //Serial.println("/");
      storage_Msgs[6] += "/\n";
      storage_Msgs[7] += "/&emsp;</td>\n<td></td>\n<td>";
      if(String(entry.name()) != "System Volume Information") {
        storage_Msgs[7] += "<button onclick=\"downloadDeleteButton(\'"+ String(entry.path()) + "\', \'delete\')\">Delete</button></tr>";
      }
      //Serial.println(entry.path());
      printDirectory(entry, Dir_Level + 1);
    }
    else
    {
      // Files have sizes, directories do not
      //Serial.print("\t\t");
      storage_Msgs[6] += "\t\t" + humanReadableSize(entry.size()) + "\n";
      storage_Msgs[7] += "&emsp; </td>\n<td>\n" + humanReadableSize(entry.size()) + "</td>\n";
      if(Dir_Level != 0)
      {
        storage_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'download\')\">Download</button>\n";
        if((String(entry.name()) != "WPSettings.dat") && (String(entry.name()) != "IndexerVolumeGuid")) {
            storage_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'delete\')\">Delete</button>\n</tr>\n";
        }
      } else {
        storage_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'download\')\">Download</button>\n";
        if((String(entry.name()) != "WPSettings.dat") && (String(entry.name()) != "IndexerVolumeGuid")) {
            storage_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'delete\')\">Delete</button>\n</tr>\n";
        }
      }
      //Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
  return 0;
}

/**********************************************************************
 *
 *
 *
 ***********************************************************************/
// list all of the files, if ishtml=true, return html rather than simple text
String listFiles()
{
    return storage_Msgs[7];
}

/**********************************************************************
 *
 *
 *
 ***********************************************************************/
String humanReadableSize(const size_t bytes) {
  char buffer[10];
  if (bytes < 1024) {
    snprintf(buffer, sizeof(buffer), "%u B", bytes);
  } else if (bytes < (1024 * 1024)) {
    snprintf(buffer, sizeof(buffer), "%.1f KB", bytes / 1024.0);
  } else if (bytes < (1024 * 1024 * 1024)) {
    snprintf(buffer, sizeof(buffer), "%.1f MB", bytes / 1024.0 / 1024.0);
  } else {
    snprintf(buffer, sizeof(buffer), "%.1f GB", bytes / 1024.0 / 1024.0 / 1024.0);
  }
  return String(buffer);
}