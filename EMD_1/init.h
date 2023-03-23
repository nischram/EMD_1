#ifndef __INIT_H_
#define __INIT_H_

char OUT[12];
const char* ssid     = STASSID1;
const char* password = STAPSK1;

int rebootCount = 0;
int loopCounter = 0;
long resetMillis = 0, valueMillis = 0, printMillis = 0;
long lastHmMillis = 0, lastMbMillis = 0, lastDhtMillis = 0;
long lastScreenMillis = 0, weatherMillis = 0;
int IntervalModbus = 1;
int screenSaveTime, pirUse;
//------------------------------
// Serial
void serialBegin(){
  Serial.begin(SERIALSPEED);
  delay(800);
  Serial.println("");
  Serial.println("______________________________ ");
  Serial.println("");
  Serial.print("Serial Speed is    :  ");Serial.println(SERIALSPEED);
}
void serialPrintSW(){
  Serial.print("Software Version   : ");Serial.printf(" V%4.2f\n",SW_VERSION);
  Serial.print("Software Datum     : ");Serial.printf(" %s\n",SW_DATE);
}
//------------------------------
// WiFi network
void startWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("WiFi init          :  wait...");
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < WIFI_TIMEOUT * 2) {
    Serial.print(".");
    delay(500);
    timeout++;
  }
  if (timeout < WIFI_TIMEOUT * 2){
    Serial.println("");
    Serial.printf("WiFi SSID          :  %s\n",ssid);
    Serial.println("WiFi status        :  connected");
    Serial.print("ESP-IP address     :  ");
    Serial.println(WiFi.localIP());
    Serial.print("WiFi strength      :  ");Serial.printf("%6d dBm\n",WiFi.RSSI());
    wifiTimeout = false;
  }
  else {
    Serial.println("");
    Serial.printf("WiFi SSID          :  %s\n",ssid);
    Serial.println("WiFi status        :  failed!");
    wifiTimeout = true;
  }
  if (!wifiTimeout){
    tftPrintInit("WiFi SSID      : %s",ssid);
    tftPrintInit("WiFi status    : connected");
    tftPrintInit("ESP-IP address : %s",(char*) WiFi.localIP().toString().c_str(),3,0 );
    tftPrintInit("WiFi strength  : %d dBm",WiFi.RSSI());
  }
  else {
    tftPrintInit("WiFi SSID      : %s",ssid);
    tftPrintInit("WiFi status    : failed!");
  }
}
void initWifi(){
  ssid     = STASSID1;
  password = STAPSK1;
  startWifi();
  if (wifiTimeout){
    ssid     = STASSID2;
    password = STAPSK2;
   startWifi();
  }
  if (wifiTimeout){
    ssid     = STASSID3;
    password = STAPSK3;
    startWifi();
  }
}

//------------------------------
// init SD-Card
#include <SD.h>

char fileName[20], filePath[30], msgString[128];
bool logFileIsOpen =false;

char filename[] = "/";
bool cardin = false;
bool sdwrite = false;
File logFile;
//const int SD_CS = 32;

void initSD_Card(){
  Serial.print("SD-Card init       :  ");
  SD.begin(SD_CS);
  if (SD.exists("/")) {
    Serial.println("connected");
    tftPrintInit("SD-Card        : connected");
    cardin = true;
  }
  else {
    Serial.println("failed!");
    tftPrintInit("SD-Card        : failed!");
  }

}
bool checkFolder(char const * folder){
  if (SD.exists(folder)) {
    Serial.print("SD-Card folder     :  \"");Serial.print(folder);Serial.println("\" is OK");
    return true;
  }
  else {
    Serial.print("SD-Card            :  add folder \"");Serial.print(folder);Serial.print("\" ... ");
    SD.mkdir(folder);
    if (SD.exists(folder)) {
      Serial.println(" is added");
      return true;
    }
    else {
      Serial.println("SD-Card            :  error make folder ");
      return false;
    }
  }
}

//------------------------------
// Zähler für Reboot
void rebootCounter(){
  //paraESP.putUInt("BOOT", 0);   //einmalig aktivieren zum zurücksetzen
  rebootCount = paraESP.getUInt("BOOT");
  if (rebootCount >= 65634) rebootCount = 0;
  rebootCount++;
  Serial.print("Reboot counter     :  ");Serial.println(rebootCount);
  tftPrintInit("Reboot counter : %i", rebootCount);
  paraESP.putUInt("BOOT", rebootCount);
}
int readRebootCounter(){
  return paraESP.getUInt("BOOT");
}
int readTimeDiff(){
  int tDiff = paraESP.getUInt("TIME");
  if (tDiff == 3600) return tDiff;
  else if (tDiff == 7200) return tDiff;
  else paraESP.putUInt("TIME", 7200);
  return paraESP.getUInt("TIME");
}
// Zeitanzeige
int changeTimeDiff(){
  int tDiff = paraESP.getUInt("TIME");
  if (tDiff == 3600) paraESP.putUInt("TIME", 7200);
  else if (tDiff == 7200) paraESP.putUInt("TIME", 3600);
  else paraESP.putUInt("TIME", 7200);
  return paraESP.getUInt("TIME");
}
// Bildschirmschoner
int readScreenSave(){
  screenSaveTime = paraESP.getUInt("SCREEN");
  if (screenSaveTime < 0 )paraESP.putUInt("SCREEN", 0);
  else if (screenSaveTime > 15 )paraESP.putUInt("SCREEN", 15);
  return paraESP.getUInt("SCREEN");
}
int writeScreenSave(int value){
  if(value < 0) value = 0;
  paraESP.putUInt("SCREEN", value);
  return paraESP.getUInt("SCREEN");
}
// Bewegungsmelder
int changePIR(){
  int pir = paraESP.getUInt("PIR");
  if (pir == 0) paraESP.putUInt("PIR", 1);
  else paraESP.putUInt("PIR", 0);
  return paraESP.getUInt("PIR");
}
int readPIRuse(){
  return paraESP.getUInt("PIR");
}
void initPIR(){
    pirUse = readPIRuse();
    if(pirUse == 1){
      pinMode(PIR_PIN, INPUT);
      Serial.println("PIR Init           :  done");
      Serial.printf("PIR State          :  %i\n", digitalRead(PIR_PIN));
    }
    else Serial.println("PIR Init           :  not use!");
}
void readPIR(){
    if(pirUse == 1){
      Serial.printf("PIR State          :  %i\n", digitalRead(PIR_PIN));
    }
}
// DHT on screen
int changeDhtPut(){
  if (paraESP.getUInt("DHT_PUT") == 0) paraESP.putUInt("DHT_PUT", 1);
  else paraESP.putUInt("DHT_PUT", 0);
  return paraESP.getUInt("DHT_PUT");
}
int readDhtPut(){
  return paraESP.getUInt("DHT_PUT");
}

// WAIT
void makeWait(unsigned long delayTime){
  delay(delayTime /3);tft.printf(".");Serial.print(".");
  delay(delayTime /3);tft.printf(".");Serial.println(".");
  delay(delayTime /3);
}
void rebootEspWithReason(String reason){
    Serial.println(reason);
    delay(1000);
    ESP.restart();
}


#endif // __INIT_H_
