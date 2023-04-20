#include <Global_Variables.h>
#include "SPI.h"
#include <SD_Card/SD_Card.h>
#include "FS.h"
#include "SD.h"


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
unsigned long cardSize = 0;
unsigned long cardused = 0;
unsigned long cardFree = 0;

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
 *                          local functions
 *
 ***********************************************************************/
int printDirectory(File dir, int Dir_Level);
String humanReadableSize(const size_t bytes);
void Webpage_SD_File_Directory(File dir, int Dir_Level);

/**********************************************************************
 *
 *                       Initialize SD Card
 *
 ***********************************************************************/
bool initSDCard()
{
    // sdSPI.begin(18,19,23,SD_CS);
    digitalWrite(5, HIGH);
    SD_Card_Msgs[0] = "Initializing....SD CARD";
    // SPI.begin();
    if (!SD.begin(SD_CS)) // Initilize SD card and check if Init was succesful. If not stop.
    {
        SD_Card_Msgs[1] = "Card Mount Failed";

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
            delay(1000);
            if (timeout == 3) return 0;
        }; // stop program since no SD available
    }

    SD_detection[0] = 1;
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) // verify mounted SD Card is connected
    {
        SD_Card_Msgs[1] = "No SD card attached";
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

    SD_Card_Msgs[1] = "SD Card Detected!";

    // Store what kind of SD card is mounted for debug messages
    SD_Card_Msgs[2] = "SD Card Type: ";
    if (cardType == CARD_MMC)
    {
        SD_Card_Msgs[2] += "MMC";
    }
    else if (cardType == CARD_SD)
    {
        SD_Card_Msgs[2] += "SDSC";
    }
    else if (cardType == CARD_SDHC)
    {
        SD_Card_Msgs[2] += "SDHC";
    }
    else
    {
        SD_Card_Msgs[2] += "UNKNOWN";
    }

    // calculate SD card size, used space, and free space then store each of them along with a message for debug output
    cardSize = SD.cardSize() / 1024;
    SD_Card_Msgs[3] = "SD Card Size: " + String((cardSize / 1024)) + " MB\n";

    cardused = ((SD.usedBytes() * 2.666666) + 0.9) / 1024;
    SD_Card_Msgs[4] = "SD Card used bytes: " + String(cardused) + " KB\n";

    cardFree = cardSize - cardused;
    SD_Card_Msgs[5] = "SD Card Free bytes: " + String((cardFree / 1024)) + " MB\n";

    return 0;
}

/**********************************************************************
 *
 *
 *
 ***********************************************************************/
void File_List()
{
  root = SD.open("/");
  SD_Card_Msgs[7] = "<table>\n<tr>\n<th align='left'>Name</th>\n<th align='left'>Size</th>\n<th></th>\n<th></th>\n</tr>\n";
  printDirectory(root, 0);
  // If(!SD.exists("listfiles"))
  root.close();
  SD_Card_Msgs[7] += "</table>";
  Serial.print(SD_Card_Msgs[6]);
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

    SD_Card_Msgs[7] += "<tr align='left'>\n\t<td>";
    for (uint8_t i = 0; i < Dir_Level; i++)
    {
      //Serial.print('\t');
       SD_Card_Msgs[6] += '\t';
       SD_Card_Msgs[7] += ".../ ";
    }

    //Serial.print(entry.name());
    if(SD_Card_Msgs[6] == "") {
        SD_Card_Msgs[6] = entry.name();
    } else {
        SD_Card_Msgs[6] += entry.name();
    }
    SD_Card_Msgs[7] += String(entry.name());

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
      SD_Card_Msgs[6] += "/\n";
      SD_Card_Msgs[7] += "/&emsp;</td>\n<td></td>\n<td>";
      if(String(entry.name()) != "System Volume Information") {
        SD_Card_Msgs[7] += "<button onclick=\"downloadDeleteButton(\'"+ String(entry.path()) + "\', \'delete\')\">Delete</button></tr>";
      }
      //Serial.println(entry.path());
      printDirectory(entry, Dir_Level + 1);
    }
    else
    {
      // Files have sizes, directories do not
      //Serial.print("\t\t");
      SD_Card_Msgs[6] += "\t\t" + humanReadableSize(entry.size()) + "\n";
      SD_Card_Msgs[7] += "&emsp; </td>\n<td>\n" + humanReadableSize(entry.size()) + "</td>\n";
      if(Dir_Level != 0)
      {
        SD_Card_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'download\')\">Download</button>\n";
        if((String(entry.name()) != "WPSettings.dat") && (String(entry.name()) != "IndexerVolumeGuid")) {
            SD_Card_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'delete\')\">Delete</button>\n</tr>\n";
        }
      } else {
        SD_Card_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'download\')\">Download</button>\n";
        if((String(entry.name()) != "WPSettings.dat") && (String(entry.name()) != "IndexerVolumeGuid")) {
            SD_Card_Msgs[7] += "<td><button onclick=\"downloadDeleteButton(\'" + String(entry.path()) + "\', \'delete\')\">Delete</button>\n</tr>\n";
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
    return SD_Card_Msgs[7];
}

/**********************************************************************
 *
 *
 *
 ***********************************************************************/
// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String humanReadableSize(const size_t bytes)
{
  if (bytes < 1024)
    return String(bytes) + " B";
  else if (bytes < (1024 * 1024))
    return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024))
    return String(bytes / 1024.0 / 1024.0) + " MB";
  else
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}