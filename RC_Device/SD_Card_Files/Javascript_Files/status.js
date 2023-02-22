source.addEventListener('hardware_status', function (e) {
    console.log("Hardware status response", e.data);
    document.getElementById("ARTICLE").innerHTML = e.data;
}, false);

source.addEventListener('SD_cardStatus', function (e) {
    console.log("SD_CARD status response", e.data);
    document.getElementById("SD_cardStatus").innerHTML = e.data;
}, false);

source.addEventListener('RX_Status', function (e) {
    console.log("Reciever status response", e.data);
    document.getElementById("RX_Status").innerHTML = e.data;
}, false);

source.addEventListener('IMU_Status', function (e) {
    console.log("IMU status response", e.data);
    document.getElementById("IMU_Status").innerHTML = e.data;
}, false);

source.addEventListener('GPS_Status', function (e) {
    console.log("GPS status response", e.data);
    document.getElementById("GPS_Status").innerHTML = e.data;
}, false);

source.addEventListener('Temp_Status', function (e) {
    console.log("Reciever status response", e.data);
    document.getElementById("Temp_Status").innerHTML = e.data;
}, false);

source.addEventListener('RPM_Status', function (e) {
    console.log("IMU status response", e.data);
    document.getElementById("RPM_Status").innerHTML = e.data;
}, false);

source.addEventListener('Battery_Status', function (e) {
    console.log("GPS status response", e.data);
    document.getElementById("Battery_Status").innerHTML = e.data;
}, false);

source.addEventListener('Webpage_Upload', function (e) {
    console.log("Webpage_Upload response", e.data);
    if(HWconfigUpdate)stopHWconfigUpdate();
    document.getElementById("ARTICLE").innerHTML = e.data;
}, false);