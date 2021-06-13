#ifndef __NTP_H_
#define __NTP_H_

int timeDiff;
char timeStamp[100];
char datum[100];
char zeit[100];
char zeitHoMi[100];

long millisNTP = 0;

void cTime(int TM, char * var){
  if(TM < 10)
    sprintf (var, "0%i", TM);
  else
    sprintf (var, "%i", TM);
}
void makeClock(time_t tm, char *url, char *datum, char *zeit, char *tHM){
  // digital clock display of the time
  char HOUR[4], MIN[4], SEK[4], DAY[4], MON[4], YEAR[6];
  char tStamp[100];
  cTime(hour(tm), HOUR);
  cTime(minute(tm), MIN);
  cTime(second(tm), SEK);
  cTime(day(tm), DAY);
  cTime(month(tm), MON);
  cTime(year(tm), YEAR); 
  sprintf (datum, "%s.%s.%s",DAY,MON,YEAR);
  sprintf (zeit, "%s:%s:%s",HOUR,MIN,SEK);
  sprintf (tHM, "%s:%s",HOUR,MIN);
  sprintf (url, "%s:%s:%s__%s.%s.%s",HOUR,MIN,SEK,DAY,MON,YEAR);
}
void serialPrintClock(){
    makeClock(time(nullptr) + timeDiff, timeStamp, datum, zeit,zeitHoMi);
    Serial.print("Datum            :  ");Serial.println(datum);
    Serial.print("Uhrzeit          :  ");Serial.println(zeit);
}
void tftPrintClock(){
    makeClock(time(nullptr) + timeDiff, timeStamp, datum, zeit,zeitHoMi);
    tftPrintInit("ESP Date       : %s", datum);
    tftPrintInit("ESP Time       : %s", zeit);
}
void initNTP(){
  configTime(TIMEZONE * 3600, DST * 0, NTP_SERVER_1, NTP_SERVER_2);
  Serial.println("Time init        :  wait...");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(500);
  }
  delay(500);
  timeDiff = readTimeDiff();
  serialPrintClock();
  tftPrintClock();
}
int makeMonth(){
  time_t tm = time(nullptr);
  char MON[4];
  cTime(month(tm), MON);
  return atoi(MON);
}

#endif // __NTP_H_
