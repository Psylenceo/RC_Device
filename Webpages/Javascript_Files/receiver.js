import * as events from './events.js';

var json;
var usedChannels = 3;
var valuesChannel = [["",0,1000,2000,1300,1700]]; //channel 0 reference base values
var graphBar = [];
var overlay = [];

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
      valuesChannel.push(["",0,1000,2000,1300,1700]);
    }
    getRXjsonValues();
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
      graphBar.push(document.createElement("div"));
      //graphBar = document.createElement("div");
      graphBar[i-1].classList.add("graph-bar");
    
      graphContainer.appendChild(graphBar[i-1]);

      // Add variable and text overlay
      overlay.push(document.createElement("div"));
      overlay[i-1].classList.add("overlay");
            
      graphContainer.appendChild(graphBar[i-1]);
      graphContainer.appendChild(overlay[i-1]);
      rowBottom.appendChild(graphContainer);

      rowWrapper.appendChild(rowTop);
      rowWrapper.appendChild(rowBottom);
      rowDiv.appendChild(rowLabel);
      rowDiv.appendChild(rowWrapper);
      mainDiv.appendChild(rowDiv);
      
  }
  updateRXgraphs();
}

window.recieverPage = recieverPage;

export function RXEventListener(e) {
  console.log("RX_Values", e.data);
  json = JSON.parse(e.data);
  getRXjsonValues();
  updateRXgraphs();
  events.resetRxEventListener();
}

function getRXjsonValues(){
  for(var i=1;i<=usedChannels;i++){
    valuesChannel[i][0] = json["ch" + i];
    valuesChannel[i][1] = json["ch" + i + "value"];
    if(json["ch" + i + "minRange"] > 800 && json["ch" + i + "minRange"] < 1000){
      valuesChannel[i][2] = json["ch" + i + "minRange"];
    }
    if(json["ch" + i + "maxRange"] > 2000 && json["ch" + i + "maxRange"] < 2400){
      valuesChannel[i][3] = json["ch" + i + "maxRange"];
    }
    if(json["ch" + i + "minDeadZone"] > (1000 + (500 * .7)) && json["ch" + i + "minDeadZone"] < 1500){
      valuesChannel[i][4] = json["ch" + i + "minDeadZone"];
    }
    if(json["ch" + i + "maxDeadZone"] > 1500 && json["ch" + i + "maxDeadZone"] < (1500 +(500 * 0.3))){
      valuesChannel[i][5] = json["ch" + i + "maxDeadZone"];
    }
  }
}

function updateRXgraphs() {
  for (var i = 1; i <= usedChannels; i++) {
    //console.log(i);
    //console.log(valuesChannel[i][1]);
    if (valuesChannel[i][1] >= 1500) {
      graphBar[i-1].style.left = '49%';
    } else if(valuesChannel[i][1] >= valuesChannel[i][2] && valuesChannel[i][1] < 1500) {
      graphBar[i-1].style.left = `${(49 - (Math.abs(valuesChannel[i][1] - 1500) / 10))}%`;
    } else {
      graphBar[i-1].style.left = '0%';
    }

    if (valuesChannel[i][1] >= valuesChannel[i][4] && valuesChannel[i][1] <= valuesChannel[i][5]) {
      graphBar[i-1].style.backgroundColor = "yellow";
    } else if (valuesChannel[i][1] <= valuesChannel[i][2] || valuesChannel[i][1] >= valuesChannel[i][3]) {
      graphBar[i-1].style.backgroundColor = "red";
    } else if ((valuesChannel[i][1] > valuesChannel[i][2] && valuesChannel[i][1] < 1000) ||
      (valuesChannel[i][1] > 2000 && valuesChannel[i][1] < valuesChannel[i][3])) {
      graphBar[i-1].style.backgroundColor = "orange";
    } else {
      graphBar[i-1].style.backgroundColor = "#4CAF50";
    }

    if(valuesChannel[i][1] <= valuesChannel[i][2] || valuesChannel[i][1] >= valuesChannel[i][3]){
      overlay[i-1].innerHTML = valuesChannel[i][1];
      graphBar[i-1].style.width = '100%'; 
    }else {
      overlay[i-1].innerHTML = valuesChannel[i][1];
      graphBar[i-1].style.width = ((Math.abs(valuesChannel[i][1] - 1500) / 10) + 2) + "%"; // set the width of the graph bar based on the difference between the variable and 1500
    }
  }
}