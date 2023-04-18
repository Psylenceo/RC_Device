import { RXEventListener } from './receiver.js';

let source;

if (!!window.EventSource) {
    source = new EventSource('/events');
    source.addEventListener('open', function (e) {
        console.log("Events Connected");
    }, false);

    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            console.log("Events Disconnected");
        }
    }, false);

    source.addEventListener('RX_Values',RXEventListener,false);

    source.addEventListener('Webpage_Upload', function (e) {
        console.log("Webpage_Upload response", e.data);
        document.getElementById("ARTICLE").innerHTML = e.data;
    }, false);
    
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
}

export function resetRxEventListener(){
    source.removeEventListener('RX_Values', RXEventListener);
    setTimeout(function() {
      source.addEventListener('RX_Values', RXEventListener,false);
    },75);
}