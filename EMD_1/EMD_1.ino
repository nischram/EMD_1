#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <TimeLib.h>

#define SW_VERSION                  1.07
#define SW_DATE                    "02.01.2023"
#define TYPE                       "EMD-1 : "

#include "parameter.h"
#include "define.h"

// init Preferences
#include <Preferences.h>
Preferences paraWifi, paraESP;
void initPreferences(){
  paraESP.begin("ESP");
}

// Variable
int screenChoose = 1, screenActive = SCREEN_AKTUEL, mainScreen;
bool drawScreen = NEW, drawContent = NEW, drawButton = NEW;
bool enter = false, firstBoot = true;
bool wifiTimeout = false, noDeepSleep = false, screenSaveActiv = false;
int ret, delayTime = 20;

#include "display.h"
#include "init.h"
#include "ntp.h"
#include "update.h"
#include "weatherGui.h"
#ifdef HM_USE
  #include "homematic.h"
#endif
#include "modbus.h"
#include "funktion.h"
#ifdef OAT_USE
  #include "ota.h"
#endif
#ifdef DHT_USE
  #include "dhttemp.h"
#endif


void setup() {
  serialBegin();
  serialPrintSW();
  initPreferences();
  initTouch();
  initWifi();
  initNTP();
  #ifdef OAT_USE
    ArduinoOTAinit();
  #endif
  initSD_Card();
  if (cardin){
    checkFolder("/update");
  }
  rebootCounter();
  initPIR();
  #ifdef DHT_USE
    initDHT();
    serialPrintDHT();
  #endif
  initModbus(IP_E3DC);
  firstReadMagicByte(mbIP_E3DC);
  if(wifiTimeout == true || modbusTimeout == true)screenActive = SCREEN_REFRESH;
  readPIR();
  Serial.printf("Screen Save Time :  %i\n", readScreenSave());
  lastScreenMillis = millis();
  delay(4000);
  firstBoot = false;
}
void loop() {

  if(checkTouch())checkStartScreen();

  switch (screenActive) { // switch for main screen
    case SCREEN_MAIN: {
        drawScreenMain();
        if (Touch_pressed == true) {
          if(touchField(Main_R1_S1)) screenActive = SCREEN_AKTUEL;
          if(touchField(Main_R1_S2)) screenActive = SCREEN_PV;
          if(touchField(Main_R2_S1)) screenActive = SCREEN_WETTER;
          if(touchField(Main_R2_S2)) screenActive = SCREEN_EINST;
          if(touchField(Main_R3_S1)) screenActive = SCREEN_UPDATE;
          if(touchField(Main_R3_S2)) screenActive = SCREEN_REFRESH;
          touchCheck();
        }
        break; // case SCREEN_MAIN
      }
    case SCREEN_SECOND: {
        drawScreenSecond();
        if (Touch_pressed == true) {
          if(touchField(Main_R1_S1)) screenActive = SCREEN_AKTUEL;
          if(touchField(Main_R1_S2)) screenActive = SCREEN_PV;
          if(touchField(Main_R2_S1)) screenActive = SCREEN_UPDATE;
          if(touchField(Main_R2_S2)) screenActive = SCREEN_EINST;
          if(touchField(Main_R3_S1)) screenActive = SCREEN_AKTUEL;
          if(touchField(Main_R3_S2)) screenActive = SCREEN_MAIN;
          touchCheck();
        }
        break; // case SCREEN_SECOND
      }
    case SCREEN_AKTUEL: {
        drawScreenAktuel();
        if (!modbusReady){
          initModbus(IP_E3DC);
        }
        if (Touch_pressed == true) {
          if (checkBackToMain()) break;
          if(touchField(Pos_Sym_PV)) {
            drawScreen = NEW;
            screenActive = SCREEN_PV;
          }
          if(touchField(Pos_Wetter)) {
            drawScreen = NEW;
            screenActive = SCREEN_WETTER;
          }
        }
        if ((millis() - lastMbMillis > IntervalModbus * 1000) || lastMbMillis == 0) {
          lastMbMillis = millis();
          mainMbRead();
          checkMb();
        }
        #ifdef DHT_USE
          if ((millis() - lastDhtMillis > DHT_INTERVALL * 1000) || lastDhtMillis == 0) {
            lastDhtMillis = millis();
            readDHT();
          }
        #endif
        if (millis() - printMillis > 1000 || printMillis == 0){
          printMillis = millis();
          #ifdef DEBUG
            overwriteLcdText(100, 269, 45, 11, ILI9341_DARKGREY, ILI9341_YELLOW, FontMonospaced_bold_13,"%5d",readRebootCounter());
            overwriteLcdText(100, 280, 45, 11, ILI9341_DARKGREY, ILI9341_YELLOW, FontMonospaced_bold_13,"%5d",mbDebugCounter);
          #endif
          overwriteLcdTextWorth(6, 93, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"W", "%6d",solarPower);
          overwriteLcdTextWorth(148, 93, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"W", "%6d",gridPower);
          overwriteLcdTextWorth(6, 284, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"W", "%6d",batPower);
          overwriteLcdTextWorth(148, 284, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"W", "%6d",homePower);
          overwriteLcdText(6, 210, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16, "%5d",batSoc);
          tft.drawRGBBitmap(65, 198, percent16,16,13);
          tftPercentRect(34, 218, 30, 46, ILI9341_BLUE, ILI9341_WHITE, batSoc);
          tftPercentRect(92, 111, 22, 66, ILI9341_DARKGREY, ILI9341_WHITE, eigenverbrauch);
          tftPercentRect(126, 111, 22, 66, ILI9341_DARKGREEN, ILI9341_WHITE, autarkie);
          #ifdef EXT_LM_USE
            overwriteLcdTextWorth(6, 194, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"W","%6d",extPower);
          #endif
          #ifdef EXT_WB_USE
            overwriteLcdTextWorth(148, 194, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"W","%6d",wbAllPower);
            if(wbSolarPower > 30)
              overwriteLcdTextWorth(148, 208, 82, 12, ILI9341_ORANGE, ILI9341_WHITE, FontMonospaced_bold_16,"W","%6d",wbSolarPower);
            if      ((wbCtrl & WB_CONNECT) == WB_CONNECT)tft.drawRGBBitmap(168, 145, carConnect,52,28);
            else if ((wbCtrl & WB_LOCKED) == WB_LOCKED)tft.drawRGBBitmap(168, 145, carLocked,52,28);
            else if ((wbCtrl & WB_CHARGE) == WB_CHARGE){
              if      (wbAllPower <= 200 && wbSolarPower >=200)tft.drawRGBBitmap(168, 145, carSun,52,28);
              else if (wbAllPower >= 200 && wbSolarPower <=200)tft.drawRGBBitmap(168, 145, carGridRed,52,28);
              else tft.drawRGBBitmap(168, 145, carMix,52,28);
            }
            else{
              tft.drawRGBBitmap(168, 145, car,52,28);
            }
            if ((wbCtrl & WB_MODE) == WB_MODE)tft.fillRect(212,145,8,5, ILI9341_ORANGE);
            else if ((wbCtrl & WB_ERR) == WB_ERR)tft.fillRect(212,145,8,5, ILI9341_RED);
            else tft.fillRect(212,145,8,5, ILI9341_BLUE);

          #endif
          #ifdef DHT_USE
            if (readDhtPut() == 0){
              tft.fillRect(90,232,60,32, ILI9341_LIGHTGREY);
              tft.fillRect(92,234,56,28, ILI9341_WHITE);
              overwriteLcdText(98, 245, 45, 11, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.1f",temp);
              tft.drawRGBBitmap(135, 236, celsius,11,9);
              overwriteLcdText(98, 260, 45, 11, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.0f",hum);
              tft.drawRGBBitmap(136, 251, percent,9,9);
            }
          #endif
          overwriteLcdText(60, 304, 140, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%s %s", datum, zeit);
          drawWeatherSingleIcon();
        }
       break; // case SCREEN_AKTUEL
      }
    case SCREEN_PV: {
        drawScreenPV();
        if (!modbusReady){
          initModbus(IP_E3DC);
        }
        if (Touch_pressed == true) {
          if (checkBackToMain()) break;
        }
        if ((millis() - lastMbMillis > IntervalModbus * 1000) || lastMbMillis == 0 ) {
          lastMbMillis = millis();
          pvMbRead();
        }
        if (millis() - printMillis > 1000 || printMillis == 0){
          printMillis = millis();
          overwriteLcdText(80, 117, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"%6d W",solarPower);
          overwriteLcdText(20, 213, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"%5d V",pvU1);
          overwriteLcdText(145, 213, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"%5d V",pvU2);
          overwriteLcdText(20, 233, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"%5.2f A",pvI1 * 0.01);
          overwriteLcdText(145, 233, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"%5.2f A",pvI2 * 0.01);
          overwriteLcdText(20, 253, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"%5d W",pvP1);
          overwriteLcdText(145, 253, 82, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_16,"%5d W",pvP2);
          overwriteLcdText(60, 304, 140, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%s %s", datum, zeit);
        }
        break; // case SCREEN_KABLE_INFO
      }
    case SCREEN_WETTER: {
        drawScreenWetter();
        if (!weatherCallReady){
          displayWeather();
        }
        if (Touch_pressed == true) {
          if (checkBackToMain()) break;
          if(touchField(Pos_newCall)){
            weatherCallReady = false;
            drawScreen = NEW;
          }
        }
        if (millis() - weatherMillis > WEATHER_INTERVALL *60*1000){
          weatherMillis = millis();
          weatherCallReady = false;
          drawScreen = NEW;
        }
        if (millis() - printMillis > 1000 || printMillis == 0){
          printMillis = millis();
          serialPrintClock();
          overwriteLcdText(60, 300, 140, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%s %s", datum, zeit);
        }
       break; // case SCREEN_WETTER
      }
    case SCREEN_EINST: {
        drawScreenEinst();
        if (Touch_pressed == true) {
          if (checkBackToMain()) break;
          //Zeiteinstellung
          if(touchField(Small_R2_S2)) {
            changeRotation();
            tft.fillRect(106, 76, 122, 40, ILI9341_RED);
            if (readRotation() == 2) printLcdText(114, 93, ILI9341_WHITE, FontSansSerif_plain_11,"Set Rotation:180");
            else printLcdText(114, 93, ILI9341_WHITE, FontSansSerif_plain_11,"Set Rotation: 0");
            printLcdText(114, 105, ILI9341_WHITE, FontSansSerif_plain_11,"Restart ESP");
             delay(800);
             ESP.restart();
          }
          if(touchField(Small_R3_S1) || touchField(Small_R3_S2)) {
             timeDiff = changeTimeDiff();
             drawContent = NEW;
             delay(600);
          }
          //Lichtsensor
          if(touchField(Small_R4_S1) || touchField(Small_R4_S2)) {
             pirUse = changePIR();
             if(pirUse == 1)initPIR();
             drawContent = NEW;
             delay(600);
          }
          //Bilschirmschoner +
          if(touchField(Small_R5_S1)) {
             screenSaveTime = writeScreenSave(screenSaveTime - 1);
             drawContent = NEW;
             delay(600);
          }
          //Bilschirmschoner -
          if(touchField(Small_R5_S2)) {
             screenSaveTime = writeScreenSave(screenSaveTime + 1);
             drawContent = NEW;
             delay(600);
          }
          //DHT auf Hauptschirm
          if(touchField(Small_R6_S1) || touchField(Small_R6_S2)) {
             changeDhtPut();
             drawContent = NEW;
             delay(600);
          }
        }
        if(!drawContent){
          drawContent = READY;
          tft.fillRect(106, 76, 122, 40, ILI9341_WHITE);
          printLcdText(114, 93, ILI9341_DARKGREY, FontSansSerif_plain_11,"Screen Rotation");
          if (readRotation() == 2) printLcdText(114, 105, ILI9341_DARKGREY, FontSansSerif_plain_11,"180 Grad");
          else printLcdText(114, 105, ILI9341_DARKGREY, FontSansSerif_plain_11,"0 Grad");
          tft.fillRect(106, 124, 122, 40, ILI9341_WHITE);
          printLcdText(114, 141, ILI9341_DARKGREY, FontSansSerif_plain_11,"Zeiteinstellung");
          if(timeDiff == 7200)printLcdText(120, 159, ILI9341_DARKGREY, FontSansSerif_plain_11,"Sommer Aktiv");
          if(timeDiff == 3600)printLcdText(120, 159, ILI9341_DARKGREY, FontSansSerif_plain_11,"Winter Aktiv");
          tft.fillRect(106, 172, 122, 40, ILI9341_WHITE);
          printLcdText(114, 189, ILI9341_DARKGREY, FontSansSerif_plain_11,"Lichtsensor");
          if(readPIRuse() == 1)printLcdText(114, 201, ILI9341_DARKGREY, FontSansSerif_plain_11,"PIR Aktiv");
          else printLcdText(114, 201, ILI9341_DARKGREY, FontSansSerif_plain_11,"PIR Deaktiv");
          tft.fillRect(106, 220, 122, 40, ILI9341_WHITE);
          printLcdText(114, 232, ILI9341_DARKGREY, FontSansSerif_plain_11,"Bildschirm-");
          printLcdText(114, 244, ILI9341_DARKGREY, FontSansSerif_plain_11,"schoner");
          if(readScreenSave() < 1)printLcdText(114, 256, ILI9341_DARKGREY, FontSansSerif_plain_11,"Deaktiv");
          else printLcdText(114, 256, ILI9341_DARKGREY, FontSansSerif_plain_11,"%i Minuten", readScreenSave());
          tft.fillRect(106, 268, 122, 40, ILI9341_WHITE);
          printLcdText(114, 280, ILI9341_DARKGREY, FontSansSerif_plain_11,"DHT on Screen");
          if(readDhtPut() == 0)printLcdText(114, 292, ILI9341_DARKGREY, FontSansSerif_plain_11,"Aktiv");
          else printLcdText(114, 292, ILI9341_DARKGREY, FontSansSerif_plain_11,"Deaktiv");
        }
        break; // case SCREEN_EINST
      }
    case SCREEN_REFRESH: {
        drawScreenRefresh();
        if (Touch_pressed == true) {
          if (checkBackToMain()) break;
          //ESP restart
          if(touchField(Small_R3_S1)) {
             tft.drawRect(touchXmin[Small_R3_S1]-1, touchYmin[Small_R3_S1]-1, 42, 42, ILI9341_GREEN);
             tft.drawRect(touchXmin[Small_R3_S1], touchYmin[Small_R3_S1], 40, 40, ILI9341_GREEN);
             delay(400);
             ESP.restart();
          }
          //Wifi restart
          if(touchField(Small_R4_S1)) {
             tft.drawRect(touchXmin[Small_R4_S1]-1, touchYmin[Small_R4_S1]-1, 42, 42, ILI9341_GREEN);
             tft.drawRect(touchXmin[Small_R4_S1], touchYmin[Small_R4_S1], 40, 40, ILI9341_GREEN);
             delay(400);
             closeModbus(mbIP_E3DC);
             mbSymbol(POS_WWW, 2);
             WiFi.disconnect();
             wifiTimeout = true;
             wwwSymbol(POS_MB, 2);
             delay(1000);
             startWifi();
             wwwSymbol(POS_MB, 2);
             initModbus(IP_E3DC);
             mbSymbol(POS_WWW, 2);
             drawScreen = NEW;
        }
          //Modbus restart
          if(touchField(Small_R5_S1)) {
             tft.drawRect(touchXmin[Small_R5_S1]-1, touchYmin[Small_R5_S1]-1, 42, 42, ILI9341_GREEN);
             tft.drawRect(touchXmin[Small_R5_S1], touchYmin[Small_R5_S1], 40, 40, ILI9341_GREEN);
             delay(400);
             closeModbus(mbIP_E3DC);
             mbSymbol(POS_WWW, 2);
             delay(1000);
             initModbus(IP_E3DC);
             mbSymbol(POS_WWW, 2);
             drawScreen = NEW;
          }
          //SD-Karte restart
          if(touchField(Small_R6_S1)) {
             tft.drawRect(touchXmin[Small_R6_S1]-1, touchYmin[Small_R6_S1]-1, 42, 42, ILI9341_GREEN);
             tft.drawRect(touchXmin[Small_R6_S1], touchYmin[Small_R6_S1], 40, 40, ILI9341_GREEN);
             tft.fillRect(POS_SD, 2, 16, 16, ILI9341_DARKGREY);
             delay(600);
             initSD_Card();
             drawScreen = NEW;
          }
        }
        if(!drawContent){
          drawContent = READY;
          tft.fillRect(touchXmin[Small_R3_S2], touchYmin[Small_R3_S2], 168, 40, ILI9341_WHITE);
          printLcdText(touchXmin[Small_R3_S2]+8, touchYmin[Small_R3_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11,"System Neustart ");
          tft.fillRect(touchXmin[Small_R4_S2], touchYmin[Small_R4_S2], 168, 40, ILI9341_WHITE);
          printLcdText(touchXmin[Small_R4_S2]+8, touchYmin[Small_R4_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11,"W-Lan Neustart");
          tft.fillRect(touchXmin[Small_R5_S2], touchYmin[Small_R5_S2], 168, 40, ILI9341_WHITE);
          printLcdText(touchXmin[Small_R5_S2]+8, touchYmin[Small_R5_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11,"Modbus Neustart");
          tft.fillRect(touchXmin[Small_R6_S2], touchYmin[Small_R6_S2], 168, 40, ILI9341_WHITE);
          printLcdText(touchXmin[Small_R6_S2]+8, touchYmin[Small_R6_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11,"SD-Karte Refresh");
        }
        break; // case SCREEN_REFRESH
      }
    case SCREEN_UPDATE: {
      bool afterUpdate = true;
        if (Touch_pressed == true) {
          if (checkBackToMain()) break;
          //SD-Karte restart
          if(touchField(Small_R3_S1)) {
             tft.drawRect(touchXmin[Small_R3_S1]-1, touchYmin[Small_R3_S1]-1, 42, 42, ILI9341_GREEN);
             tft.drawRect(touchXmin[Small_R3_S1], touchYmin[Small_R3_S1], 40, 40, ILI9341_GREEN);
             tft.fillRect(POS_SD, 2, 16, 16, ILI9341_DARKGREY);
             delay(600);
             initSD_Card();
             drawScreen = NEW;
          }

          if (touchField(Main_R2_S1) && findUpdate != 0) {
            tft.fillRect(touchXmin[Small_R3_S2], touchYmin[Small_R3_S2], 168, 88, ILI9341_WHITE);
            printLcdText(touchXmin[Small_R3_S2]+8, touchYmin[Small_R3_S2]+30, ILI9341_DARKGREY, FontSansSerif_plain_11, "start...");
            printLcdText(touchXmin[Small_R3_S2]+8, touchYmin[Small_R3_S2]+55, ILI9341_DARKGREY, FontSansSerif_plain_11, "Bitte warten!");
            updateFromFS(SD_MMC, updateFile);
            afterUpdate = NEW;
            drawScreen = NEW;
          }
        }
        drawScreenUpdate();
        if (!drawContent){
          drawContent = READY;
          if (!checkSdCard()){
            tft.drawRect(touchXmin[Small_R2_S2]-1, touchYmin[Small_R2_S2]-1, 170, 42, ILI9341_RED);
            tft.fillRect(touchXmin[Small_R2_S2], touchYmin[Small_R2_S2], 168, 40, ILI9341_WHITE);
            printLcdText(touchXmin[Small_R2_S2]+8, touchYmin[Small_R2_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, "Keine SD erkannt!");
          }
          else {
            checkUpdateBin(updateFile);
            if (findUpdate != 0){
              tft.drawRGBBitmap(120,140, menuEnter,80,80);
              tft.drawRect(touchXmin[Small_R2_S2]-1, touchYmin[Small_R2_S2]-1, 170, 42, ILI9341_GREEN);
              tft.fillRect(touchXmin[Small_R2_S2], touchYmin[Small_R2_S2], 168, 40, ILI9341_WHITE);
              printLcdText(touchXmin[Small_R2_S2]+8, touchYmin[Small_R2_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, updateInfo1);
            }
            else {
              tft.drawRect(touchXmin[Small_R2_S2]-1, touchYmin[Small_R2_S2]-1, 170, 42, ILI9341_RED);
              tft.fillRect(touchXmin[Small_R2_S2], touchYmin[Small_R2_S2], 168, 40, ILI9341_WHITE);
              printLcdText(touchXmin[Small_R2_S2]+8, touchYmin[Small_R2_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, "keine Datei gefunden");
              tft.fillRect(touchXmin[Small_R3_S2], touchYmin[Small_R3_S2], 168, 40, ILI9341_WHITE);
              printLcdText(touchXmin[Small_R3_S2]+8, touchYmin[Small_R3_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, "Beispiel:");
              tft.fillRect(touchXmin[Small_R4_S2], touchYmin[Small_R4_S2], 168, 40, ILI9341_WHITE);
              printLcdText(touchXmin[Small_R4_S2]+8, touchYmin[Small_R4_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, "/update/EMD_1-08.bin");
            }
          }
        }
        if (!afterUpdate){
          afterUpdate = READY;
          if (returnUpdate == 1){
            tft.drawRect(touchXmin[Small_R2_S2]-1, touchYmin[Small_R2_S2]-1, 170, 42, ILI9341_GREEN);
            tft.fillRect(touchXmin[Small_R2_S2], touchYmin[Small_R2_S2], 168, 40, ILI9341_WHITE);
            printLcdText(touchXmin[Small_R2_S2]+8, touchYmin[Small_R2_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, updateInfo1);
            tft.fillRect(touchXmin[Small_R3_S2], touchYmin[Small_R3_S2], 168, 40, ILI9341_GREEN);
            printLcdText(touchXmin[Small_R3_S2]+8, touchYmin[Small_R3_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, updateInfo2);
            tft.fillRect(touchXmin[Small_R4_S2], touchYmin[Small_R4_S2], 168, 40, ILI9341_WHITE);
            printLcdText(touchXmin[Small_R4_S2]+8, touchYmin[Small_R4_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, "EMD_1 reboot now!");
            delay(3000);
            rebootEspWithReason("Update successfully reboot now!");
          }
          else if (returnUpdate == -1){
            tft.drawRect(touchXmin[Small_R2_S2]-1, touchYmin[Small_R2_S2]-1, 170, 42, ILI9341_RED);
            tft.fillRect(touchXmin[Small_R2_S2], touchYmin[Small_R2_S2], 168, 40, ILI9341_WHITE);
            printLcdText(touchXmin[Small_R2_S2]+8, touchYmin[Small_R2_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, updateInfo1);
            tft.fillRect(touchXmin[Small_R3_S2], touchYmin[Small_R3_S2], 168, 40, ILI9341_RED);
            printLcdText(touchXmin[Small_R3_S2]+8, touchYmin[Small_R3_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, updateInfo2);
          }
          else {
            tft.drawRect(touchXmin[Small_R2_S2]-1, touchYmin[Small_R2_S2]-1, 170, 42, ILI9341_RED);
            tft.fillRect(touchXmin[Small_R2_S2], touchYmin[Small_R2_S2], 168, 40, ILI9341_WHITE);
            printLcdText(touchXmin[Small_R2_S2]+8, touchYmin[Small_R2_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, updateInfo1);
            tft.fillRect(touchXmin[Small_R3_S2], touchYmin[Small_R3_S2], 168, 40, ILI9341_RED);
            printLcdText(touchXmin[Small_R3_S2]+8, touchYmin[Small_R3_S2]+25, ILI9341_DARKGREY, FontSansSerif_plain_11, "unerwarteter Fehler!");
          }
        }
        break; // case SCREEN_UPDATE
      }
    case SCREEN_SAVE: {
       /* if ((millis() - lastMbMillis > IntervalModbus * 1000 * 5) || lastMbMillis == 0) {
          lastMbMillis = millis();
          mainMbRead();
          checkMb();
        }*/
        if(modbusReady){
          closeModbus(mbIP_E3DC);
        }
        drawScreenSave();
        break; // case SCREEN_THIRD
      }
  } // switch(screenActive)

  #ifdef HM_USE   //Senden der Werte an die Homemati
    if ((millis() - lastHmMillis > INTERVALL_HM * 1000) || lastHmMillis == 0) {
      lastHmMillis = millis();
      if (!modbusReady){
        initModbus(IP_E3DC);
      }
      mainMbRead();
      sendHM(PRINT, ISE_SEND_AKTIV, "Aktiv", "true");
      sendHM(NO_PRINT, ISE_SEND_WIFI, "Wifi", "%i", WiFi.RSSI());
      sendHM(NO_PRINT, ISE_S10_GRID, "S10_Grid", "%i", gridPower);
      sendHM(NO_PRINT, ISE_S10_BAT, "S10_Bat", "%i", batPower);
      sendHM(NO_PRINT, ISE_S10_SOLAR, "S10_Solar", "%i", solarPower);
      sendHM(NO_PRINT, ISE_S10_CON, "S10_Home", "%i", homePower);
      sendHM(NO_PRINT, ISE_S10_BATSOC, "S10_SOC", "%i", batSoc);
      sendHM(NO_PRINT, ISE_S10_AUTARKIE, "S10_Autarkie", "%i", autarkie);
      sendHM(NO_PRINT, ISE_S10_EIGEN, "S10_Eigenverbrauch", "%i", eigenverbrauch);
    }
  #endif

  // Screen Save
  if(digitalRead(PIR_PIN) == HIGH) {
    checkStartScreen();
  }
  else if (screenSaveTime > 0){
    if ((millis() - lastScreenMillis > screenSaveTime * 60  * 1000) && !screenSaveActiv){
      screenSaveActiv = ON;
      screenActive = SCREEN_SAVE;
      drawScreen = NEW;
      Serial.println("Screen Save      :  Aktiv");
    }
  }

  #ifdef OAT_USE
    if (!wifiTimeout) ArduinoOTA.handle();
  #endif

  delay(delayTime);
}
