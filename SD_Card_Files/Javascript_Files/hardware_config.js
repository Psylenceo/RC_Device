let HWconfigUpdate;

function toggleCheckbox(element) {
    var ToggleChecks = new XMLHttpRequest();
    if (element.checked) { ToggleChecks.open("GET", "/update?option=" + element.id + "&state=1", true); }
    else { ToggleChecks.open("GET", "/update?option=" + element.id + "&state=0", true); }
    console.log(element.id);
    ToggleChecks.send();
    HWConfigPage();
}

function HWConfigPage() {
    //console.log("Request for updated config values");
    var HWconfigSelected = new XMLHttpRequest();
    HWconfigSelected.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var obj = JSON.parse(this.responseText);
            console.log(this.responseText);
            document.getElementById("Smart_RX_CHK").checked = obj.Smart_RX_CHK;
            document.getElementById("Smart_RX_Type").innerHTML = obj.Smart_RX_Type;

            document.getElementById("CH3_CHK").checked = obj.CH3_CHK;
            document.getElementById("CH3_State").innerHTML = obj.CH3_State;

            document.getElementById("CH4_CHK").checked = obj.CH4_CHK;
            document.getElementById("CH4_State").innerHTML = obj.CH4_State;

            document.getElementById("CH5_CHK").checked = obj.CH5_CHK;
            document.getElementById("CH5_State").innerHTML = obj.CH5_State;

            document.getElementById("CH6_CHK").checked = obj.CH6_CHK;
            document.getElementById("CH6_State").innerHTML = obj.CH6_State;

            document.getElementById("CH7_CHK").checked = obj.CH7_CHK;
            document.getElementById("CH7_State").innerHTML = obj.CH7_State;

            document.getElementById("CH8_CHK").checked = obj.CH8_CHK;
            document.getElementById("CH8_State").innerHTML = obj.CH8_State;

            document.getElementById("IMU_CHK").checked = obj.IMU_CHK;
            document.getElementById("IMU_EN").innerHTML = obj.IMU_EN;

            document.getElementById("GPS_CHK").checked = obj.GPS_CHK;
            document.getElementById("GPS_EN").innerHTML = obj.GPS_EN;

            document.getElementById("T_PROBE_CHK").checked = obj.T_PROBE_CHK;
            document.getElementById("T_PROBE_EN").innerHTML = obj.T_PROBE_EN;

            document.getElementById("RPM_PROBE_CHK").checked = obj.RPM_PROBE_CHK;
            document.getElementById("RPM_PROBE_EN").innerHTML = obj.RPM_PROBE_EN;

            document.getElementById("BAT_MOD_CHK").checked = obj.BAT_MOD_CHK;
            document.getElementById("BAT_MOD_EN").innerHTML = obj.BAT_MOD_EN;

            document.getElementById("EXP_MOD_CHK").checked = obj.EXP_MOD_CHK;
            document.getElementById("EXP_MOD_CHK").innerHTML = obj.EXP_MOD_CHK;
        }
    };
    HWconfigSelected.open("GET", "/state", true);
    HWconfigSelected.send();
}

function stopHWconfigUpdate() {
    clearInterval(HWconfigUpdate);
    HWconfigUpdate = null;
}
function SaveConfig(element) {
    var SaveHWconfig = new XMLHttpRequest();
    SaveHWconfig.open("GET", "/" + element.id, true);
    console.log(element.id + " request");
    SaveHWconfig.send();
}