import * as index from './index.js';

var json;
var usedChannels = 3;
var valuesChannel = [["",0,1000,2000,1300,1700]]; //channel 0 reference base values
var graphBar;
var overlay;

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

export async function getReciever(){
  try{
    const response = await fetch('/detected_channels');
    json = await response.json();
    console.log(json);
    console.log("Number of detected Channels:",json.Channels);
    if(json.Channels > 0 && json.Channels < 9){
      usedChannels = json.Channels;
    }

    for(var i=1;i<=usedChannels;i++){
      valuesChannel.push(valuesChannel[0]);
      valuesChannel[i][0] = json["ch" + i];
      valuesChannel[i][1] = json["ch" + i + "value"];
      if(json.minRange != valuesChannel[i][2]) valuesChannel[i][2] = json["ch" + i + "minRange"];
      if(json.maxRange != valuesChannel[i][3]) valuesChannel[i][3] = json["ch" + i + "maxRange"];
      if(json.minDeadZone != valuesChannel[i][4]) valuesChannel[i][4] = json["ch" + i + "minDeadZone"];
      if(json.maxDeadZone != valuesChannel[i][5]) valuesChannel[i][5] = json["ch" + i + "maxDeadZone"];
    }

  } catch (error) {
    console.error('Error fetching data:', error);
  }
}

function recieverPage() {
  async function r(){
    try{
      var response = await fetch('/reciever');
      var text = await response.text();
      console.log(text);
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  }
  r();
  document.getElementById("title").innerHTML = "Reciever";

  var mainDiv = document.getElementById("main");
  mainDiv.innerHTML = "";

  for (var i = 1; i <= usedChannels; i++) {
    valuesChannel[i][0] = json["ch" + i];
      valuesChannel[i][1] = json["ch" + i + "value"];
      if(json.minRange != valuesChannel[i][2]) valuesChannel[i][2] = json["ch" + i + "minRange"];
      if(json.maxRange != valuesChannel[i][3]) valuesChannel[i][3] = json["ch" + i + "maxRange"];
      if(json.minDeadZone != valuesChannel[i][4]) valuesChannel[i][4] = json["ch" + i + "minDeadZone"];
      if(json.maxDeadZone != valuesChannel[i][5]) valuesChannel[i][5] = json["ch" + i + "maxDeadZone"];

      var rowDiv = document.createElement("div");
      rowDiv.classList.add("main-rows");

      var rowLabel = document.createElement("span");
      rowLabel.classList.add("row-label");
      if(valuesChannel[i][0] == ""){
          rowLabel.innerHTML = "Channel - " + i;
      } else {
        rowLabel.innerHTML = valuesChannel[i][0];
      }

      var rowWrapper = document.createElement("div");
      rowWrapper.classList.add("row-wrapper");

      var rowTop = document.createElement("div");
      rowTop.classList.add("row-top");
      rowTop.innerHTML = "Deadzone";

      var rowBottom = document.createElement("div");
      rowBottom.classList.add("row-bottom");

      var graphContainer = document.createElement("div");
      graphContainer.classList.add("graph-container");

      // Add graph bar
      graphBar = document.createElement("div");
      graphBar.classList.add("graph-bar");
      //valuesChannel[i][1] = Math.floor(Math.random() * 1000) + 1000; // generate a random value between 1000 and 2000
      if (valuesChannel[i][1] >= 1500) {
      graphBar.style.left = '49%';
    } else {
      graphBar.style.left = (49 - (Math.abs(valuesChannel[i][1] - 1500) / 10))+"%";
    }
    if (valuesChannel[i][1] >= valuesChannel[i][4] && valuesChannel[i][1] <= valuesChannel[i][5]) {
      graphBar.style.backgroundColor = "yellow";
    } else if (valuesChannel[i][1] <= valuesChannel[i][2] || valuesChannel[i][1] >= valuesChannel[i][3]) {
      graphBar.style.backgroundColor = "red";
    } else if ((valuesChannel[i][1] > valuesChannel[i][2] && valuesChannel[i][1] < 1000) ||
      (valuesChannel[i][1] > 2000 && valuesChannel[i][1] < valuesChannel[i][3])) {
      graphBar.style.backgroundColor = "orange";
    } else {
      graphBar.style.backgroundColor = "#4CAF50";
    }
      graphContainer.appendChild(graphBar);

      // Add variable and text overlay
      overlay = document.createElement("div");
      overlay.classList.add("overlay");
      overlay.innerHTML = valuesChannel[i][1];
      graphBar.style.width = ((Math.abs(valuesChannel[i][1] - 1500) / 10)+2) + "%"; // set the width of the graph bar based on the difference between the variable and 1500

      graphContainer.appendChild(graphBar);
      graphContainer.appendChild(overlay);
      rowBottom.appendChild(graphContainer);

      rowWrapper.appendChild(rowTop);
      rowWrapper.appendChild(rowBottom);
      rowDiv.appendChild(rowLabel);
      rowDiv.appendChild(rowWrapper);
      mainDiv.appendChild(rowDiv);
  }
}

window.recieverPage = recieverPage;

const RXEventListener = function(e){
  console.log("RX_Values", e.data);
    json = JSON.parse(e.data);
    for (var i = 1; i <= usedChannels; i++) {
      valuesChannel[i][0] = json["ch" + i];
        valuesChannel[i][1] = json["ch" + i + "value"];
        if(json.minRange != valuesChannel[i][2]) valuesChannel[i][2] = json["ch" + i + "minRange"];
        if(json.maxRange != valuesChannel[i][3]) valuesChannel[i][3] = json["ch" + i + "maxRange"];
        if(json.minDeadZone != valuesChannel[i][4]) valuesChannel[i][4] = json["ch" + i + "minDeadZone"];
        if(json.maxDeadZone != valuesChannel[i][5]) valuesChannel[i][5] = json["ch" + i + "maxDeadZone"];

        if (valuesChannel[i][1] >= 1500) {
          graphBar.style.left = '49%';
        } else {
          graphBar.style.left = `${(49 - (Math.abs(valuesChannel[i][1] - 1500) / 10))}%`;
        }
        if (valuesChannel[i][1] >= valuesChannel[i][4] && valuesChannel[i][1] <= valuesChannel[i][5]) {
          graphBar.style.backgroundColor = "yellow";
        } else if (valuesChannel[i][1] <= valuesChannel[i][2] || valuesChannel[i][1] >= valuesChannel[i][3]) {
          graphBar.style.backgroundColor = "red";
        } else if ((valuesChannel[i][1] > valuesChannel[i][2] && valuesChannel[i][1] < 1000) ||
          (valuesChannel[i][1] > 2000 && valuesChannel[i][1] < valuesChannel[i][3])) {
          graphBar.style.backgroundColor = "orange";
        } else {
          graphBar.style.backgroundColor = "#4CAF50";
        }

        overlay.innerHTML = valuesChannel[i][1];
      graphBar.style.width = ((Math.abs(valuesChannel[i][1] - 1500) / 10)+2) + "%"; // set the width of the graph bar based on the difference between the variable and 1500
    }
}

source.addEventListener('RX_Values',RXEventListener,false);

export function resetRxEventListener(){
  source.removeEventListener('RX_Values',RXEventListener);
  source.addEventListener('RX_Values', RXEventListener,false);
}