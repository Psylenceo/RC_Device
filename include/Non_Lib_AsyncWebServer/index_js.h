#ifndef index_js_H
#define index_js_H

const uint8_t index_js[] PROGMEM = R"rawliteral(
    var dt = new Date();
    setTimeout(console.log(dt.toString()), 100);

    document.addEventListener("DOMContentLoaded", function () {
        document.getElementById("datetime").innerHTML = dt.toLocaleString();
       // Update the datetime every second
       setInterval(function () {
           dt = new Date();
        document.getElementById("datetime").innerHTML = dt.toLocaleString();
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

    source.addEventListener('open', function (e) {
        console.log("Events Connected");
    }, false);

    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            console.log("Events Disconnected");
        }
    }, false);
}

function webpageRequest(element) {
    console.log(element.id + " request");
    var webRequest = new XMLHttpRequest();
    webRequest.open("Get", "/" + element.id, false);
    webRequest.send();
    if (element.id == "HardwareConfig") {
        HWConfigPage();
        document.getElementById("ARTICLE").innerHTML = webRequest.responseText;
    }
    if (element.id == "Reciever_Monitoring") {
        document.getElementById("ARTICLE").innerHTML = webRequest.responseText;
    }    
}
)rawliteral";

#endif