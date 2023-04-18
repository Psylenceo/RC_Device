import * as events from './events.js';

var json;
var usedChannels = 3;
var valuesChannel = [["",0,1000,2000,1300,1700]]; //channel 0 reference base values
var graphBar;
var overlay;

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
      valuesChannel[i][0] = json["ch" + i];
      valuesChannel[i][1] = json["ch" + i + "value"];
      if(json["ch" + i + "minRange"] != valuesChannel[i][2]) valuesChannel[i][2] = json["ch" + i + "minRange"];
      if(json["ch" + i + "maxRange"] != valuesChannel[i][3]) valuesChannel[i][3] = json["ch" + i + "maxRange"];
      if(json["ch" + i + "minDeadZone"] != valuesChannel[i][4]) valuesChannel[i][4] = json["ch" + i + "minDeadZone"];
      if(json["ch" + i + "maxDeadZone"] != valuesChannel[i][5]) valuesChannel[i][5] = json["ch" + i + "maxDeadZone"];
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
    /*valuesChannel[i][0] = json["ch" + i];
      valuesChannel[i][1] = json["ch" + i + "value"];
      if(json.minRange != valuesChannel[i][2]) valuesChannel[i][2] = json["ch" + i + "minRange"];
      if(json.maxRange != valuesChannel[i][3]) valuesChannel[i][3] = json["ch" + i + "maxRange"];
      if(json.minDeadZone != valuesChannel[i][4]) valuesChannel[i][4] = json["ch" + i + "minDeadZone"];
      if(json.maxDeadZone != valuesChannel[i][5]) valuesChannel[i][5] = json["ch" + i + "maxDeadZone"];
      */
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
    /*if (valuesChannel[i][1] >= 1500) {
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
    }*/
      graphContainer.appendChild(graphBar);

      // Add variable and text overlay
      overlay = document.createElement("div");
      overlay.classList.add("overlay");
      /*overlay.innerHTML = valuesChannel[i][1];
      graphBar.style.width = ((Math.abs(valuesChannel[i][1] - 1500) / 10)+2) + "%"; // set the width of the graph bar based on the difference between the variable and 1500
      */
      
      graphContainer.appendChild(graphBar);
      graphContainer.appendChild(overlay);
      rowBottom.appendChild(graphContainer);

      rowWrapper.appendChild(rowTop);
      rowWrapper.appendChild(rowBottom);
      rowDiv.appendChild(rowLabel);
      rowDiv.appendChild(rowWrapper);
      mainDiv.appendChild(rowDiv);
      updateRXgraphs();
  }
}

window.recieverPage = recieverPage;

export function RXEventListener(e) {
  console.log("RX_Values", e.data);
  json = JSON.parse(e.data);
  //for (var i = 1; i <= usedChannels; i++) {
    /*valuesChannel[1][0] = json.ch1;
    valuesChannel[1][1] = json.ch1value;
    valuesChannel[1][2] = json.ch1minRange;
    valuesChannel[1][3] = json.ch1maxRange;
    valuesChannel[1][4] = json.ch1minDeadZone;
    valuesChannel[1][5] = json.ch1maxDeadZone;

    valuesChannel[2][0] = json.ch1;
    valuesChannel[2][1] = json.ch1value;
    valuesChannel[2][2] = json.ch1minRange;
    valuesChannel[2][3] = json.ch1maxRange;
    valuesChannel[2][4] = json.ch1minDeadZone;
    valuesChannel[2][5] = json.ch1maxDeadZone;

    valuesChannel[3][0] = json.ch1;
    valuesChannel[3][1] = json.ch1value;
    valuesChannel[3][2] = json.ch1minRange;
    valuesChannel[3][3] = json.ch1maxRange;
    valuesChannel[3][4] = json.ch1minDeadZone;
    valuesChannel[3][5] = json.ch1maxDeadZone;*/
  events.resetRxEventListener();
}



function getRXjsonValues(){

}

function updateRXgraphs() {
  for (var i = 1; i <= usedChannels; i++) {
    console.log(i);
    console.log(valuesChannel[i][1]);
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
    graphBar.style.width = ((Math.abs(valuesChannel[i][1] - 1500) / 10) + 2) + "%"; // set the width of the graph bar based on the difference between the variable and 1500
  }
}