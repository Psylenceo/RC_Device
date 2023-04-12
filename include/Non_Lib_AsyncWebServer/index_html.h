#ifndef index_html_H
#define index_html_H

/*<script type="text/javascript" src="index.js"></script>
    <script src="/file_system_mgmt.js"></script>
    <script src="/receiver.js"></script>*/


const uint8_t index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
	<title>My Website</title>
	<link rel="stylesheet" type="text/css" href="index.css">
	<script type="text/javascript" src="index.js"></script>
</head>
<body>
	<div class="top">
		<h1>My Website</h1>
		<p>Welcome to my website!</p>
	</div>
	<div class="wrapper">
		<div class="nav">
			<ul>
				%SIDEBAR%
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