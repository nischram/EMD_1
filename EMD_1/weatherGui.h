#ifndef __WEATHERGUI_H_
#define __WEATHERGUI_H_

/*
   Open Weather One Call Library
   v3.0.4
   Copyright 2020 - Jessica Hershey
   www.github.com/JHershey69

   One Call API key at www.openweathermap.org
   Google Developer Key no longer required.

   Simple_Latitude_Longitude_Weather_Example.ino

   Returns ALL information and uses Latitude/Longitude, CITY ID, or IP Address
   If using a hotspot to connect your ESP32 to the WWW your results for IP
   search maybe be radically out of range. Please consult the documentation
   for use and variables available in the returned weather message

*/

// ===== Required libraries (Other required libraries are installed in header file)
#include <OpenWeatherOneCall.h>

// Weather icon
void drawWeatherIcon(char *id,int x, int y){
  if      (strcmp(id, "01d")==0) tft.drawRGBBitmap(x, y, wetter01d,80,80);
  else if (strcmp(id, "01n")==0) tft.drawRGBBitmap(x, y, wetter01n,80,80);
  else if (strcmp(id, "02d")==0) tft.drawRGBBitmap(x, y, wetter02d,80,80);
  else if (strcmp(id, "02n")==0) tft.drawRGBBitmap(x, y, wetter02n,80,80);
  else if (strcmp(id, "03d")==0) tft.drawRGBBitmap(x, y, wetter03d,80,80);
  else if (strcmp(id, "03n")==0) tft.drawRGBBitmap(x, y, wetter03n,80,80);
  else if (strcmp(id, "04d")==0) tft.drawRGBBitmap(x, y, wetter04d,80,80);
  else if (strcmp(id, "04n")==0) tft.drawRGBBitmap(x, y, wetter04n,80,80);
  else if (strcmp(id, "09d")==0) tft.drawRGBBitmap(x, y, wetter09d,80,80);
  else if (strcmp(id, "09n")==0) tft.drawRGBBitmap(x, y, wetter09n,80,80);
  else if (strcmp(id, "10d")==0) tft.drawRGBBitmap(x, y, wetter10d,80,80);
  else if (strcmp(id, "10n")==0) tft.drawRGBBitmap(x, y, wetter10n,80,80);
  else if (strcmp(id, "11d")==0) tft.drawRGBBitmap(x, y, wetter11d,80,80);
  else if (strcmp(id, "11n")==0) tft.drawRGBBitmap(x, y, wetter11n,80,80);
  else if (strcmp(id, "13d")==0) tft.drawRGBBitmap(x, y, wetter13d,80,80);
  else if (strcmp(id, "13n")==0) tft.drawRGBBitmap(x, y, wetter13n,80,80);
  else if (strcmp(id, "50d")==0) tft.drawRGBBitmap(x, y, wetter50d,80,80);
  else if (strcmp(id, "50n")==0) tft.drawRGBBitmap(x, y, wetter50n,80,80);
}
void drawWeatherIconSmall(char *id,int x, int y){
  if      (strcmp(id, "01d")==0) tft.drawRGBBitmap(x, y, wetterSmall01d,40,40);
  else if (strcmp(id, "01n")==0) tft.drawRGBBitmap(x, y, wetterSmall01n,40,40);
  else if (strcmp(id, "02d")==0) tft.drawRGBBitmap(x, y, wetterSmall02d,40,40);
  else if (strcmp(id, "02n")==0) tft.drawRGBBitmap(x, y, wetterSmall02n,40,40);
  else if (strcmp(id, "03d")==0) tft.drawRGBBitmap(x, y, wetterSmall03d,40,40);
  else if (strcmp(id, "03n")==0) tft.drawRGBBitmap(x, y, wetterSmall03n,40,40);
  else if (strcmp(id, "04d")==0) tft.drawRGBBitmap(x, y, wetterSmall04d,40,40);
  else if (strcmp(id, "04n")==0) tft.drawRGBBitmap(x, y, wetterSmall04n,40,40);
  else if (strcmp(id, "09d")==0) tft.drawRGBBitmap(x, y, wetterSmall09d,40,40);
  else if (strcmp(id, "09n")==0) tft.drawRGBBitmap(x, y, wetterSmall09n,40,40);
  else if (strcmp(id, "10d")==0) tft.drawRGBBitmap(x, y, wetterSmall10d,40,40);
  else if (strcmp(id, "10n")==0) tft.drawRGBBitmap(x, y, wetterSmall10n,40,40);
  else if (strcmp(id, "11d")==0) tft.drawRGBBitmap(x, y, wetterSmall11d,40,40);
  else if (strcmp(id, "11n")==0) tft.drawRGBBitmap(x, y, wetterSmall11n,40,40);
  else if (strcmp(id, "13d")==0) tft.drawRGBBitmap(x, y, wetterSmall13d,40,40);
  else if (strcmp(id, "13n")==0) tft.drawRGBBitmap(x, y, wetterSmall13n,40,40);
  else if (strcmp(id, "50d")==0) tft.drawRGBBitmap(x, y, wetterSmall50d,40,40);
  else if (strcmp(id, "50n")==0) tft.drawRGBBitmap(x, y, wetterSmall50n,40,40);
}

// OpenWeatherMap OneCallApi Counter
int addCallCount(){
  int Month = makeMonth();
  if (Month != paraESP.getUInt("OWMC_MON")){
    paraESP.putUInt("OWMC",1);
    paraESP.putUInt("OWMC_MON",Month);
  }
  else
    paraESP.putUInt("OWMC",paraESP.getUInt("OWMC")+1);
  return paraESP.getUInt("OWMC");
}
int readCallCount(){
  return paraESP.getUInt("OWMC");
}


//  OpenWeatherOneCall variables

//For Latitude and Longitude Location setting if used
float myLATITUDE  = atof(WEATHER_LAT);    //<-----Some location on theGulf Coast of Florida
float myLONGITUDE = atof(WEATHER_LON);

//For City ID Location setting if used
int myCITYID = WEATHER_ID;          //<-----Toms River, NJ USA

int myUNITS = METRIC;          //<-----METRIC, IMPERIAL, KELVIN (IMPERIAL is default)

//Can't get current and historical at the same time
int myHISTORY = NULL;            //<-----Only required for historical data up to 5 days

//See manual for excludes, only CURRENT Data allows 1,000,000 calls a month
int myEXCLUDES = 0;              //<-----0 Excludes is default


//for debugging loop counting
int nextCall = 0;
//int callAttempt = 1;

OpenWeatherOneCall OWOC;              // <------ Invoke Library like this

bool weatherCallReady = false;
char icon[12], dayName[3];
int OWOC_error = 0;

void startWeatherCall(){

      // First set your Key value
    OWOC.setOpenWeatherKey(WEATHER_KEY);

    // Counter for OpenWeatherOneCall ++
    addCallCount();

    // Second set your position (can be CITY ID or Manual LATITUDE/LONGITUDE)
    // Choose one of the following options

    #ifdef WEATHER_GEO_USE
      // Here we use the Lat and Lon for Pensacola, Florida (Using the predefined values)
      OWOC.setLatLon(myLATITUDE, myLONGITUDE);
    #endif

    #ifdef WEATHER_ID_USE
      // If we are using CITY ID
      OWOC.setLatLon(myCITYID);
    #endif

    // Third set any EXCLUDES if required (Here we are not using any
    OWOC.setExcl(myEXCLUDES);

    // Set History if you want historical weather other wise NULL
    OWOC.setHistory(myHISTORY);

      // Set UNITS of MEASURE otherwise default is IMPERIAL
    OWOC.setUnits(myUNITS);

     //Now call the weather. Please notice no arguments are required in this call
    OWOC_error = OWOC.parseWeather();

}
void displayWeather() {
  if(strcmp(WEATHER_KEY, "12345678910111213141516171819202")==0){
    tft.drawRGBBitmap(30, 30, wetter01d,80,80);
    tft.drawRGBBitmap(130, 30, wetter10d,80,80);
    printLcdText(20,130, ILI9341_DARKGREY, FontSansSerif_plain_11,"Es ist kein Key gesetzt.");
    printLcdText(20,150, ILI9341_DARKGREY, FontSansSerif_plain_11,"Bitte einen Key erstellen auf:");
    printLcdText(20,170, ILI9341_DARKGREY, FontSansSerif_plain_11,"www.openweathermap.org");
    printLcdText(20,210, ILI9341_DARKGREY, FontSansSerif_plain_11,"Key in folgede Datei eintragen:");
    printLcdText(20,230, ILI9341_DARKGREY, FontSansSerif_plain_11,"parameter.h");
    weatherCallReady = true;
  }
  else {
    // wait symbol
    tft.drawRGBBitmap(80, 30, wait,80,80);

    // Set Ecludes
    myEXCLUDES = EXCL_H+EXCL_M;

    // Start Weather Call
    startWeatherCall();
    Serial.printf("OpenWeatherCall    :  error ");
    Serial.println(OWOC_error);
    if (OWOC_error != 0){
      tft.drawRGBBitmap(80, 30, wetterNOK,80,80);
      delay(1000);
      drawScreen = NEW;
      screenActive = SCREEN_MAIN;
      return;
    }
    else tft.drawRGBBitmap(80, 30, wetterOK,80,80);
    delay(500);

    //Now display some information, note the pointer requirement for current and alert, this is NEW for v3.0.0

    // Location info is available for ALL modes (History/Current)
    Serial.printf("Weather Location   :  %s, %s %s\n", OWOC.location.CITY, OWOC.location.STATE, OWOC.location.COUNTRY);
    Serial.printf("Weather Timeoffset :  %i Sek.\n\n", OWOC.location.timezoneOffset);

    //Verify all other values exist before using
      if (OWOC.current)
      {
        makeClock(OWOC.current->dayTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Current Temp       :  %.1f °C\n", OWOC.current->temperature);
        Serial.printf("Current Humidity   :  %.0f %%\n", OWOC.current->humidity);
        Serial.printf("Current Pressure   :  %.0f\n", OWOC.current->pressure);
        Serial.printf("Current windSpeed  :  %.1f m/s\n", OWOC.current->windSpeed);
        Serial.printf("Current icon       :  %s\n", OWOC.current->icon);
        weatherCallReady = true;
      }
      if (OWOC.forecast)
      {
        Serial.printf("\n");
        makeClock(OWOC.forecast[0].dayTime, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Temp H  :  %.0f °C\n", datum, OWOC.forecast[0].temperatureHigh);
        Serial.printf("Forecast %s Temp L  :  %.0f °C\n", datum, OWOC.forecast[0].temperatureLow);
        Serial.printf("Forecast %s Rain    :  %.1f l\n", datum, OWOC.forecast[0].rainVolume);
        Serial.printf("Forecast %s Icon    :  %s\n", datum, OWOC.forecast[0].icon);
        makeClock(OWOC.forecast[0].sunriseTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        makeClock(OWOC.forecast[0].sunsetTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        Serial.printf("Forecast %s WeekDay :  %s\n", datum, OWOC.forecast[0].weekDayName);
        makeClock(OWOC.forecast[1].dayTime, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s datum   :  %s\n", datum, datum);
        Serial.printf("Forecast %s Temp H  :  %.0f °C\n", datum, OWOC.forecast[1].temperatureHigh);
        Serial.printf("Forecast %s Temp L  :  %.0f °C\n", datum, OWOC.forecast[1].temperatureLow);
        Serial.printf("Forecast %s Rain    :  %.1f l\n", datum, OWOC.forecast[1].rainVolume);
        Serial.printf("Forecast %s Icon    :  %s\n", datum, OWOC.forecast[1].icon);
        makeClock(OWOC.forecast[1].sunriseTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        makeClock(OWOC.forecast[1].sunsetTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        Serial.printf("Forecast %s WeekDay :  %s\n", datum, OWOC.forecast[1].weekDayName);
        makeClock(OWOC.forecast[2].dayTime, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s datum   :  %s\n", datum, datum);
        Serial.printf("Forecast %s Temp H  :  %.0f °C\n", datum, OWOC.forecast[2].temperatureHigh);
        Serial.printf("Forecast %s Temp L  :  %.0f °C\n", datum, OWOC.forecast[2].temperatureLow);
        Serial.printf("Forecast %s Rain    :  %.1f l\n", datum, OWOC.forecast[2].rainVolume);
        Serial.printf("Forecast %s Icon    :  %s\n", datum, OWOC.forecast[2].icon);
        makeClock(OWOC.forecast[2].sunriseTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        makeClock(OWOC.forecast[2].sunsetTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        Serial.printf("Forecast %s WeekDay :  %s\n", datum, OWOC.forecast[2].weekDayName);
        makeClock(OWOC.forecast[3].dayTime, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s datum   :  %s\n", datum, datum);
        Serial.printf("Forecast %s Temp H  :  %.0f °C\n", datum, OWOC.forecast[3].temperatureHigh);
        Serial.printf("Forecast %s Temp L  :  %.0f °C\n", datum, OWOC.forecast[3].temperatureLow);
        Serial.printf("Forecast %s Rain    :  %.1f l\n", datum, OWOC.forecast[3].rainVolume);
        Serial.printf("Forecast %s Icon    :  %s\n", datum, OWOC.forecast[3].icon);
        makeClock(OWOC.forecast[3].sunriseTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        makeClock(OWOC.forecast[3].sunsetTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
        Serial.printf("Forecast %s Sunset  :  %s\n", datum, zeitHoMi);
        Serial.printf("Forecast %s WeekDay :  %s\n", datum, OWOC.forecast[3].weekDayName);
        weatherCallReady = true;
      }

      if (OWOC.alert) {
        Serial.printf("ALERT *** ALERT *** ALERT\n");
        Serial.printf("Sender :  %s\n", OWOC.alert->senderName);
        Serial.printf("Event  :  %s\n", OWOC.alert->event);
        Serial.printf("ALERT  :  %s\n", OWOC.alert->summary);
        weatherCallReady = true;
      }

      //Screen
      makeClock(OWOC.current->dayTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
      overwriteLcdText(50, 24, 150, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%s %s %s", OWOC.forecast[0].weekDayName, datum, zeitHoMi);
      drawWeatherIcon(OWOC.current->icon,80,30);
      tft.drawRGBBitmap(12, 25, sunriseSmall,40,40);
      tft.drawRGBBitmap(47, 25, sunsetSmall,40,40);
      tft.drawRGBBitmap(30, 65, windSmall,40,40);
      makeClock(OWOC.forecast[0].sunriseTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
      overwriteLcdText(14, 65, 40, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%s", zeitHoMi);
      makeClock(OWOC.forecast[0].sunsetTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
      overwriteLcdText(52, 65, 40, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%s", zeitHoMi);
      overwriteLcdText(25, 110, 60, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.current->windSpeed);
      printLcdText(55,110, ILI9341_DARKGREY, FontMonospaced_bold_10,"m");printLcdText(61,110, ILI9341_DARKGREY, FontMonospaced_bold_10,"/");printLcdText(65,110, ILI9341_DARKGREY, FontMonospaced_bold_10,"s");

      overwriteLcdText(160, 50, 68, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.1f\n", OWOC.current->temperature);
      tft.drawRGBBitmap(200, 41, celsius,11,9);
      overwriteLcdText(160, 65, 68, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.0f\n", OWOC.current->humidity);
      tft.drawRGBBitmap(200, 56, percent,9,9);
      overwriteLcdText(160, 80, 68, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.0f\n", OWOC.current->pressure);
      printLcdText(200,80, ILI9341_DARKGREY, FontMonospaced_bold_13,"h");
      printLcdText(207,80, ILI9341_DARKGREY, FontMonospaced_bold_13,"P");
      printLcdText(214,80, ILI9341_DARKGREY, FontMonospaced_bold_13,"a");
      overwriteLcdText(160, 95, 68, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.1f\n", OWOC.forecast[0].rainVolume);
      printLcdText(200,95, ILI9341_DARKGREY, FontMonospaced_bold_13,"l");
      overwriteLcdText(160, 110, 68, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.0f\n", OWOC.forecast[0].windBearing);
      tft.drawRGBBitmap(200, 101, celsius,11,9);tft.fillRect(204,101,9,9, ILI9341_WHITE);
      //
      tft.fillRect(20,120,200,2, ILI9341_LIGHTGREY);
      sprintf (dayName, "%s", OWOC.forecast[0].weekDayName);
      printLcdText(33,140, ILI9341_DARKGREY, FontMonospaced_bold_13,"%s",dayName);
      sprintf (dayName, "%s", OWOC.forecast[1].weekDayName);
      printLcdText(83,140, ILI9341_DARKGREY, FontMonospaced_bold_13,"%s",dayName);
      sprintf (dayName, "%s", OWOC.forecast[2].weekDayName);
      printLcdText(130,140, ILI9341_DARKGREY, FontMonospaced_bold_13,"%s",dayName);
      sprintf (dayName, "%s", OWOC.forecast[3].weekDayName);
      printLcdText(180,140, ILI9341_DARKGREY, FontMonospaced_bold_13,"%s",dayName);
      drawWeatherIconSmall(OWOC.forecast[0].icon,27,142);
      drawWeatherIconSmall(OWOC.forecast[1].icon,78,142);
      drawWeatherIconSmall(OWOC.forecast[2].icon,125,142);
      drawWeatherIconSmall(OWOC.forecast[3].icon,175,142);
      printLcdText(17,195, ILI9341_DARKGREY, FontMonospaced_bold_10,"'");printLcdText(22,195, ILI9341_DARKGREY, FontMonospaced_bold_10,"C");
      overwriteLcdText(40, 195, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[0].temperatureHigh);
      overwriteLcdText(86, 195, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[1].temperatureHigh);
      overwriteLcdText(132, 195, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[2].temperatureHigh);
      overwriteLcdText(178, 195, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[3].temperatureHigh);
      printLcdText(17,207, ILI9341_DARKGREY, FontMonospaced_bold_10,"'");printLcdText(22,207, ILI9341_DARKGREY, FontMonospaced_bold_10,"C");
      overwriteLcdText(40, 207, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[0].temperatureLow);
      overwriteLcdText(86, 207, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[1].temperatureLow);
      overwriteLcdText(132, 207, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[2].temperatureLow);
      overwriteLcdText(178, 207, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[3].temperatureLow);
      printLcdText(22,219, ILI9341_DARKGREY, FontMonospaced_bold_10,"l");
      overwriteLcdText(40, 219, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[0].rainVolume);
      overwriteLcdText(86, 219, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[1].rainVolume);
      overwriteLcdText(132, 219, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[2].rainVolume);
      overwriteLcdText(178, 219, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[3].rainVolume);
      printLcdText(17,231, ILI9341_DARKGREY, FontMonospaced_bold_10,"m");printLcdText(23,231, ILI9341_DARKGREY, FontMonospaced_bold_10,"/");printLcdText(27,231, ILI9341_DARKGREY, FontMonospaced_bold_10,"s");
      overwriteLcdText(40, 231, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[0].windSpeed);
      overwriteLcdText(86, 231, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[1].windSpeed);
      overwriteLcdText(132, 231, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[2].windSpeed);
      overwriteLcdText(178, 231, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.1f\n", OWOC.forecast[3].windSpeed);
      printLcdText(17,243, ILI9341_DARKGREY, FontMonospaced_bold_10,"h");printLcdText(23,243, ILI9341_DARKGREY, FontMonospaced_bold_10,"P");printLcdText(29,243, ILI9341_DARKGREY, FontMonospaced_bold_10,"a");
      overwriteLcdText(40, 243, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.0f\n", OWOC.forecast[0].pressure);
      overwriteLcdText(86, 243, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.0f\n", OWOC.forecast[1].pressure);
      overwriteLcdText(132, 243, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.0f\n", OWOC.forecast[2].pressure);
      overwriteLcdText(178, 243, 30, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%4.0f\n", OWOC.forecast[3].pressure);

      overwriteLcdText(60, 276, 160, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"OpenWeatherMap");
      overwriteLcdText(60, 286, 160, 8, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_10,"OneCallCounter:%4i", readCallCount());

      weatherCallReady = true;
  }
}
void drawWeatherSingleIcon() {
      if (millis() - weatherMillis > WEATHER_INTERVALL *60*1000 || weatherMillis == 0){
        weatherMillis = millis();
        if(strcmp(WEATHER_KEY, "12345678910111213141516171819202")==1){
          // Set Ecludes
          myEXCLUDES = EXCL_H+EXCL_M+EXCL_D+EXCL_A;

          // Start Weather Call
          startWeatherCall();

          if (OWOC.current)
          {
            drawWeatherIcon(OWOC.current->icon,80,10);
            makeClock(OWOC.current->dayTime+OWOC.location.timezoneOffset, timeStamp, datum, zeit, zeitHoMi);
            overwriteLcdText(104, 18, 50, 8, ILI9341_LIGHTGREY, ILI9341_WHITE, FontMonospaced_bold_10,"%s", zeitHoMi);
            overwriteLcdText(101, 91, 68, 12, ILI9341_DARKGREY, ILI9341_WHITE, FontMonospaced_bold_13,"%4.1f\n", OWOC.current->temperature);
            tft.drawRGBBitmap(137, 82, celsius,11,9);
          }
        }
      }
}



#endif // __WEATHERGUI_H_
