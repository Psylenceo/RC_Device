import * as receiver from './receiver.js';

var dt = new Date();
setTimeout(console.log(dt.toString()), 100);

document.addEventListener("DOMContentLoaded", function () {
    document.getElementById("datetime").textContent = dt;
    // Update the datetime every second
    setInterval(function () {
        dt = new Date();
        document.getElementById("datetime").textContent = dt;
    }, 1000);

    //send local client date and time to webserver for timestamping
    setTimeout(function() {
    var timesync = new XMLHttpRequest();
    timesync.open("GET","/time?timeDate=" + dt.toString(), true);
    timesync.send();
    }, 500);
    
    setTimeout(function() {
        receiver.getReciever();
    },750);
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

/*function recieverBtn(){
    receiver.recieverPage();
}*/