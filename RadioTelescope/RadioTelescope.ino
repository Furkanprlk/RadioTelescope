/**..
  ESP8266 - Basic HTTP server with multiple pages from SPIFFS
  v. 1.0
  Copyright (C) 2019 Robert Ulbricht
  https://www.arduinoslovakia.eu

  IDE: 1.8.6 or higher
  Board: NodeMCU 0.9 (ESP-12)
  Core: https://github.com/esp8266/Arduino
  Version: 2.5.0

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <FS.h>

#include "ESP8266Essentials.h"

const char* ssid = "Furkan Parlak";  // Enter SSID here
const char* password = "RV7CUUPTJTFC";  //Enter Password here

const char* tempssid;
const char* temppassword;

const char* APssid = "NeTracker";  // Enter SSID here
const char* APpassword = "1234567890";

//IPAddress ip(192, 168, 1, 220);


int aad = 0;                    //Kullanılıyor
int xmode = 0;                  //Kullanılıyor
int sys = 5;                   //Kullanılıyor
int motors[2] = {0, 0};         //Kullanılıyor
bool directions[2] = {0, 0};    //Kullanılıyor
int zaz = 0;                    //Kullanılıyor
float cord[2] = {0, 0};         //Kullanılıyor
String hiz;
String yon;
int deBug[6] = {0, 0, 0, 0, 0, 0};

int Step[2] = {0, 5}; //GPIO0---D3 of Nodemcu--Step of stepper motor driver
int Dir[2]  = {2, 4}; //GPIO2---D4 of Nodemcu--Direction of stepper motor driver


ESP8266WebServer server(80);


void homme() {
  String message = "<html>";
  message += "<body>";
  message += "<progress id=\"file\" max=\"100\" value=\"";
  message += aad;
  message += "\"></progress>";
  message += "<p style=\"color: #fd7878\">X=" + String(cord[0]) + " - Y=" + String(cord[1]) + "</p>";
  //message += "<br><br><label id=\"starrt\" style=\"color: #878787\";>System Start:</label>";
  //message += "<button onclick=\"starrt()\">Start</button>";
  message += "</body>";
  message += "</html>";
  Serial.print("HOME ");
  server.send(200, "text/html", message);
}


void moveMotors()
{
  yon = server.arg("yon");
  hiz = server.arg("hiz");

  Serial.println(yon + ' ' + hiz);

  if (yon.startsWith("0")) { //up
    sys = 0;
  } else if (yon.startsWith("1")) { //down
    sys = 1;
  }
  else if (yon.startsWith("2")) { //left
    sys = 2;
  } else if (yon.startsWith("3")) { //right
    sys = 3;
  }
  else if (yon.startsWith("4")) { //automathic
    if (sys == 4)
      sys = 5;
    else if (sys == 5)
      sys = 4;
  }
  else if (yon.startsWith("5")) { //stop
    sys = 5;
  }

  server.send(200, "text/plain"); //Send web page
}

void scan() {
  int Tnetwork = 0, i = 0, len = 0;
  Tnetwork = WiFi.scanNetworks();//Scan for total networks available
  String s = "{\"wifiList\":[";
  for (int i = 0; i < Tnetwork; ++i)
  {
    //Serial.println(String(WiFi.SSID(i)));
    s += "\"" + String(WiFi.SSID(i)) + "\",";
  }
  s = s.substring(0, s.length() - 1);
  s +=  "]}";

  //Serial.println(s);
  File file = SPIFFS.open("/data.json", "w");
  if (!file) {
    Serial.println("Error opening file for writing");
    return;
  }

  file.print(s);
  file.close();
  /*
    //StaticJsonBuffer<500> jsonBuffer;
    //JsonObject& root = jsonBuffer.createObject();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, s);
    JsonObject& obj = doc.as<JsonObject>();
    Serial.println("Save History");
    File historyFile = SPIFFS.open("/data.json", "w");
    obj.printTo("/data.json");
    historyFile.close();*/
}
void readJSON(String path) {
  File file2 = SPIFFS.open(path, "r");

  if (!file2) {
    Serial.println("Failed to open " + path + " for reading");
    return;
  }
  Serial.println("");
  Serial.println(path + " Content:");
  while (file2.available()) {
    Serial.write(file2.read());
  }
  file2.close();
}
void datajson() {
  scan();
  File file2 = SPIFFS.open("/data.json", "r");

  if (!file2) {
    Serial.println("Failed to open data.json for reading");
    return;
  }

  //Serial.println("JSON Content:");
  String kk = "";
  while (file2.available()) {

    kk += char(file2.read());

  }
  server.send(200, "text/html", kk);
  file2.close();

}


void systemSave() {
  ssid = string2char(server.arg("SSID"));
  password = string2char(server.arg("passwd"));

  Serial.println(server.arg("SSID"));
  Serial.println(server.arg("passwd"));
  Serial.println(server.arg("Mode"));

  File file = SPIFFS.open("/tempssid.txt", "w");
  if (!file) {
    Serial.println("Error opening file for writing");
  }
  else {
    String s = server.arg("SSID") + "|" + server.arg("passwd") + "|" + server.arg("Mode");
    file.print(s);
    file.close();
  }

  String mgds = "<html><head><meta http-equiv=\"refresh\" content=\"0;URL=http://";
  mgds += ip2Str(WiFi.localIP());
  mgds += "/index.html\"></head></html>";
  server.send(200, "text/html", mgds);
}
void sistemGiris() {
  String s1, s2;
  File file = SPIFFS.open("/ssid.txt", "r");
  if (!file) {
    Serial.println("ssid.txt açılamadı");
    deBug[0]++;
  } else {
    Serial.println("ssid.txt açıldı");
    s1 = file.readStringUntil('|');
    s2 = file.readStringUntil('|');
    xmode = ((int)file.read()) - 48;

    ssid = (char*)s1.c_str();
    password = (char*)s2.c_str();
  }
  file.close();

  File tfile = SPIFFS.open("/tempssid.txt", "r");
  if (!tfile) {
    Serial.println("tempssid.txt açılamadı");
    deBug[0]++;
  } else {
    Serial.println("tempssid.txt açıldı");
    s1 = tfile.readStringUntil('|');
    s2 = tfile.readStringUntil('|');
    xmode = ((int)tfile.read()) - 48;

    tempssid = (char*)s1.c_str();
    temppassword = (char*)s2.c_str();
  }
  tfile.close();
  Serial.println("Buraya Gelmesi Lazim Eger Gelmezse Sikinti");
  Serial.println("====[ New SSID ]====");
  Serial.print("NSSID= ");
  Serial.println(ssid);
  Serial.print("NPassword= ");
  Serial.println(password);
  Serial.print("Nmode= ");
  Serial.println(xmode);
  Serial.println("====[ Temp SSID ]====");
  Serial.print("TSSID= ");
  Serial.println(tempssid);
  Serial.print("TPassword= ");
  Serial.println(temppassword);
  Serial.print("Tmode= ");
  Serial.println(xmode);
  if (deBug[0] == 2)
    Serial.println("Sistemden Hic Veri Cekilemedi");
  if (!xmode) {
    wifiStarting(0, APssid, APpassword, APssid, APpassword);
  } else if (!wifiStarting(1, tempssid, temppassword, 10)) {
    if (!wifiStarting(1, ssid, password, 10)) {
      wifiStarting(0, APssid, APpassword, APssid, APpassword);
    }
  }
}
void sysReset() {
  server.sendHeader("Location", "/index.html");
  server.send(301);
  Serial.println("Reset..");
  ESP.restart();
}
void setup(void) {
  Serial.begin(115200);
  SPIFFS.begin();
  WiFi.hostname("netracker");
  sistemGiris();

  //readJSON("/ssid.txt");




  Serial.println("");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(Step[0], OUTPUT); //Step pin as output
  pinMode(Dir[0],  OUTPUT); //Direcction pin as output
  digitalWrite(Step[0], LOW); // Currently no stepper motor movement
  digitalWrite(Dir[0], LOW);
  pinMode(Step[1], OUTPUT); //Step pin as output
  pinMode(Dir[1],  OUTPUT); //Direcction pin as output
  digitalWrite(Step[1], LOW); // Currently no stepper motor movement
  digitalWrite(Dir[1], LOW);

  if(MDNS.begin("netracker"))
    Serial.println("mDNS Responder Started!");

  server.on("/home", homme);
  server.on("/data.json", datajson);
  server.on("/Save", systemSave);
  server.on("/sReset", sysReset);
  //  server.on("/starrt", starrt);
  server.on("/moveMotors", moveMotors);

  scan();

  //readJSON("/data.json"); //json içini görmek için kullandık

  server.serveStatic("/", SPIFFS, "/netflis.html");
  server.serveStatic("/index.html", SPIFFS, "/index.html");
  server.serveStatic("/netflis.css", SPIFFS, "/netflis.css");
  server.serveStatic("/style.css", SPIFFS, "/style.css");
  server.serveStatic("/script.js", SPIFFS, "/script.js");
  server.serveStatic("/jquery.js", SPIFFS, "/jquery.js");
  server.serveStatic("/netracker.ico", SPIFFS, "/netracker.ico");
  //server.serveStatic("/data.json", SPIFFS, "/data.json");


  server.begin();
  Serial.println("HTTP server started");

  MDNS.addService("http", "tcp", 80);
}
void motor(int cc) {
  // stringstream degree(sayyi);
  if (cc == 0) {                    //UP
    don(1, 1, hiz.toInt());
    Serial.println("UP");
  } else if (cc == 1) {              //DOWN
    don(1, 0, hiz.toInt());
    Serial.println("DOWN");
  } else if (cc == 2) {              //LEFT
    don(0, 1, hiz.toInt());
    Serial.println("LEFT");
  } else if (cc == 3) {              //RIGHT
    don(0, 0, hiz.toInt());
    Serial.println("RIGHT");
  } else if (cc == 4) {              //AUTOMATIC
    Serial.println("AUTOMATIC");
  }
}


void don(bool mot, bool yon, int mspeed) {
  digitalWrite(Dir[mot], yon); //Rotate stepper motor in clock wise direction
  digitalWrite(Step[mot], HIGH);
  delay(mspeed);
  digitalWrite(Step[mot], LOW);
  delay(mspeed);
}
void loop(void) {
  server.handleClient();
  motor(sys);
  MDNS.update();
}
