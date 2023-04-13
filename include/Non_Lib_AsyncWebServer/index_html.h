#ifndef index_html_H
#define index_html_H

/*<script type="text/javascript" src="index.js"></script>
    <script src="/file_system_mgmt.js"></script>
    <script src="/receiver.js"></script>*/


const uint8_t index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
	<title>My Website</title>
	<style>
	* {
	box-sizing: border-box;
}

body {
	margin: 0;
	padding: 0;
	font-family: Arial, sans-serif;
}

.top {
	background-color: #333;
	text-align: center;
	padding: 20px;
	color: #fff;
}

.wrapper {
	display: flex;
	flex-wrap: wrap;
	justify-content: space-between;
	padding: 20px;
	max-width: 100%;
}

.nav {
	flex-basis: 20%;
	background-color: #f2f2f2;
	padding: 10px;
}

.nav ul {
	list-style: none;
	padding: 0;
	margin: 0;
}

.nav li {
	margin-bottom: 10px;
	border-bottom: 1px solid #ccc;
	/* Add border to separate the links */
}

.nav button {
	background-color: transparent;
	border: none;
	color: #000;
	cursor: pointer;
	font-size: 16px;
}

.nav button:hover {
	color: #00f;
	cursor: pointer;
}

.content {
	flex-basis: 75%;
	background-color: #fff;
	padding: 10px;
	max-width: 100%;
}

/* Add styles for the rows in the graph */
.main-rows {
	display: flex;
	flex-wrap: wrap;
	justify-content: space-between;
	align-items: center;
	height: 60px;
	background-color: #f2f2f2;
	margin-bottom: 10px;
	border-radius: 20px;
	padding: 10px;
	flex-direction: row;
	/* added */
	height: fit-content;
	max-width: 100%;
}

/* Add styles for the label in the rows */
.row-label {
	font-size: 18px;
	font-weight: bold;
	color: #333;
	flex-basis: 20%;
	text-align: left;
}

/* Add styles for the numbers in the columns */
.row-wrapper {
	display: flex;
	justify-content: space-between;
	align-items: center;
	flex-basis: 80%;
	text-align: center;
	flex-direction: column;
	/* added */
	height: fit-content;
	max-width: 100%;
}

/* Add styles for the numbers in the columns */
.row-top {
	font-size: 18px;
	font-weight: bold;
	color: #333;
	/*flex-basis: 80%;*/
	flex-direction: column;
	/* added */
	display: flex;
	/* added */
	max-width: 100%;
}

/* Add styles for the graphs in the columns */
.row-bottom {
	flex-basis: 100%;
	width: 100%;
	height: 30px;
	background-color: #ccc;
	border-radius: 10px;
	margin-left: 10px;
	flex-direction: column;
	/* added */
	display: flex;
	/* added */
	z-index: 1;
	/* Add this line */
}

.graph-container {
	position: relative;
	max-width: 100%;
	height: 30px;
	display: flex;
	justify-content: center;
	align-items: center;
	flex-direction: column;
	border: 1px solid #ccc;
	border-radius: 15px;
	overflow: hidden;
	/* Add overflow hidden property */
	z-index: 2;
	/* Add this line */
}

.graph-bar {
	position: absolute;
	top: 0;
	left: 50%;
	height: 100%;
	background-color: #4CAF50;
	transition: width 0.5s ease-in-out;
	border-radius: 15px;
}

.overlay {
	position: absolute;
	top: 0;
	left: 0;
	right: 0;
	bottom: 0;
	display: flex;
	justify-content: center;
	align-items: center;
	font-size: 24px;
	font-weight: bold;
}

.footer {
	background-color: #666;
	color: #fff;
	display: flex;
	/* Use flexbox to divide the footer into sections */
	justify-content: space-between;
	padding: 20px;
}

.footer-left {
	flex-basis: 30%;
}

.footer-middle {
	flex-basis: 40%;
	text-align: center;
}

.footer-right {
	flex-basis: 30%;
	text-align: right;
	color: #fff;
	/* Set white font color for the "TBD" text */
}
</style>
<script>
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
</script>
</head>
<body>
	<div class="top">
		<h1>My Website</h1>
		<p>Welcome to my website!</p>
	</div>
	<div class="wrapper">
		<div class="nav">
			<ul>
				^SIDEBAR^
			</ul>
		</div>
		<div class="content">
			<h2 id="title">Welcome to My Website</h2>
			<div id="main"></div>
		</div>
	</div>
	<div class="footer">
		<div class="footer-left">
			Debug info goes here
		</div>
		<div class="footer-middle">
			<span id="datetime"></span>
		</div>
		<div class="footer-right">
			TBD
		</div>
	</div>
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