#ifndef __DHTTEMP_H_
#define __DHTTEMP_H_

#ifdef DHT_USE

  #include "DHT.h"
  
    DHT dht(DHT_PIN, DHT_TYPE);
    float hum, temp;
  
  void initDHT(){
      dht.begin();
  }
  void readDHT(){
      hum = dht.readHumidity();  
      temp = dht.readTemperature();
  }
  void serialPrintDHT(){
      readDHT();
      
      Serial.print("Temperatur       :  ");
      Serial.print(temp);
      Serial.println(" °C");
      Serial.print("Luftfeuchtigkeit :  ");
      Serial.print(hum);
      Serial.println(" %");
  }
#endif
   

#endif // __DHTTEMP_H_
