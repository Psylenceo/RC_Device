#ifndef index_html_H
#define index_html_H


const uint8_t index_html[] PROGMEM = R"rawliteral(
 <!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>RC Light Controller</title>
        <link rel="icon" type="image/png" href="https://www.arrmaforum.com/favicon-32x32.png" sizes="32x32" />
        <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
        <style>
            * {
             box-sizing: border-box;
            }

            body {
                font-family: Arial, Helvetica, sans-serif;
            }

            /* Style the header */
            header {
                background-color: #666;
                padding: 30px;
                text-align: center;
                font-size: 35px;
                color: white;
            }       

            /* Container for flexboxes */
            section {
                display: -webkit-flex;
                display: flex;
            }

            /* Style the navigation menu */
            nav {
                -webkit-flex: 1;
                -ms-flex: 1;
                flex: 1;
                background: #ccc;
                padding: 20px;
                text-align: Left
            }

            /* Style the list inside the menu */
            nav ul {
                list-style-type: none;
                padding: 0;
            }

            button.Sidebar { background:none; border:none; }

            .Sidebar:hover {
                background:transparent;
                cursor: pointer;
            }

            /* Style the content */
            article {
                -webkit-flex: 3;
                -ms-flex: 3;
                flex: 3;
                background-color: #f1f1f1;
                padding: 10px;
                text-align: center
            }

            .switch {
                position: relative;
                display: inline-block;
                width: 60px;
                height: 34px
            }

            .switch input {
                display: none
            }

            .slider {
                position: absolute;
                top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                background-color: #ccc;
                border-radius: 17px
            }

            .slider:before {
                position: absolute;
                content: "";
                height: 26px;
                width: 26px;
                left: 4px;
                bottom: 4px;
                background-color: #fff;
                -webkit-transition: .4s;
                transition: .4s;
                border-radius: 34px
            }
            .flex-container {
            display: flex;
            }
            .flex-container > div {
                display: flex;
            }

            input:checked+.slider {
                background-color: #2196F3
            }

            input:checked+.slider:before {
                -webkit-transform: translateX(26px);
                -ms-transform: translateX(26px);
                transform: translateX(26px)
            }

            /* Style the footer */
            footer {
                background-color: #777;
                padding: 10px;
                text-align: center;
                color: white;
            }

            /* Responsive layout - makes the menu and the content (inside the section) sit on top of each other instead of next to each other */
            @media (max-width: 600px) {
                section {
                    -webkit-flex-direction: column;
                    flex-direction: column;
                }
            }
        </style>
    </head>
    <body>
        <header>
            <h1>RC Light Controller</h1>
            <h3>SafeMode</h3>
        </header>
        <section>
            <nav>
                <ul>
                    %SIDEBAR%
                </ul>
            </nav>
            <article>
                <div id="ARTICLE"></div>
                <script>
                    var dt = new Date();
                    setTimeout(console.log(dt.toString()), 100);
                    document.addEventListener("DOMContentLoaded", function () {
                        document.getElementById("RTC").textContent = dt;
                        setInterval(function() {
                            var dt = new Date();
                            document.getElementById("RTC").textContent = dt;
                        }, 1000);

                        //send local client date and time to webserver for timestamping
                        setTimeout(function() {
                            var timesync = new XMLHttpRequest();
                            timesync.open("GET","/time?timeDate=" + dt.toString(), true);
                            timesync.send();
                            }, 500);
                    });

                    if (!!window.EventSource) {
                        var source = new EventSource('/events');

                        source.addEventListener('open', function(e) {
                            console.log("Events Connected");
                        }, false);
  
                        source.addEventListener('error', function(e) {
                            if (e.target.readyState != EventSource.OPEN) {
                                console.log("Events Disconnected");
                            }
                        }, false);
                    }

                    /*function loadStatus() {
                        xmlhttp = new XMLHttpRequest();
                        console.log("Hardware status request");
                        xmlhttp.open("GET", "/hardwareStatus", false);
                        xmlhttp.send();                        
                    }*/
                    
                    source.addEventListener('hardware_status', function(e) {
                        console.log("Hardware status response", e.data);
                        document.getElementById("ARTICLE").innerHTML = e.data;
                    }, false);

                    source.addEventListener('SD_cardStatus', function(e) {
                        console.log("SD_CARD status response", e.data);
                        document.getElementById("SD_cardStatus").innerHTML = e.data;
                    }, false);
                    
                    source.addEventListener('RX_Status', function(e) {
                        console.log("Reciever status response", e.data);
                        document.getElementById("RX_Status").innerHTML = e.data;
                    }, false);
                    
                    source.addEventListener('IMU_Status', function(e) {
                        console.log("IMU status response", e.data);
                        document.getElementById("IMU_Status").innerHTML = e.data;
                    }, false);
                    
                    source.addEventListener('GPS_Status', function(e) {
                        console.log("GPS status response", e.data);
                        document.getElementById("GPS_Status").innerHTML = e.data;
                    }, false);

                    source.addEventListener('Temp_Status', function(e) {
                        console.log("Reciever status response", e.data);
                        document.getElementById("Temp_Status").innerHTML = e.data;
                    }, false);
                    
                    source.addEventListener('RPM_Status', function(e) {
                        console.log("IMU status response", e.data);
                        document.getElementById("RPM_Status").innerHTML = e.data;
                    }, false);
                    
                    source.addEventListener('Battery_Status', function(e) {
                        console.log("GPS status response", e.data);
                        document.getElementById("Battery_Status").innerHTML = e.data;
                    }, false);
                    
                    source.addEventListener('Webpage_Upload', function(e) {
                        console.log("Webpage_Upload response", e.data);
                        document.getElementById("ARTICLE").innerHTML = e.data;
                    }, false);

                    function webpageRequest(element){
                        var xhr = new XMLHttpRequest();
                        xhr.open("Get", "/"+element.id, true);
                        console.log(element.id+" request");
                        xhr.send();
                        if(element.id == "HardwareConfig"){
                            document.getElementById("ARTICLE").innerHTML = Response.text();
                        }
                    }

                    source.addEventListener('SD_size', function (e) {
                        console.log("SD_size", e.data);
                        document.getElementById("SD_size").innerHTML = e.data + " MB";
                    }, false)

                    source.addEventListener('SD_used', function (e) {
                        console.log("SD_used", e.data);
                        document.getElementById("SD_used").innerHTML = e.data + " KB";
                    }, false)

                    source.addEventListener('SD_free', function (e) {
                        console.log("SD_free", e.data);
                        document.getElementById("SD_free").innerHTML = e.data + " MB";
                    }, false)

                    function listFilesButton() {
                        xmlhttp = new XMLHttpRequest();
                        console.log("listFiles request");
                        xmlhttp.open("GET", "/listfiles", false);
                        xmlhttp.send();
                        document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
                        document.getElementById("details").innerHTML = xmlhttp.responseText;
                    }

                    function showUploadButtonFancy() {
                        document.getElementById("detailsheader").innerHTML = "<h3>Upload File<h3>"
                        document.getElementById("status").innerHTML = "";
                        var uploadform = "<form method = \"POST\" action = \"/\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"data\"/><input type=\"submit\" name=\"upload\" value=\"Upload\" title = \"Upload File\"></form>"
                        console.log("Upload File request");
                        document.getElementById("details").innerHTML = uploadform;
                        var uploadform =
                            "<form id=\"upload_form\" enctype=\"multipart/form-data\" method=\"post\">" +
                            "<input type=\"file\" name=\"file1\" id=\"file1\" onchange=\"uploadFile()\"><br>" +
                            "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>" +
                            "<h3 id=\"status\"></h3>" +
                            "<p id=\"loaded_n_total\"></p>" +
                            "</form>";
                        document.getElementById("details").innerHTML = uploadform;
                    }

                    function _(el) {
                        return document.getElementById(el);
                    }
                    
                    function uploadFile() {
                        console.log("Upload File selected");
                        var file = _("file1").files[0];
                        // alert(file.name+" | "+file.size+" | "+file.type);
                        var formdata = new FormData();
                        formdata.append("file1", file);
                        var ajax = new XMLHttpRequest();
                        ajax.upload.addEventListener("progress", progressHandler, false);
                        ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
                        ajax.addEventListener("error", errorHandler, false);
                        ajax.addEventListener("abort", abortHandler, false);
                        ajax.open("POST", "/upload");
                        ajax.send(formdata);
                    }

                    function progressHandler(event) {
                        //_("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total; // event.total doesnt show accurate total file size
                        _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
                        var percent = (event.loaded / event.total) * 100;
                        _("progressBar").value = Math.round(percent);
                        _("status").innerHTML = Math.round(percent) + "% uploaded... please wait";
                        if (percent >= 100) {
                            _("status").innerHTML = "Please wait, writing file to filesystem";
                        }
                    }

                    function completeHandler(event) {
                        _("status").innerHTML = "Upload Complete";
                        _("progressBar").value = 0;
                        xmlhttp = new XMLHttpRequest();
                        xmlhttp.open("GET", "/listfiles", false);
                        xmlhttp.send();
                        document.getElementById("status").innerHTML = "File Uploaded";
                        document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
                        document.getElementById("details").innerHTML = xmlhttp.responseText;
                    }

                    function errorHandler(event) {
                        _("status").innerHTML = "Upload Failed";
                    }

                    function abortHandler(event) {
                        _("status").innerHTML = "inUpload Aborted";
                    }

                    function downloadDeleteButton(filename, action) {
                        var urltocall = "/file?name=" + filename + "&action=" + action;
                        xmlhttp = new XMLHttpRequest();
                        if (action == "delete") {
                            console.log("Delete File selected");
                            xmlhttp.open("GET", urltocall, false);
                            xmlhttp.send();
                            document.getElementById("status").innerHTML = xmlhttp.responseText;
                            xmlhttp.open("GET", "/listfiles", false);
                            xmlhttp.send();
                            document.getElementById("details").innerHTML = xmlhttp.responseText;
                        }
                        if (action == "download") {
                            console.log("Download File selected");
                            document.getElementById("status").innerHTML = "";
                            window.open(urltocall, "_blank");
                         }
                    }

                    /*setTimeout(loadStatus,500);*/
                </script>
            </article>
        </section>
        <footer>
            <p>,<span ID ="RTC"></span></p>
        </footer>
     </body>
</html>   
)rawliteral";
const char w_upload[] PROGMEM = R"rawliteral(
    <h1>Web Page File Management</h1>
            <p>SD Card Size: <span id="SD_size"></span></p>
            <p>SD Card used: <span id="SD_used"></span></p>
            <p>SD Card Free: <span id="SD_free"></span></p>
            <p>
                <button onclick="listFilesButton()">List Files</button>
                <button onclick="showUploadButtonFancy()">Upload File</button>
            </p>
            <p id="status"></p>
            <p id="detailsheader"></p>
            <p id="details"></p>
)rawliteral";
const char status[] PROGMEM = R"rawliteral(
    <h1>Connected devices</h1>
            <p>SD Card: <span id="SD_cardStatus"></span></p>
            <p>Reciever: <span id="RX_Status"></span></p>
            <p>IMU: <span id="IMU_Status"></span></p>
            <p>GPS: <span id="GPS_Status"></span></p>
            <p>Temperature sensor(s): <span id="Temp_Status"></span></p>
            <p>RPM Sensor: <span id="RPM_Status"></span></p>
            <p>Battery Pack: <span id="Battery_Status"></span></p>
            
)rawliteral";


#endif