#include <Global_Variables.h>
#include <Non_Lib_AsyncWebServer/Non_Lib_AsyncWebServer.h>
#include <Non_Lib_AsyncWebServer/index_html.h>
#include <Non_Lib_AsyncWebServer/index_css.h>
#include <personal_Wifi_Credential.h>
#include <AsyncElegantOTA.h>
#include <SD_Card/SD_Card.h>
#include "SD.h"
#include "FS.h"

/**********************************************************************
 *
 *                       Local Variable Space
 *
 ***********************************************************************/
// Variable used to hold the ip address to be used for webpage home and debug stuff
IPAddress IP;

// SSID for Access point broadcasting, also optional password protection
const char *APssid = "RC-Device";
const char *APpassword = "esp";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// update after remembering
char *char_conv = (char *)index_html;

// names obvious?
volatile int Active_Webpage = 0;

// bit changes state when SD card files are uploaded or deleted and when system starts
bool checkFileList = 0;

// developement debug bit, will most likely get rid of at some point
bool HTML_dev = 1;

volatile int lastTime_RX[2] = {0,150}; //last sample time, sample delay

/**********************************************************************
 *
 *                       Local functions
 *
 ***********************************************************************/
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

/**********************************************************************
 *
 *                       Initialize WiFi
 *
 ***********************************************************************/
void Init_WiFi()
{
    Serial.println("Initializing....");

    // Station mode status variable
    int connection_ms = 0;             // sample milis() before entering connetion loop for polling
    int connection_ms_timeout = 20000; // max time allowed before switching to AP mode
    bool no_connected_network = 0;     // Final connection status before exiting connection loop

    Serial.print("Webserver scanning for WiFi networks.");

    int Local_WiFi = WiFi.scanNetworks(); // scan for any near by wifi AP's, storing the number found

    WiFi.disconnect(); // make sure wifi is off before switching modes

    // check if scan found any WiFi APs, if not create an AP
    if (Local_WiFi == 0)
    {
        Serial.println("\n  No Wifi Networks found.");
        Serial.println("\n  Setting AP (Access Point)…");
        // set unit to AP mode for access to webserver
        WiFi.mode(WIFI_AP);
        // Add the password parameter, if you want the AP (Access Point) to be locked
        WiFi.softAP(APssid);
        Serial.print("ESP32 IP as soft AP: ");
        // store and display AP's IP address
        IP = WiFi.softAPIP();
        Serial.println(IP);
    }
    else
    {
        Serial.print("\n    Scan found " + String(Local_WiFi) + " WiFi Networks.");
        Serial.print("\n    Checking for Registered WiFi Networks");
        int8_t searchIndex = -1;
        size_t stringSize = sizeof(WiFissid) / sizeof(WiFissid[0]);
        // go through detected wifi AP's, if a saved AP is found connect to it, if not begin AP mode
        for (size_t i = 0; i < Local_WiFi; i++)
        {
            const char *searchString = WiFi.SSID(i).c_str();
            for (size_t j = 0; j < stringSize; j++) {
                if (strstr(WiFissid[j], searchString) != NULL) {
                searchIndex = j;
                break;
                }
            }
            if (searchIndex != -1)
            {
                Serial.print("\n    Registered WiFi Network found!");
                Serial.print("\n    Connecting");
                WiFi.mode(WIFI_STA);
                WiFi.begin(WiFissid[searchIndex], WiFipassword[searchIndex]);
                connection_ms = millis();
                while (WiFi.status() != WL_CONNECTED)
                {
                    Serial.print('.');
                    if ((millis() - connection_ms) > connection_ms_timeout)
                    {
                        no_connected_network = 1;
                        Serial.println("\nNo Wifi Network connection.");
                        break;
                    }
                    delay(1000);
                }
                if (!no_connected_network)
                {
                    Serial.println(String(millis() - connection_ms) + " ms");
                    Serial.print("ESP32 IP on the WiFi network: ");
                    IP = WiFi.localIP();
                    Serial.println(WiFi.localIP());
                    break;
                }
                if (no_connected_network)
                {
                    Serial.println("\n  Setting AP (Access Point)…");
                    WiFi.disconnect();
                    WiFi.mode(WIFI_AP);
                    // Remove the password parameter, if you want the AP (Access Point) to be open
                    WiFi.softAP(APssid);
                    Serial.print("ESP32 IP as soft AP: ");
                    IP = WiFi.softAPIP();
                    Serial.println(IP);
                    break;
                }
            }
        }
    }
}

/**********************************************************************
 *
 *                      Webpage pre-processing,
 * *            replaces stuff between % % with new data on load
 *
 ***********************************************************************/
String processor(const String &var)
{
    Serial.println(var);

    if(var == "%"){
         String s ="%";
         return s;
    }
    if (var == "SIDEBAR")
    {
        String sidebar = "<div><li><a href= http://" + IP.toString() + ">Home</a></li> - <li><a href=\" /SafeMode\">SafeMode</a></li></div>\n";
        sidebar += "<button class = \"Sidebar\", id = \"Webpage_Upload\" onclick=\"webpageRequest(this)\"><u>Webpage Upload</u></button>\n";
        sidebar += "<button class = \"Sidebar\", id = \"HardwareConfig\" onclick=\"webpageRequest(this)\"><u>Hardware Configuration</u></button>\n";
        sidebar += "<li><button id=\"recieverBtn\" onclick=\"recieverPage()\" style=\"background: none; border: none; color: black; cursor: pointer;\">Graph</button></li>";

        return sidebar;
    }

    return var;
}

/**********************************************************************
 *
 *              Configure different webserver page calls
 *
 ***********************************************************************/
void Web_Server_Handle()
{
    // debug message
    Serial.println("Initializing....Webserver connection handling.");

    // when client is connected, if JavaScript file or CSS file breaks webpage
    // functionality, the user can call safe mode webpage which will load from
    // a hard coded webpage in webpages.h and allow for a barebones webpage to be
    // loaded. Including a file upload page to update the broken webpage file
    // in the file system.
    server.on("/SafeMode", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Active_Webpage = 0;
        Serial.println("Senidng Index webpage. Webpage #: " + String(Active_Webpage));
        request->send_P(200, "text/html", index_html, sizeof(index_html), processor); 
    });

    // the intial connection will come across on /, which will call this routine
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        //local tracking of which webpage is open
        Active_Webpage = 0;
        //debug message indicating that this routine was called
        Serial.println("Senidng Index webpage. Webpage #: " + String(Active_Webpage));
        //logic to verify the needed full featured webpage files are on the SD_card
        //if not then run the hardcoded webpage
        if(0 && SD_detection[0] && SD_detection[1]  && SD_detection[2] && SD_detection[3] && HTML_dev) {
            //debug message
            Serial.println("Conditions met for SD webserver");
            //have the web server and the client load the JS and CSS file static
            server.serveStatic("/index.css", SD, "/index.css").setCacheControl("max-age=86400");
            server.serveStatic("/index.js", SD, "/index.js");
            server.serveStatic("/file_system_mgmt.js", SD, "/file_system_mgmt.js");
            server.serveStatic("/receiver.js", SD, "/receiver.js");
            //Finally send the actual webpage on the SD-card after going through
            //processor to dynamically replace elements of the webpage
            request->send(SD, "/index.html", String(), false,processor);
        } else {
            //Send the barebones hardcoded webpage to the client connection
            //processor to dynamically replace elements of the webpage
            request->send_P(200, "text/html", index_html, sizeof(index_html), processor);
        } 
    });

    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String inputMessage1;
        const char* PARAM_INPUT_1 = "timeDate";
        if (request->hasParam(PARAM_INPUT_1)) {
            inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
        } else {
            inputMessage1 = "time and date data not recieved";
        }
        Serial.print(inputMessage1);
        request->send(200, "text/plain", "OK"); 
    });

    server.on("/hardwareStatus", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("Index loaded, request for status recieved.");
        events.send(status, "hardware_status", millis());
        request->send(200);
        if (SD_detection[0]) // event send - SD-card
        {
             events.send("SD_Card Detected", "SD_cardStatus", millis());
        }
        else
        {
            events.send("SD_Card Not Detected", "SD_cardStatus", millis());
        }
        // event send - Reciever
        // event send - IMU
        // event send - GPS
        // event send - temp
        // event send - rpm
        // event send - battery
    });

    server.on("/Webpage_Upload", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        // local tracking of which webpage is open
        Active_Webpage = 51;
        // debug message indicating that this routine was called
        Serial.println("Senidng Web page file management webpage. Webpage #: " + String(Active_Webpage));
        events.send(w_upload, "Webpage_Upload", millis());
        request->send(200);
        events.send(String((cardSize / 1024)).c_str(), "SD_size", millis());
        events.send(String(cardused).c_str(), "SD_used", millis());
        events.send(String((cardFree / 1024)).c_str(), "SD_free", millis()); 
    });

    server.on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
        Serial.println(logmessage);
        request->send(200, "text/plain", listFiles()); 
    });

    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request)
    {
        //Serial.println("Got upload post...now what?");
        if(checkFileList == 1) {
            Serial.println("updating file list");
            File_List();
            checkFileList = 0;
        }
        request->send(200,"text/plain", listFiles()); 
    },handleUpload);

    server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
        Serial.println(logmessage);

        if (request->hasParam("name") && request->hasParam("action")) {
            const char *fileName = request->getParam("name")->value().c_str();
            const char *fileAction = request->getParam("action")->value().c_str();

            logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url() + "?name=" + String(fileName) + "&action=" + String(fileAction);

            Serial.println("Requested file:"+String(fileName));
            Serial.println("File exist?: "+String(SD.exists(fileName)));
            //Serial.println(""+SD.o);
            if (!SD.exists(fileName)) {
              Serial.println(logmessage + " ERROR: file does not exist");
              request->send(400, "text/plain", "ERROR: file does not exist");
            } else {
                Serial.println(logmessage + " file exists");
                if (strcmp(fileAction, "download") == 0) {
                    logmessage += " downloaded";
                    request->send(SD, fileName, "application/octet-stream");
                } else if (strcmp(fileAction, "delete") == 0) {
                    
                    File openFile = SD.open(fileName);
                    if(openFile.isDirectory())
                    {
                        openFile.close();
                        SD.rmdir(fileName);
                        logmessage += " Directory deleted";
                    } else {
                        openFile.close();
                        SD.remove(fileName);
                        logmessage += " File deleted";
                    }
                    
                    Serial.println("updating file list");
                    File_List();
                    
                    request->send(200, "text/plain", "Deleted File: " + String(fileName));
                } else {
                    logmessage += " ERROR: invalid action param supplied";
                    request->send(400, "text/plain", "ERROR: invalid action param supplied");
                }
                Serial.println(logmessage);
            }
        } else {
            request->send(400, "text/plain", "ERROR: name and action params required");
        } 
    });

    server.on("/Reciever_Monitoring", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Active_Webpage = 11;
        
        //if(pwm_in_ISR_sequence_CH[1] == 2 && pwm_in_ISR_sequence_CH[2] == 2 && pwm_in_ISR_sequence_CH[3] == 2 && pwm_in_ISR_sequence_CH[4] == 2)
        {
            Serial.println("Senidng RX monitor webpage. Webpage #: " + String(Active_Webpage));
            File sd = SD.open("/Reciever_Monitoring.html");
            String SD_Webpage = sd.readString();
            sd.close();
            request->send(200,"text/plain",SD_Webpage);   
        }
        
        
    });
}

/**********************************************************************
 *
 *                   Webserver event handling
 *
 ***********************************************************************/
void Web_Server_Event_Handle()
{
    Serial.println("Initializing....Web Server Event Handler");

    events.onConnect([](AsyncEventSourceClient *client)
                     {
        if(client->lastId())
        {
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());    
        }      
        // send event with message "hello!", id current millis
        // and set reconnect delay to 1 second
        client->send("hello!", NULL, millis(), 100000); });
    server.addHandler(&events);
}

/**********************************************************************
 *
 *                    Starting the actual server
 *
 ***********************************************************************/
void Start_Server()
{
    Serial.println("Initializing....Webserver");
    AsyncElegantOTA.begin(&server);
    server.begin();
}

/**********************************************************************
 *
 *           Server call function when file is being uploaded
 *
 ***********************************************************************/
// handles uploads to the filserver
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    String logmessage = "";

    // Serial.println("Upload Handler entered");

    // make sure authenticated before allowing upload
    logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
    Serial.println(logmessage);

    if (!index)
    {
        String logmessage = "Upload Start: " + String(filename);
        // open the file on first call and store the file handle in the request object
        request->_tempFile = SD.open("/" + filename, "w");
        Serial.println(logmessage);
    }

    if (len)
    {
        // stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
        logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
        Serial.println(logmessage);
    }

    if (final)
    {
        logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
        // close the file handle as the upload is now done
        request->_tempFile.close();
        Serial.println(logmessage);
        if (checkFileList == 0)
            checkFileList = 1;
        request->redirect("/");
    }
}

/**********************************************************************
*
*
*
***********************************************************************/
void Update_RX_Graph_Webpage()
{
    if((millis() - lastTime_RX[0]) > lastTime_RX[1]) {
        events.send(Update_RX_JSON().c_str(), "RX_Values", millis());
        lastTime_RX[0] = millis();
    }
}