#ifndef __DISPLAY_H_
#define __DISPLAY_H_

/*
 *  Application note: Simple Keylock / Keypad for ArduiTouch and ESP32
 *  Version 1.0
 *  Copyright (C) 2019  Hartmut Wendt  www.zihatec.de
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*______Import Libraries_______*/
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "Fonts/Monospaced_bold_10.h"
#include "Fonts/Monospaced_bold_13.h"
#include "Fonts/Monospaced_bold_16.h"
#include "Fonts/SansSerif_plain_11.h"
#include "graphics.h"
/*______End of Libraries_______*/

/*__Pin definitions for the ESP32__*/
#define TFT_CS           5
#define TFT_DC           4
#define TFT_LED          15
#define TFT_MOSI         23
#define TFT_CLK          18
#define TFT_RST          22
#define TFT_MISO         19

#define HAVE_TOUCHPAD
#define TOUCH_CS         14
//#define TOUCH_IRQ        27         // #define TOUCH_IRQ        2 // Beim alten AZ-Touch für 2,4" GPIO 2 // Bei EMD-Platine auch Pin 2 // Taster zum flaschen nötig auf GPIO2 erforderlich
#define TOUCH_OFFSET_Y   5
#define TOUCH_OFFSET_X   0
/*_______End of definitions______*/

/*____Calibrate Touchscreen_____*/
#define MINPRESSURE 10      // minimum required force for touch event
#define TS_MINX 320
#define TS_MINY 380
#define TS_MAXX 3600
#define TS_MAXY 3600
/*______End of Calibration______*/

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

String symbol[4][4] = {
  { "7", "8", "9" },
  { "4", "5", "6" },
  { "1", "2", "3" },
  { "C", "0", "OK" }
};
int X,Y;
long Num1,Num2,Number;
char action;
boolean result = false;
bool Touch_pressed = false;
TS_Point p;

/********************************************************************//**
 * @brief     detects a touch event and converts touch data
 * @param[in] None
 * @return    boolean (true = touch pressed, false = touch unpressed)
 *********************************************************************/
bool Touch_Event() {
  p = touch.getPoint();
  #ifdef TOUCH_YELLOW_HEADER
    p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0); // yellow header
    p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
  #else
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240); // black header
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
  #endif
  if (p.z > MINPRESSURE) return true;
  return false;
}
// Screen Rotation
int changeRotation(){
  if (paraESP.getUInt("ROTATION") == 0) paraESP.putUInt("ROTATION", 2);
  else paraESP.putUInt("ROTATION", 0);
  return paraESP.getUInt("ROTATION");
}
int readRotation(){
  return paraESP.getUInt("ROTATION");
}


void backlightON(){
    digitalWrite(TFT_LED, LOW);    // LOW to turn backlight on;
}
void backlightOFF(){
    digitalWrite(TFT_LED, HIGH);   // HIGH to turn backlight off;
}

int initPosition = 152;
void tftPrintInit(const char *format, ...){
  va_list argptr;
  char output[128];
  va_start(argptr, format);
  vsprintf(output,  format, argptr);

  if(initPosition < 308){
    tft.setCursor(12, initPosition);
    tft.printf(output);
  }
  initPosition = initPosition + 11;
}
void drawBitmapRGB(int t, const uint16_t bitmap[], int16_t w, int16_t h) {
      tft.drawRGBBitmap(touchXmin[t],touchYmin[t], bitmap,w,h);
}
void initTouch(){
  pinMode(TFT_LED, OUTPUT); // define as output for backlight control
  backlightOFF();
  Serial.print("Init TFT         :  ");
  tft.begin();
  tft.setRotation(readRotation());   // landscape mode
  Serial.print("TFT-X ="); Serial.print(tft.width());
  Serial.print(" TFT-Y ="); Serial.println(tft.height());
  Serial.print("Init Tuoch       :  ");
  touch.begin();
  touch.setRotation(readRotation());
  tft.fillRect(0, 0, 240, 320, ILI9341_DARKGREY);
  tft.drawRGBBitmap(6,6, emdLogo,228,144);
  tft.fillRect(6, 148, 228, 166, ILI9341_WHITE);
  tft.setTextSize(0);
  tft.setTextColor(ILI9341_DARKGREY);
  tft.setFont(&Monospaced_bold_10);
  backlightON();
  delay(200);
  Serial.println("done");
  tftPrintInit("Software       : EMD-%.2f", SW_VERSION);
  tftPrintInit("Software Date  : %s", SW_DATE);
  tftPrintInit("Serial Speed   : %i", SERIALSPEED);

}
void printLcdText(int x, int y, uint16_t c, int font, const char *format, ...) {
  va_list argptr;
  char output[128];
  va_start(argptr, format);
  vsprintf(output,  format, argptr);

  tft.setCursor(x, y);
  tft.setTextColor(c);
  if(font ==FontSans9pt7b) tft.setFont(&FreeSans9pt7b);
  else if(font == FontSansBold9pt7b) tft.setFont(&FreeSansBold9pt7b);
  else if(font == FontMonospaced_bold_10) tft.setFont(&Monospaced_bold_10);
  else if(font == FontMonospaced_bold_13) tft.setFont(&Monospaced_bold_13);
  else if(font == FontMonospaced_bold_16) tft.setFont(&Monospaced_bold_16);
  else if(font == FontSansSerif_plain_11) tft.setFont(&SansSerif_plain_11);
  tft.printf(output);
}
void overwriteLcdText(int x, int y, int w, int h, uint16_t c,uint16_t bg, int font, const char *format, ...) {
  va_list argptr;
  char output[128];
  va_start(argptr, format);
  vsprintf(output,  format, argptr);
  tft.fillRect(x, y-h, w, h, bg);
  printLcdText(x, y, c, font, output);
}
void overwriteLcdTextWorth(int x, int y, int w, int h, uint16_t c,uint16_t bg, int font, char worth[6], const char *format, ...) {
  va_list argptr;
  char output[128];
  va_start(argptr, format);
  vsprintf(output,  format, argptr);
  tft.fillRect(x, y-h, w, h, bg);
  printLcdText(x, y, c, font, output);
  printLcdText(x+w-11, y, c, font, worth);
}
bool checkTouch(){
  if (Touch_Event()== true) {
    X = p.x + TOUCH_OFFSET_X; Y = p.y + TOUCH_OFFSET_Y;
    Touch_pressed = true;
    Serial.print("Y ");Serial.print(Y);Serial.print(" X ");Serial.println(X); //Kontrolle Touch
  }
  else {
    Touch_pressed = false;
  }
  return Touch_pressed;
}
bool touchField(int t){
  if(X > touchXmin[t] && Y > touchYmin[t] && X < touchXmax[t] && Y < touchYmax[t]) {
    delay(200);
    return true;
  }
  else return false;
}
bool checkBackToMain(){
      if(screenActive == SCREEN_PV || screenActive == SCREEN_AKTUEL || screenActive == SCREEN_WETTER ){
          if(touchField(Pos_Sym_Back)) {
            screenActive = mainScreen;
            drawScreen = NEW;
            drawContent = NEW;
            delay(800);
            return true;
          }
      }
      else {
          if(touchField(Small_Back)) {
            screenActive = mainScreen;
            drawScreen = NEW;
            drawContent = NEW;
            delay(800);
            return true;
          }
      }
          return false;
}
void touchCheck(){
  if(touchField(Main_R1_S1) ||
     touchField(Main_R1_S2) ||
     touchField(Main_R2_S1) ||
     touchField(Main_R2_S2) ||
     touchField(Main_R3_S1) ||
     touchField(Main_R3_S2) ){
        drawScreen = NEW;
        drawContent = NEW;
  }
}
/*
// Color definitions
#define ILI9341_BLACK 0x0000       ///<   0,   0,   0
#define ILI9341_NAVY 0x000F        ///<   0,   0, 123
#define ILI9341_DARKGREEN 0x03E0   ///<   0, 125,   0
#define ILI9341_DARKCYAN 0x03EF    ///<   0, 125, 123
#define ILI9341_MAROON 0x7800      ///< 123,   0,   0
#define ILI9341_PURPLE 0x780F      ///< 123,   0, 123
#define ILI9341_OLIVE 0x7BE0       ///< 123, 125,   0
#define ILI9341_LIGHTGREY 0xC618   ///< 198, 195, 198
#define ILI9341_DARKGREY 0x7BEF    ///< 123, 125, 123
#define ILI9341_BLUE 0x001F        ///<   0,   0, 255
#define ILI9341_GREEN 0x07E0       ///<   0, 255,   0
#define ILI9341_CYAN 0x07FF        ///<   0, 255, 255
#define ILI9341_RED 0xF800         ///< 255,   0,   0
#define ILI9341_MAGENTA 0xF81F     ///< 255,   0, 255
#define ILI9341_YELLOW 0xFFE0      ///< 255, 255,   0
#define ILI9341_WHITE 0xFFFF       ///< 255, 255, 255
#define ILI9341_ORANGE 0xFD20      ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define ILI9341_PINK 0xFC18        ///< 255, 130, 198
 */

#endif // __DISPLAY_H_
