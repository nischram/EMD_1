#ifndef __MODBUS_H_
#define __MODBUS_H_

/*
  Modbus-Arduino Example - Master (Modbus IP ESP8266/ESP32)
  Read Holding Register from Slave device

  (c)2018 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
*/

#include <ModbusIP_ESP8266.h>

bool modbusTimeout = false, modbusReady = false;

ModbusIP mb;  //ModbusIP
IPAddress mbIP_E3DC;
int mbDelay = 45;

union DoubleRegister
{
    int32_t  dr;    // occupies 4 bytes
    uint16_t sr[2]; // occupies 4 bytes
};

DoubleRegister registerBuffer;
uint16_t magicbyte = 0;
DoubleRegister solarPowerReg;
DoubleRegister gridPowerReg;
DoubleRegister batPowerReg;
DoubleRegister homePowerReg;
DoubleRegister extPowerReg;
DoubleRegister wbAllPowerReg;
DoubleRegister wbSolarPowerReg;
uint16_t wbCtrlReg             = 0;
uint16_t batSocReg             = 0;
uint16_t autarkieReg           = 0;
uint16_t PV_U1_Reg             = 0;
uint16_t PV_U2_Reg             = 0;
uint16_t PV_I1_Reg             = 0;
uint16_t PV_I2_Reg             = 0;
uint16_t PV_P1_Reg             = 0;
uint16_t PV_P2_Reg             = 0;
int batPower       = 0;
int gridPower      = 0;
int solarPower     = 0;
int homePower      = 0;
int extPower       = 0;
int wbAllPower     = 0;
int wbSolarPower   = 0;
int wbCtrl         = 0;
int batSoc         = 0;
int autarkie       = 0;
int eigenverbrauch = 0;
int pvU1           = 0;
int pvU2           = 0;
int pvI1           = 0;
int pvI2           = 0;
int pvP1           = 0;
int pvP2           = 0;
int mbDebugCounter = 0;
int countMbReset   = 0;
int countMbReboot  = 0;
bool wbMode,wbErr,wbConnect,wbLocked,wbCharge;
int wbPhases;

bool cbConn(IPAddress ip) {
  Serial.println(ip);
  return true; // Return 'true' to allow connection or 'false' to drop connection
}

void initModbus(const char * ipAdress){
  solarPowerReg.dr = 0;
  gridPowerReg.dr = 0;
  batPowerReg.dr = 0;
  homePowerReg.dr = 0;
  extPowerReg.dr = 0;
  wbAllPowerReg.dr = 0;
  wbSolarPowerReg.dr = 0;
  
  mbIP_E3DC.fromString(ipAdress);
  Serial.print("Modbus Init        :  wait...");
  if(INTERVALL_MODBUS > INTERVALL_HM) IntervalModbus = INTERVALL_HM;
  else IntervalModbus = INTERVALL_MODBUS;
  int timeout = 0;
  mb.master();
  delay(100);
  lastMbMillis = millis();
  while (!mb.isConnected(mbIP_E3DC) && millis() - lastMbMillis < MODBUS_TIMEOUT*1000) {
    mb.connect(mbIP_E3DC);
    Serial.print(".");
    delay(100);
  }
  if (millis() - lastMbMillis < MODBUS_TIMEOUT*1000){
    Serial.println("");
    Serial.println("Modbus status      :  connected");
    tftPrintInit("Modbus status  : connected");

    Serial.print("Modbus port        :  ");
    Serial.println(MODBUSTCP_PORT);
    modbusTimeout = false;
    modbusReady = true;
  }
  else {
    Serial.println("");
    Serial.println("Modbus status      :  failed!");
    tftPrintInit("Modbus status  : failed!");
    modbusTimeout = true;
    modbusReady = false;

  }
}
void closeModbus(IPAddress ip){
    Serial.print("Modbus close       :  ");
    if (mb.disconnect(ip)) Serial.println("ready");
    else Serial.println("failed!");
    modbusTimeout = true;
    modbusReady = false;
}
void firstReadMagicByte(IPAddress ip){
  if(modbusTimeout == true)return;
  char out[24];
    while(magicbyte != 0xE3DC ) {
    mb.readHreg(ip, REG_MAGIC -1, &magicbyte);
    delay(40);
    mb.task();
    delay(40);
  }
  sprintf (out, "0x%X",magicbyte);
  Serial.print("Magic byte         :  ");Serial.println(out);
  tftPrintInit("Magic byte     : %s", out);
}
void mainTaskMbRead(){
    if(INTERVALL_MODBUS >= 10 ) {
      mb.connect(mbIP_E3DC);
      delay(mbDelay);
    }
    mb.readHreg(mbIP_E3DC, REG_MAGIC -1, &magicbyte);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_SOLAR +REG_OFFSET, &solarPowerReg.sr[0], 2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_GRID +REG_OFFSET, &gridPowerReg.sr[0], 2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_BAT +REG_OFFSET, &batPowerReg.sr[0], 2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_CON +REG_OFFSET, &homePowerReg.sr[0], 2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_BATSOC +REG_OFFSET, &batSocReg);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_AUTARKIE +REG_OFFSET, &autarkieReg);delay(mbDelay);
    mb.task();
    #ifdef EXT_LM_USE
      mb.readHreg(mbIP_E3DC, REG_EXT +REG_OFFSET, &extPowerReg.sr[0], 2);delay(mbDelay);
      mb.task();

#endif
    #ifdef EXT_WB_USE
      mb.readHreg(mbIP_E3DC, REG_WB_ALL +REG_OFFSET, &wbAllPowerReg.sr[0], 2);delay(mbDelay);
      mb.task();
      mb.readHreg(mbIP_E3DC, REG_WB_SOLAR +REG_OFFSET, &wbSolarPowerReg.sr[0], 2);delay(mbDelay);
      mb.task();
      mb.readHreg(mbIP_E3DC, REG_WB_CTRL +REG_OFFSET, &wbCtrlReg);delay(mbDelay);
      mb.task();
    #endif
    mbDebugCounter++;
    if(mbDebugCounter > 65530) mbDebugCounter = 0;
    if(INTERVALL_MODBUS >= 10 ) {
      mb.disconnect(mbIP_E3DC);
    }
}
void pvTaskMbRead(){
    if(INTERVALL_MODBUS >= 10 ) {
      mb.connect(mbIP_E3DC);
      delay(mbDelay);
    }
    mb.readHreg(mbIP_E3DC, REG_SOLAR +REG_OFFSET, &solarPowerReg.sr[0], 2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_PV_U1 +REG_OFFSET, &PV_U1_Reg);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_PV_U2 +REG_OFFSET, &PV_U2_Reg);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_PV_I1 +REG_OFFSET, &PV_I1_Reg);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_PV_I2 +REG_OFFSET, &PV_I2_Reg);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_PV_P1 +REG_OFFSET, &PV_P1_Reg);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_PV_P2 +REG_OFFSET, &PV_P2_Reg);delay(mbDelay);
    mb.task();
    if(INTERVALL_MODBUS >= 10 ) {
      mb.disconnect(mbIP_E3DC);
    }
}
void mbCalcInt16(uint16_t *reg1, int *value){
    *value = *reg1;
    *reg1 = 0;
}
void mbCalcAutarkieEigenv(uint16_t *reg, int *Autarkie, int *Eigenverbrauch){
    *Autarkie = *reg / 256;
    *Eigenverbrauch = *reg % 256;
    *reg = 0;
}
void mainMbRead(){
    Serial.println("_______________________________________ ");
    serialPrintClock();
    mainTaskMbRead();
    #ifdef DEBUG
      Serial.printf("Reboot Counter     :  %5d\n",readRebootCounter());
      Serial.printf("Debug  Counter     :  %5d\n",mbDebugCounter);
    #endif
    solarPower = solarPowerReg.dr;
    gridPower = gridPowerReg.dr;
    batPower = batPowerReg.dr;
    homePower = homePowerReg.dr;
    mbCalcInt16(&batSocReg, &batSoc);
    mbCalcAutarkieEigenv(&autarkieReg, &autarkie, &eigenverbrauch);
    #ifdef EXT_LM_USE
      extPower = extPowerReg.dr;
    #endif
    #ifdef EXT_WB_USE
      wbAllPower = wbAllPowerReg.dr;
      wbSolarPower = wbSolarPowerReg.dr;
      mbCalcInt16(&wbCtrlReg, &wbCtrl);
    #endif
    Serial.printf("Power Solar        :  %6d W\n",solarPower);
    Serial.printf("Power Grid         :  %6d W\n",gridPower);
    Serial.printf("Power Batterie     :  %6d W\n",batPower);
    Serial.printf("Power Home         :  %6d W\n",homePower);
    Serial.printf("Batterie SOC       :  %6d %%\n",batSoc);
    Serial.printf("Autarkie           :  %6d %%\n",autarkie);
    Serial.printf("Eigenverbrauch     :  %6d %%\n",eigenverbrauch);
    #ifdef EXT_LM_USE
      Serial.printf("Power Ext-LM       :  %6d W\n",extPower);
    #endif
    #ifdef EXT_WB_USE
      Serial.printf("Power WB-All       :  %6d W\n",wbAllPower);
      Serial.printf("Power WB-Solar     :  %6d W\n",wbSolarPower);
      Serial.printf("WB-CTRL            :  0x%X\n",wbCtrl  );
      if ((wbCtrl & WB_MODE) == WB_MODE) Serial.printf("WB-Mode            :   Solar | ");
      else Serial.printf("WB-Mode            :  Mix | ");
      if ((wbCtrl & WB_ERR) == WB_ERR) Serial.printf("Laden abgebrochen\n");
      else Serial.printf("Laden freigegeben\n");
      if ((wbCtrl & WB_CONNECT) == WB_CONNECT) Serial.printf("WB > Auto          :  gesteckt | ");
      else Serial.printf("WB > Auto          :  nicht gesteckt | ");
      if ((wbCtrl & WB_LOCKED) == WB_LOCKED) Serial.printf("verriegelt | ");
      else Serial.printf("nicht verriegelt | ");
      if ((wbCtrl & WB_CHARGE) == WB_CHARGE) Serial.printf("lädt\n");
      else Serial.printf("lädt nicht\n");
      if ((wbCtrl & WB_PHASES) == WB_PHASES) Serial.printf("WB-Phasen          :  1 von 3\n");
      else Serial.printf("WB-Phasen          :  3 von 3\n");
    #endif
}
void pvMbRead(){
    Serial.println("_______________________________________ ");
    serialPrintClock();
    pvTaskMbRead();
    #ifdef DEBUG
      Serial.printf("Reboot Counter     :  %5d\n",readRebootCounter());
    #endif
    solarPower = solarPowerReg.dr;
    mbCalcInt16(&PV_U1_Reg, &pvU1);
    mbCalcInt16(&PV_U2_Reg, &pvU2);
    mbCalcInt16(&PV_I1_Reg, &pvI1);
    mbCalcInt16(&PV_I2_Reg, &pvI2);
    mbCalcInt16(&PV_P1_Reg, &pvP1);
    mbCalcInt16(&PV_P2_Reg, &pvP2);
    Serial.printf("Power Solar        :  %6d W\n",solarPower);
    Serial.printf("Volage String 1    :  %6d V\n",pvU1);
    Serial.printf("Volage String 2    :  %6d V\n",pvU2);
    Serial.printf("Ampere String 1    :  %6.2f A\n",pvI1 * 0.01);
    Serial.printf("Ampere String 2    :  %6.2f A\n",pvI2 * 0.01);
    Serial.printf("Power  String 1    :  %6d W\n",pvP1);
    Serial.printf("Power  String 2    :  %6d W\n",pvP2);
}
void checkMb(){
    if (magicbyte != 0xE3DC){
      countMbReset++;
      Serial.printf("MB Reset  Count    :  %6d\n",countMbReset);
    }
    else {
      countMbReset  = 0;
      countMbReboot = 0;
    }
    if (countMbReset >= MB_RESET){
      closeModbus(mbIP_E3DC);
      delay(1000);
      initModbus(IP_E3DC);
      countMbReboot++;
      countMbReset  = 0;
      Serial.printf("MB Reboot Count    :  %6d\n",countMbReboot);
      drawButton  = NEW;
      drawScreen  = NEW;
      drawContent = NEW;
    }
    if (countMbReboot >= MB_REBOOT){
      ESP.restart();
    }
}
#endif // __MODBUS_H_
