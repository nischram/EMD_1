#ifndef __OTA_H_
#define __OTA_H_

#ifdef OAT_USE
  #include <ArduinoOTA.h>
  
  void ArduinoSetOTA(){
    
    ArduinoOTA.setHostname(OTA_HOSTNAME);
  
    ArduinoOTA.onStart([]() {
      drawScreenOTA();
      printLcdText(80,210, ILI9341_BLACK, FontSansSerif_plain_11,"%s", OTA_HOSTNAME);

      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_SPIFFS
        type = "filesystem";
      }
  
      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      overwriteLcdText(80, 270, 140, 14, ILI9341_BLACK, ILI9341_WHITE, FontSansSerif_plain_11,"Restart ESP32\r");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      overwriteLcdText(75, 230, 140, 14, ILI9341_BLACK, ILI9341_WHITE, FontSansSerif_plain_11,"Progress: %3u%%\r", (progress / (total / 100)));
      tft.drawRGBBitmap(170, 221, percent,9,9);
      overwriteLcdText(45, 250, 180, 14, ILI9341_BLACK, ILI9341_WHITE, FontSansSerif_plain_11,"Size:%5u of%5u kB\r", progress/1024, total/1024);
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
  
  }
  void ArduinoOTAinit(){
      ArduinoSetOTA();
      ArduinoOTA.begin();
      tftPrintInit("ESP OTA-Name   : %s", OTA_HOSTNAME);
  
  }
  #endif // OAT_USE
#endif // __OTA_H_
