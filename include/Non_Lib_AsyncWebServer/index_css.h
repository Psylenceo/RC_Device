#ifndef index_css_H
#define index_css_H

const char index_css[] PROGMEM = R"rawliteral(
    /* modified css styling suggested by chat */

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
)rawliteral";

#endif