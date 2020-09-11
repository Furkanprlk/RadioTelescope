#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);


void apMode(const char* APssid, const char* APpassword) {
  WiFi.mode(WIFI_AP);
  Serial.println("WiFi Access Point Mode\n");
  Serial.println(WiFi.softAP(APssid, APpassword) ? "Ready!\n" : "Failed!\n");
  Serial.print("Access Point IP address = ");
  Serial.println(WiFi.softAPIP());
}
int staMode(const char* ssid, const char* password) {
  Serial.println("WiFi Station Mode");
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int wifiTime = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (wifiTime++ > 10)
      break;
    else {
      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.print("IP: ");  Serial.println(WiFi.localIP());
    }
  }
  return wifiTime;
}

bool wifiStarting (int Mode, const char* ssid, const char* password, IPAddress ip, int timeout=10) {
  WiFi.config(ip, gateway, subnet);
  WiFi.softAPConfig(ip, gateway, subnet);
  if (Mode == 0) //First Connection Mode ( Only AP Mode )
    apMode(ssid, password);
  else if (Mode == 1)
    if (staMode(ssid, password) > timeout){
      return false;
    }else 
      return true;
}

void wifiStarting (int Mode, const char* ssid, const char* password, const char* APssid, const char* APpassword, IPAddress ip) {
  WiFi.config(ip, gateway, subnet);
  WiFi.softAPConfig(ip, gateway, subnet);

  if (Mode == 0) { //First Connection Mode ( Only AP Mode )
    apMode(APssid, APpassword);
  } else if (Mode == 1) { //Standard Mode
    if (staMode(ssid, password) > 10)
      apMode(APssid, APpassword);
  }
}
void wifiStarting (int Mode, const char* ssid, const char* password, const char* APssid, const char* APpassword, IPAddress ip, const char* tempssid, const char* temppassword) {
  WiFi.config(ip, gateway, subnet);
  WiFi.softAPConfig(ip, gateway, subnet);

  if (Mode == 0) { //First Connection Mode ( Only AP Mode )
    apMode(APssid, APpassword);

  } else if (Mode == 1) { //Standard Mode
    if (staMode(tempssid, temppassword) > 10)
      if (staMode(ssid, password) > 10)
        apMode(APssid, APpassword);
  }
}
String ip2Str(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}
char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}