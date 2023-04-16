var time = Date();
setTimeout(console.log(time.toString()), 100);


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

function loadStatus() {
    xmlhttp = new XMLHttpRequest();
    console.log("Hardware status request");
    xmlhttp.open("GET", "/hardwareStatus", false);
    xmlhttp.send();
}

function webpageRequest(element) {
    //show which button was clickedconsole.log(element.id + " request");
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

//update local clock on webpages
setInterval(function() {
    var liveTime = Date();
    document.getElementById("RTC").textContent = liveTime;
}, 1000);

//send local client date and time to webserver for timestamping
setTimeout(function() {
    var timesync = new XMLHttpRequest();
    timesync.open("GET","/time?timeDate=" + time.toString(), true);
    timesync.send();
}, 500);

setTimeout(loadStatus, 500);