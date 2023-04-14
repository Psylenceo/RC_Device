#include <Global_Variables.h>
#include <SPI.h>
#include <FileSystems/storage.h>
#include <FS.h>
#include <SPIFFS.h>
#include <SD.h>

/**********************************************************************
 *
 *                          Local Variable Space
 *
 ***********************************************************************/

File spiffs_root; // SPIFFS root directory
File sd_root; // SD card root directory

bool spiffs = false; // SPIFFS initialization status
bool sd_card = false; // SD card initialization status

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

/**********************************************************************
 *
 *                       Initialize Storages
 *
 ***********************************************************************/

bool initializeStorages() {
    // Initialize SPIFFS and set flag
    spiffs = SPIFFS.begin(true);

    // Set SD card initialization message
    SD_Card_Msgs[0] = "Initializing SD CARD";

    // Initialize SD card and check if it was successful
    if (!SD.begin(SD_CS)) {
        // Set SD card mount failure message
        SD_Card_Msgs[1] = "Card Mount Failed";

        // Check if debug port is connected, if so, send message, otherwise wait for port to open
        if (Debug_Port_Connected) {
            // Debug port is connected, do nothing
        } else {
            // Debug port is not connected, wait for port to open
            while (!Debug_Port_Connected) {
                Debug_Port_Active_Check();
            }
        }

        // Attempt to reinitialize SD card for up to 3 seconds
        for (int timeout = 0; timeout < 3; timeout++) {
            if (SD.begin(SD_CS)) {
                break;
            }
            delay(1000);
        }

        // Stop program since no SD card is available
        if (!SD.cardPresent()) {
            while (true);
        }
    }

    // Set SD card initialization flag
    sd_card = true;

    // Get card type and store it for debug messages
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_MMC) {
        SD_Card_Msgs[2] = "SD Card Type: MMC";
    } else if (cardType == CARD_SD) {
        SD_Card_Msgs[2] = "SD Card Type: SDSC";
    } else if (cardType == CARD_SDHC) {
        SD_Card_Msgs[2] = "SD Card Type: SDHC";
    } else {
        SD_Card_Msgs[2] = "SD Card Type: UNKNOWN";
    }

    // Get card size, used space, and free space and store them for debug messages
    size_t cardSize = SD.cardSize();
    size_t cardUsed = SD.usedBytes();
    SD_Card_Msgs[3] = "SD Card Size: " + humanReadableSize(cardSize);
    SD_Card_Msgs[4] = "SD Card Used: " + humanReadableSize(cardUsed);
    SD_Card_Msgs[5] = "SD Card Free: " + humanReadableSize(cardSize - cardUsed);

    // Return initialization success status
    return true;
}

/**********************************************************************
 *
 *                       Helper Functions
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
