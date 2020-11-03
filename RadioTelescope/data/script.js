function addOption(selectbox, text, value) {
  var optn = document.createElement("OPTION");
  optn.text = text;
  optn.value = value;
  selectbox.options.add(optn);
}

function setupWifiList() {
  document.getElementById('SSID').innerText = null
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      var obj = JSON.parse(this.responseText);
      //console.log(obj.wifiList[1])
      for (var i in obj.wifiList) {
        addOption(document.WIFI.SSID, obj.wifiList[i], obj.wifiList[i]);
      }
    }
  };
  xhttp.open("GET", "/data.json", true);
  xhttp.send();
}

setupWifiList();

function openPage(pageName, elmnt, color) {
  var i, tabcontent, tablinks;
  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }
  tablinks = document.getElementsByClassName("tablink");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].style.backgroundColor = "";
  }
  document.getElementById(pageName).style.display = "block";
  elmnt.style.backgroundColor = color;
}

// Get the element with id="defaultOpen" and click on it
document.getElementById("defaultOpen").click();

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pressure").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/home", true);
  xhttp.send();


}, 5000);


function reset() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("resset").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/sReset", true);
  xhttp.send();
}
function starrt() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("starrt").innerHTML = this.responseText;
    }
  };
  //xhttp.open("GET", "/starrt", true);
  xhttp.open("GET", "moveMotors?yon=4&hiz=" + ((30 - document.getElementById('hiz').value) + 2), true);
  xhttp.send();
}

$('#up').mousedown(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=0&hiz=" + ((30 - document.getElementById('hiz').value) + 2), true);
  xhttp.send();
});

$('#down').mousedown(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=1&hiz=" + ((30 - document.getElementById('hiz').value) + 2), true);
  xhttp.send();
});

$('#left').mousedown(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=2&hiz=" + ((30 - document.getElementById('hiz').value) + 2), true);
  xhttp.send();
});

$('#right').mousedown(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=3&hiz=" + ((30 - document.getElementById('hiz').value) + 2), true);
  xhttp.send();
});


$('#up').mouseup(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=5&hiz=0", true);
  xhttp.send();
});

$('#down').mouseup(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=5&hiz=0", true);
  xhttp.send();
});

$('#left').mouseup(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=5&hiz=0", true);
  xhttp.send();
});

$('#right').mouseup(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "moveMotors?yon=5&hiz=0", true);
  xhttp.send();
});


$('#hiz').change(function (event) {
  console.log();
});