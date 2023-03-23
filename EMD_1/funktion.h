#ifndef __FUNKTION_H_
#define __FUNKTION_H_

// SD-Card Symbol
void sdSymbol(int x, int y, char const * folder) {
  if (SD.exists(folder)) {
    cardin = true;
    tft.drawRGBBitmap(x,y, sdOk,16,16);
  }
  else {
    cardin = false;
    tft.drawRGBBitmap(x,y, sdNOk,16,16);
  }
}
// www Symbol
void wwwSymbol(int x, int y) {
  if (!wifiTimeout) {
    tft.drawRGBBitmap(x,y, wwwOk,16,16);
  }
  else {
    tft.drawRGBBitmap(x,y, wwwNOk,16,16);
  }
}
// Modbus Symbol
void mbSymbol(int x, int y) {
  if (!modbusTimeout) {
    tft.drawRGBBitmap(x,y, MbOk,16,16);
  }
  else {
    tft.drawRGBBitmap(x,y, MbNOk,16,16);
  }
}

void drawNew(){
    drawButton  = NEW;
    drawScreen  = NEW;
    drawContent = NEW;
    delay(500);
}
void drawMainScreen(){
    tft.fillRect(0, 0, 240, 320, ILI9341_DARKGREY);
    printLcdText(6, 14, ILI9341_LIGHTGREY, FontMonospaced_bold_13,"EMD-%.2f ", SW_VERSION);
    tft.fillRect(6, 20, 228, 294, ILI9341_LIGHTGREY);

    //check SD, WWW, MB and make symbol
    sdSymbol(POS_SD, 2, "/update");
    wwwSymbol(POS_MB, 2);
    mbSymbol(POS_WWW, 2);
}
void drawScreenMain(){
    if(!drawScreen){
      delayTime = 20;
      drawMainScreen();
      Serial.println("Display Menü       :  Main");
      drawBitmapRGB(Main_R1_S1, menuLeistung,80,80);
      drawBitmapRGB(Main_R1_S2, menuPV,80,80);
      drawBitmapRGB(Main_R2_S1, menuWetter,80,80);
      drawBitmapRGB(Main_R2_S2, menuEinst,80,80);
      drawBitmapRGB(Main_R3_S1, menuUpdate,80,80);
      drawBitmapRGB(Main_R3_S2, menuRefresh,80,80);
      drawScreen = READY;
      mainScreen = SCREEN_MAIN;
      delay(200);
    }
}
void drawScreenSecond(){
    if(!drawScreen){
      delayTime = 20;
      drawMainScreen();
      Serial.println("Display Menü       :  Second");
      drawBitmapRGB(Main_R1_S1, menuFree,80,80);
      drawBitmapRGB(Main_R1_S2, menuFree,80,80);
      drawBitmapRGB(Main_R2_S1, menuFree,80,80);
      drawBitmapRGB(Main_R2_S2, menuBack,80,80);
      drawBitmapRGB(Main_R3_S1, menuFree,80,80);
      drawBitmapRGB(Main_R3_S2, menuNext,80,80);
      drawScreen = READY;
      mainScreen = SCREEN_SECOND;
      delay(200);
    }
}
void drawScreenAktuel(){
    if(!drawScreen){
      Serial.println("Display Screen     :  Aktuell");
      tft.fillRect(0, 0, 240, 320, ILI9341_DARKGREY);
      tft.fillRect(6, 6, 228, 308, ILI9341_WHITE);
      drawBitmapRGB(Pos_Sym_PV, Sym_PV,60,60);
      drawBitmapRGB(Pos_Sym_Grid, Sym_Grid,60,60);
      tft.fillRect(30, 214, 38, 54, ILI9341_DARKGREY);
      tft.fillRect(34, 218, 30, 46, ILI9341_WHITE);
      drawBitmapRGB(Pos_Sym_Home, Sym_Home,60,60);
      #ifdef EXT_LM_USE
        drawBitmapRGB(Pos_Sym_Ext, Sym_Ext,52,52);
      #endif
      drawBitmapRGB(Pos_Sym_E3DC, Sym_E3DC,80,80);
      drawBitmapRGB(Pos_Sym_Back, Sym_Back,32,21);
      weatherMillis = 0;
      drawScreen = READY;
      delay(200);
    }
}
void drawScreenPV(){
    if(!drawScreen){
      drawMainScreen();
      Serial.println("Display Screen     :  PV");
      tft.fillRect(6, 20, 228, 294, ILI9341_WHITE);
      drawBitmapRGB(Pos_PV_Mitte, Sym_PV,60,60);
      drawBitmapRGB(Pos_Modul_1, PV_Modul,60,52);
      drawBitmapRGB(Pos_Modul_2, PV_Modul,60,52);
      drawBitmapRGB(Pos_Sym_Back, Sym_Back,32,21);
      drawBitmapRGB(Pos_Sym_Back, Sym_Back,32,21);
      drawScreen = READY;
      delay(200);
    }
}
void drawScreenWetter(){
    if(!drawScreen){
      Serial.println("Display Screen     :  Wetter");
      tft.fillRect(0, 0, 240, 320, ILI9341_DARKGREY);
      tft.fillRect(6, 6, 228, 308, ILI9341_WHITE);
      drawBitmapRGB(Pos_Sym_Back, Sym_Back,32,21);
      drawBitmapRGB(Pos_newCall, newCall,32,21);
      drawScreen = READY;
      weatherCallReady = false;
      delay(200);
    }
}
void drawScreenEinst(){
    if(!drawScreen){
      drawMainScreen();
      Serial.println("Display Menü       :  Einstellungen");
      tft.drawRect(touchXmin[Small_R1_S1]-1, touchYmin[Small_R1_S1]-1, 42, 42, ILI9341_GREEN);
      tft.drawRect(touchXmin[Small_R1_S2]-1, touchYmin[Small_R1_S2]-1, 170, 42, ILI9341_GREEN);
      tft.fillRect(touchXmin[Small_R1_S2], touchYmin[Small_R1_S2], 168, 40, ILI9341_WHITE);
      printLcdText(touchXmin[Small_R1_S2]+8, touchYmin[Small_R1_S2]+25, ILI9341_DARKGREY, FontSansBold9pt7b,"Einstellungen");
      drawBitmapRGB(Small_Menue, menuEinstSmall,40,40);
      drawBitmapRGB(Small_Back, menuBackSmall,40,40);
      drawBitmapRGB(Small_R2_S2, screenRotation,40,40);
      drawBitmapRGB(Small_R3_S2, menuSommerSmall,40,40);
      drawBitmapRGB(Small_R3_S1, menuWinterSmall,40,40);
      drawBitmapRGB(Small_R4_S2, menuOkSmall,40,40);
      drawBitmapRGB(Small_R4_S1, menuNOkSmall,40,40);
      drawBitmapRGB(Small_R5_S2, menuPlusSmall,40,40);
      drawBitmapRGB(Small_R5_S1, menuMinusSmall,40,40);
      drawBitmapRGB(Small_R6_S2, Sym_Temp,40,40);
      drawBitmapRGB(Small_R6_S1, Sym_TempOff,40,40);
      drawScreen = READY;
      delay(200);
    }
}
void drawScreenRefresh(){
    if(!drawScreen){
      drawMainScreen();
      Serial.println("Display Menü       :  Refresh");
      tft.drawRect(touchXmin[Small_R1_S1]-1, touchYmin[Small_R1_S1]-1, 42, 42, ILI9341_GREEN);
      tft.drawRect(touchXmin[Small_R1_S2]-1, touchYmin[Small_R1_S2]-1, 170, 42, ILI9341_GREEN);
      tft.fillRect(touchXmin[Small_R1_S2], touchYmin[Small_R1_S2], 168, 40, ILI9341_WHITE);
      printLcdText(touchXmin[Small_R1_S2]+8, touchYmin[Small_R1_S2]+25, ILI9341_DARKGREY, FontSansBold9pt7b,"Neustarten");
      drawBitmapRGB(Small_Menue, menuRefreshSmall,40,40);
      drawBitmapRGB(Small_Back, menuBackSmall,40,40);
      drawBitmapRGB(Small_R3_S1, refreshESP,40,40);
      drawBitmapRGB(Small_R4_S1, refreshWifi,40,40);
      drawBitmapRGB(Small_R5_S1, refreshModbus,40,40);
      drawBitmapRGB(Small_R6_S1, refreshSD,40,40);
      drawScreen = READY;
      drawContent = NEW;
      delay(200);
    }
}
void drawScreenUpdate(){
    if(!drawScreen){
      drawMainScreen();
      Serial.println("Display Menü       :  Update");
      tft.drawRect(touchXmin[Small_R1_S1]-1, touchYmin[Small_R1_S1]-1, 42, 42, ILI9341_GREEN);
      tft.drawRect(touchXmin[Small_R1_S2]-1, touchYmin[Small_R1_S2]-1, 170, 42, ILI9341_GREEN);
      tft.fillRect(touchXmin[Small_R1_S2], touchYmin[Small_R1_S2], 168, 40, ILI9341_WHITE);
      printLcdText(touchXmin[Small_R1_S2]+8, touchYmin[Small_R1_S2]+25, ILI9341_DARKGREY, FontSansBold9pt7b,"Update EMD -1");
      drawBitmapRGB(Small_Menue, menuUpdateSmall,40,40);
      drawBitmapRGB(Small_Back, menuBackSmall,40,40);
      drawBitmapRGB(Small_R3_S1, refreshSD,40,40);
      drawScreen = READY;
      delay(200);
    }
}
void drawScreenSave(){
    if(!drawScreen){
      backlightOFF();
      drawMainScreen();
      Serial.println("Display Menü       :  Screensaver");
      tft.fillRect(0, 0, 240, 320, ILI9341_DARKGREY);
      tft.fillRect(6, 6, 228, 308, ILI9341_LIGHTGREY);
      tft.fillRect(12, 12, 216, 296, ILI9341_WHITE);
      drawBitmapRGB(Pos_Sym_PV, Sym_PV,60,60);
      drawBitmapRGB(Pos_Sym_Grid, Sym_Grid,60,60);
      drawBitmapRGB(Pos_Sym_Bat, Sym_Bat,60,60);
      drawBitmapRGB(Pos_Sym_Home, Sym_Home,60,60);
      drawBitmapRGB(Pos_Sym_E3DC, Sym_E3DC,80,80);
      drawBitmapRGB(Pos_Sym_Back, Sym_Back,32,21);
      drawScreen = READY;
      delay(200);
    }
}
void drawScreenOTA(){
      Serial.println("Display Screen     :  OTA");
      tft.fillRect(0, 0, 240, 320, ILI9341_DARKGREY);
      tft.fillRect(6, 6, 228, 308, ILI9341_LIGHTGREY);
      tft.fillRect(12, 12, 216, 296, ILI9341_WHITE);
      tft.drawRGBBitmap(80, 30, Sym_OTA,80,80);
      printLcdText(80,130, ILI9341_BLACK, FontSansSerif_plain_11,"Over-The-Air");
      printLcdText(100,150, ILI9341_BLACK, FontSansSerif_plain_11,"Update");
      drawScreen = READY;
}
void tftPercentRect(int x, int y, int w, int h, int c, int bg, int value){
  tft.fillRect(x, y, w, h, bg);
  int hp = h*value/100;
  tft.fillRect(x, y+h-hp, w, hp, c);
}
void checkStartScreen(){
    lastScreenMillis = millis();
    if(screenSaveActiv){
      screenActive = SCREEN_AKTUEL;
      screenSaveActiv = OFF;
      drawScreen = NEW;
      backlightON();
      Serial.println("Screen Save        :  Deaktiv");
    }
}
#endif // __FUNKTION_H_
