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

function listFilesButton() {
    console.log("listFiles request");
    xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", "/listfiles", false);
    xmlhttp.send();
    document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
    document.getElementById("details").innerHTML = xmlhttp.responseText;
}

function showUploadButtonFancy() {
    document.getElementById("detailsheader").innerHTML = "<h3>Upload File<h3>"
    document.getElementById("status").innerHTML = "";
    var uploadform = "<form method = \"POST\" action = \"/\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"data\"/><input type=\"submit\" name=\"upload\" value=\"Upload\" title = \"Upload File\"></form>"
    console.log("Upload File request");
    document.getElementById("details").innerHTML = uploadform;
    var uploadform =
        "<form id=\"upload_form\" enctype=\"multipart/form-data\" method=\"post\">" +
        "<input type=\"file\" name=\"file1\" id=\"file1\" onchange=\"uploadFile()\"><br>" +
        "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>" +
        "<h3 id=\"status\"></h3>" +
        "<p id=\"loaded_n_total\"></p>" +
        "</form>";
    document.getElementById("details").innerHTML = uploadform;
}

function _(el) {
    return document.getElementById(el);
}

function uploadFile() {
    console.log("Upload File selected");
    var file = _("file1").files[0];
    // alert(file.name+" | "+file.size+" | "+file.type);
    var formdata = new FormData();
    formdata.append("file1", file);
    var ajax = new XMLHttpRequest();
    ajax.upload.addEventListener("progress", progressHandler, false);
    ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
    ajax.addEventListener("error", errorHandler, false);
    ajax.addEventListener("abort", abortHandler, false);
    ajax.open("POST", "/upload");
    ajax.send(formdata);
}

function progressHandler(event) {
    //_("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total; // event.total doesnt show accurate total file size
    _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
    var percent = (event.loaded / event.total) * 100;
    _("progressBar").value = Math.round(percent);
    _("status").innerHTML = Math.round(percent) + "% uploaded... please wait";
    if (percent >= 100) {
        _("status").innerHTML = "Please wait, writing file to filesystem";
    }
}

function completeHandler(event) {
    _("status").innerHTML = "Upload Complete";
    _("progressBar").value = 0;
    xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", "/listfiles", false);
    xmlhttp.send();
    document.getElementById("status").innerHTML = "File Uploaded";
    document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
    document.getElementById("details").innerHTML = xmlhttp.responseText;
}

function errorHandler(event) {
    _("status").innerHTML = "Upload Failed";
}

function abortHandler(event) {
    _("status").innerHTML = "inUpload Aborted";
}

function downloadDeleteButton(filename, action) {
    var urltocall = "/file?name=" + filename + "&action=" + action;
    xmlhttp = new XMLHttpRequest();
    if (action == "delete") {
        console.log("Delete File selected");
        xmlhttp.open("GET", urltocall, false);
        xmlhttp.send();
        document.getElementById("status").innerHTML = xmlhttp.responseText;
        xmlhttp.open("GET", "/listfiles", false);
        xmlhttp.send();
        document.getElementById("details").innerHTML = xmlhttp.responseText;
    }
    if (action == "download") {
        console.log("Download File selected");
        document.getElementById("status").innerHTML = "";
        window.open(urltocall, "_blank");
    }
}