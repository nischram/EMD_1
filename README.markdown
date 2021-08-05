# E3DC-Modbus-Display-1
[Stand: V1.01 05.08.2021](https://github.com/nischram/EMD_1#changelog)

Hier beschreibe ich, wie du die Daten eines S10 Hauskraftwerk von E3DC auf einem Display anzeigen kannst.

Es wird ein ESP32 und ein 2,8" Touch-Display verwendet.

__Daten per Modbus vom E3DC-S10 Speicher abfragen__  
__Daten vom S10 auf dem Display darstellen__  
__Daten vom E3DC zur Homematic Hausautomation senden__  
__Daten der E3DC Wallbox anzeigen__  
__Wetterinformationen von "OpenWeatherMap"__  
__Temperatursensor für Raumtemperatur__  
__Update per SD-Karte__  
__Update fia OTA__  


<img width="400" src="https://user-images.githubusercontent.com/19279623/121783642-1613a180-cbb0-11eb-87c8-10a93b23b2a2.png" alt="Display_E3DC">  

Diese Kombination bietet eine sehr gute Möglichkeit, um das S10 einfach und schnell mit einem eigenem Display zu überwachen.

Ich nutze diese Applikation auf einem ESP32, mit dem 2,8“ Touchdisplay. Auf das Material und die Installation gehe ich in dieser Anleitung ein.

## Display

### E3DC aktuelle Werte
<img src="https://user-images.githubusercontent.com/19279623/121786745-377d8900-cbc2-11eb-82c2-1f1a71016661.png" alt="menuLM">  
Auf dem Display kannst du dir die Aktuellen Werte deine S10 Hauskraftwerk anzeigen lassen.  
<img width="300" src="https://user-images.githubusercontent.com/19279623/121786643-7101c480-cbc1-11eb-9b63-5a5279d32729.png" alt="E3DC_Aktuell">  
Die Werte werden je nach Einstellung z.B. im Sekunden-Rhythmus oder langsamer aktualisiert.  
Wenn OpenWeatherMap konfiguriert oder/und ein DHT Sensor verbaut wurden, dann werden diese Werte auf Schirm angezeigt.  
<img width="300" src="https://user-images.githubusercontent.com/19279623/121818145-b687c580-cc85-11eb-949e-662c5df929f7.png" alt="Aktuell-DHT-Wetter">  
Um diese Ansicht der aktuellen Werte zu verlassen gibt es drei Touch-Bereiche:  
<img width="300" src="https://user-images.githubusercontent.com/19279623/121818266-60675200-cc86-11eb-8d37-d8e57adeabe8.png" alt="Aktuell-Touch">  
Unten der Pfeil bringt dich ins Hauptmenü. Oben links direkt auf der Sonne geklickt, bringt dich in die Solar-Ansicht und wenn die Wetterdaten aktiv sind, kannst du auf dem Wettersymbol klicken und direkt zu den Wetterdaten wechseln. Wichtig zu wissen, die Modbusabfrage dauert fast die Sekunde bis zum nächsten Intervall und so muss der Touch ggf. über einen Abfrageintervall hinaus gedrückt werden.  

### Wetterdaten von OpenWeatherMap
<img src="https://user-images.githubusercontent.com/19279623/121786776-6267dd00-cbc2-11eb-8255-7b775f9d63dc.png" alt="Sym_Wetter">  
In einem weiterem Fenster bekommst du Wetterinformationen angezeigt. Hierfür ist es aber erforderlich, dass du dir einen Account bei [openweathermap.org](https://openweathermap.org) einrichtest. Mit diesem Zugang kannst du dir einen API-Key erstellen. Deine City-ID oder deine Geokoordinaten noch eintragen und dann bekommst du folgende Grafik auf dem Display:  
<img width="400" src="https://user-images.githubusercontent.com/19279623/121783697-6b4fb300-cbb0-11eb-87c1-16a11f59a37d.png" alt="Display_Wetter">  
Unten im Fenster ist ein Zähler für die Anzahl der Abfragen, der Monatlich zurückgesetzt wird.  

### Solarinformation vom Wechselrichter
<img src="https://user-images.githubusercontent.com/19279623/121786760-4e23e000-cbc2-11eb-9f94-2a2cbc6dc9ca.png" alt="menuPV">  
Mit einer Solaranzeige kannst du dir die zwei Tracker deines Wechselrichters anzeigen lassen.  
<img width="400" src="https://user-images.githubusercontent.com/19279623/121783686-583ce300-cbb0-11eb-817e-4b59be6bf7d0.png" alt="Display_Solar">  

### Update des Display
<img src="https://user-images.githubusercontent.com/19279623/121786757-46fcd200-cbc2-11eb-947b-ba1d0ff7e06f.png" alt="menuUpdate">  
Ein Menü habe ich erstellt mit man per SD-Karte den ESP32 updaten kann. Das Update per SD-Karte ist unten im Bereich [Update per SD-Karte](https://github.com/nischram/EMD_1#software) beschrieben.

### Refresh Menü
<img src="https://user-images.githubusercontent.com/19279623/121786755-43694b00-cbc2-11eb-991c-3c8b402b8d47.png" alt="menuRefresh">  
In einen Menü kannst du verschiedene Dinge neu starten:  
- den ESP32  
- die W-LAN Verbindung  
- die Modbus Schnittstelle  
- die SD-Karte  

### Menü für Einstellungen
<img src="https://user-images.githubusercontent.com/19279623/121786758-49f7c280-cbc2-11eb-949d-94509a973084.png" alt="menuEinst">  
Ein Paar einstellungen kannst du vornehmen.  
- Display drehen damit ggf. der Temperatur-Sensor oder der PIR nach unten kommt  
- Sommer- Winterzeit  
- Bewegungsmelder aktivieren (schaltet den Bildschirmschoner aus)  
- Zeit bis der Bildschirmschoner startet  
- DHT11/22 anzeige auf Hauptschirm ein- ausblenden  

## Aufbau

### Material
Als Kostengünstig und Einfach habe ich für mich das Display von AZ-Delivery entdeckt. Die neue Version dieses Sets wird jetzt mit einem 2,8" Display geliefert es kann aber auch als 2,4" bestellt werden, die ältere Version des Sets wurde nur mit einem 2,4" geliefert. Dieses Set kann mit einem ESP32 NodeMCU ausgestattet werden. Um den ESP32 herum, habe ich die Software aufgesetzt.  
- [AZ-Touch MOD Wandgehäuseset mit 2,8 Zoll Touchscreen](https://www.az-delivery.de/products/az-touch-wandgehauseset-mit-2-8-zoll-touchscreen-fur-esp8266-und-esp32)  
- [ESP32 NodeMCU](https://www.az-delivery.de/collections/mikrocontroller/products/esp32-dev-kit-c-unverlotet)  
Das Set muss zu einem Kleinen Teil noch zusammengebaut und verlötet werden. Auf diesen Aufbau gehe ich jetzt nicht näher ein, denn die Anleitung von AZ-Delivery ist gut beschrieben. Das Kostenlose eBook sollte man gleich mit bestellen: [eBook AZ Touch](https://www.az-delivery.de/products/az-touch-wandgehauseset-mit-2-8-zoll-touchscreen-fur-esp8266-und-esp32-1)  
- [AMR312 PIR Bewegungsmelder](http://www.amazon.de/dp/B07RJQMD9G/)  
- [DHT11 Temperatur- und Luftfeuchtigkeits-Sensor](https://www.az-delivery.de/products/5-x-dht11-temperatursensor?_pos=1)  

### ESP32 einlöten
Ich habe mir den ESP32 "unverlötet" bestellt. So konnte ich die Stiftleiste nicht komplett eingeschoben verlöten. Hier im Foto zu sehen:  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121781609-dfd12480-cba5-11eb-8134-aad518182764.png" alt="Stiftleiste">  
Die gelbe Abstandshalter habe ich dann nach dem einlöten an die Platine geschoben. Der Vorteil darin liegt im einstecken in den Pinheader auf der Haupt-Platine. Dieser Pinheader ist ein Header zum durchstecken und so wird für einen guten Kontakt etwas mehr Länge bei der Stiftleiste benötigt.  

### SD-Kartenleser anschließen
Die Display-Einheit hat im Standard einen SD-Kartenslot verbaut. Die Haupt-Platine wird aber ohne Anschlüsse für den SD-Kartenleser ausgeliefert. Der Kartenleser wird auch per SPI-Bus angesteuert. Um die SD-Karte zu nutzen habe ich auf der Hauptplatine einen zusätzlichen Pinheader eingelötet.  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121782083-2de72780-cba8-11eb-90e1-277563980b6c.png" alt="SD-Pinheader">  
Auf der Rückseite müssen dann einige Adern verlegt werden.  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121782645-e31adf00-cbaa-11eb-9b5a-e87195e9319b.png" alt="SD-Verdrahtung">  
Hier en Foto mit Beschreibung der Pinbelegung:  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121782635-d7c7b380-cbaa-11eb-9f07-1492fd8fba9f.png" alt="SD-Pinbelegung">  

### PIR Sensor verbauen
Es kann ein PIR Sensor angeschlossen werden, dieser beendet dann den Bildschirmschoner. Ich verwende einen [AMR312 PIR Bewegungsmelder](http://www.amazon.de/dp/B07RJQMD9G/). Für diesen muss ein Loch in den Gehäusedeckel bohren, bei meinem Sensor ein 9mm Loch.  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121815771-5b030b00-cc78-11eb-841c-de4f8c15e705.png" alt="Gehäuse_Loch">  
Ich habe den PIR-Sensor in der Platine nur angesetzt, alles im Gehäuse montiert und dann erst auf der Gegenseite verlötet. So sitzt dann der Sensor in der Platine:  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121816208-9999c500-cc7a-11eb-96ae-1659a73d93a8.png" alt="PIR-Montage">  
Die Verdrahtung habe ich wieder mit verlacktem Kupferdraht gemacht.  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121815827-97cf0200-cc78-11eb-9429-fac9f2628205.png" alt="PIR-Verdrahtung">  
Folgende Pinbelegung habe ich verwendet:  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121819861-bbea0d80-cc8f-11eb-84f6-bd1c84ac0c5f.png" alt="PIR-Pinbelegung">  

### DHT Temperatursensor
Es kann entweder ein DHT11 oder ein DHT22 Temperatur und Luftfeuchtigkeits-Sensor verbaut werden. Ich habe einen DHT11 eingebaut, damit dieser möglichst Nahe an den Öffnungen im Gehäuse sitzt habe ich diese Schräg eingesetzt.  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121817224-56425500-cc80-11eb-8b02-acff71cb33e9.png" alt="DHT-Montage">  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121817220-517da100-cc80-11eb-8583-c843f084a854.png" alt="PIR-DHT-Montage">  
Hier wieder Verdrahtung und die Pinbelegung dazu:  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121817151-e764fc00-cc7f-11eb-8598-0c52684e9c54.png" alt="DHT-Verdrahtung">  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121817146-e03dee00-cc7f-11eb-92ba-2296fd1a1935.png" alt="DHT-Pinbelegung">  
Das Geschlossene Gehäuse mit PIR im Loch und DHT-Sensor hinter der Öffnung:  
<img width="600" src="https://user-images.githubusercontent.com/19279623/121817227-5e9a9000-cc80-11eb-9620-d83d797a1a4a.png" alt="PIR-DHT-Gehäuse">  

## Einrichtung

### Arduine-IDE
Meine Software muss auf den ESP32 installiert werden. Die Software [Arduino-IDE](https://www.arduino.cc/en/software) ist hierfür geeignet. Die Einrichtung der Software und die Verbindung zum ESP32 erkläre ich nicht näher, denn auch zum ESP gibt es ein eBook mit wichtigen Informationen: [eBook ESP32 NodeMCU](https://www.az-delivery.de/products/esp32-nodemcu-kostenfreies-e-book)  
Sollte es trotzdem Probleme oder Fragen geben, helfe ich gerne wenn du einen [Issue](https://github.com/nischram/EMD_1/issues) erstellst.  

### Parameter für ESP32
Wichtig sind die Einstellungen für den ESP32. Mein Program ist durch die Grafiken recht umfangreich geworden, so weichen meine Einstellungen vom Standard ab. Wichtig ist folgender Parameter:  
> Partition Scheme: "Minimal SPIFFS (1,9MB App with OTA/190KB SPIFFS)"  

Hier noch ein Screenshot von meinen Einstellungen:  
<img width="463" alt="image" src="https://user-images.githubusercontent.com/19279623/121787937-8975dd00-cbc9-11eb-993e-1ccda0750905.png">  

### Library
Es müssen für die Software einige Libraries installiert werden, in der Bibliothek-Verwaltung kann man diese suchen und installieren. Folgende sind erforderlich:  
- XPT2046_Touchscreen  
- Adafruit_GFX  
- Adafruit_ILI9341  
- modbus-esp8266  
- ArduinoOTA  
- Time  
- Adafruit_BusIO  
- DHT sensors library  
- OpenWeatherOneCall  
- ArduinoJson  

### Parameter der Software
Im Code / Download stelle ich für die Parameter eine Datei zur Verfügung die als Grundlage dient. Die Datei __parameter_temp.h__ musst du umbenennen nach __parameter.h__. Durch das umbenennen der Datei wir bei einem Update nicht versehentlich deine Einstellung überschrieben. Wenn du jetzt die Software mit Arduino-IDE öffnest, kannst du in der __parameter.h__ deine Einstellung vornehmen.  

### Modbus
Die Funktion Modbus musst du einmalig am Display deines Speichers aktivieren. In der __parameter.h__ musst du dann die IP-Adresse deines Speichers eintragen.  

### W-LAN Zugangsdaten
In der __parameter.h__ müssen auch die Zugangsdaten für W-LAN eingetragen werden. Ich habe drei mögliche Zugangspunkte vorbereitet, so kann das Display an verschiedenen W-LAN Zugangspunkten betrieben werden. Aber wichtig ist, dass die W-LAN-Zugangspunkte alle Verbindung zum Speicher haben.  

### Homematic
Wie auch schon von anderer Software von mir bekannt, ist es auch mit dieser Software möglich die aktuellen Werte des Speichers zur Homematic zu senden. Es sind die jeweiligen ISE-ID der Systemvariablen nötig und müssen in der __parameter.h__ eingetragen werden.  

### Weitere Einstellungen
Es gibt noch einige weitere Einstellungen die möglich sind, aber zum größten Teil sollte der eingetragene default Wert funktionieren.  
Folgendes ist möglich:  

|Definition        | Beschreibung                                                                 |
| ---------------- | ---------------------------------------------------------------------------- |
|`PIR_PIN`         | für den Bewegungsmelder der den Bildschirmschoner abschaltet.                |
|`DHT_PIN`         | für DHT11/22 Temperatur-Sensor.                                              |
|`DHT_TYPE`        | als Auswahl für DHT11 oder DHT22.                                            |
|`NTP_SERVER_1`    | Parameter für den Zeitserver der Abgefragt wird um die Uhrzeit anzuzeigen.   |
|`OTA_HOSTNAME`    | Name für den ESP32 damit man diesen *OverTheAir* updaten kann.               |
|`WEATHER_KEY`     | und weitere Einstellungen für die Wetteranzeige.                             |
|`SERIALSPEED`     | Einstellung für die Geschwindigkeit der Serialen Schnittstelle.              |

## Software

### Projekt
Die Software und das Projekt hat viele parallelen zu meinem anderen Projekt: [nischram/E3dcGui](https://github.com/nischram/E3dcGui)  

### Update fia Arduino-IDE
Das Update mit der Software Arduino-IDE ist der Standard, hierfür muss aber eine USB-Verbindung mit dem ESP32 geschaffen werden. Bei der ersten Installation muss dieser Weg genommen werden.  

### Update per SD-Karte
Eine weitere Möglichkeit, ist das Update per SD-Karte. Hierzu musst du in der Software Arduino-IDE den Sketch nur als __Binärdatei__ kompilieren. Die Binärdatei muss umbenannt werden in __EMD_1.bin__, diese Datei muss auf einer SD-Karte in einem Ordner __update__ gespeichert werden. Wenn Update-Menü die SD-Karte erkannt und die Datei gefunden wurde, kann das Update mit dem Bestätigungs-Button gestartet werden. Nach dem Update startet der ESP32 neu.  

### Update fia OTA
Die Einfachste Option für ein Update ist die __Over-The-Air__ Variante. In der Arduino-IDE wird dann der ESP32 mit dem OTA-Namen angezeigt. Für die OTA-Funktion muss zuvor der Computer vorbereitet werden. Hier ein schöner Beitrag zum [OTA - Over the Air - ESP Programmieren](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/ota-over-the-air-esp-programmieren-uber-wlan-entwurf).  
Wenn alles Vorbereitet ist in der Arduino-IDE die richtige Einstellung für den ESP32 gesetzt sind, kann der Upload beginnen. Sobald die Verbindung aufgebaut ist, erscheint auf dem Display die eine Anzeige mit dem Fortschritt.  
<img width="200" src="https://user-images.githubusercontent.com/19279623/121818853-17190180-cc8a-11eb-8655-683f7e6377cc.png" alt="OTA-Update">  

### Versionen
V1.01 05.08.2021 [Issue #3](https://github.com/nischram/EMD_1/issues/3) Anpassung der Schriftgröße  
- Issue #3 Neue Schriftart und Anpassungen für die Schrift  

V1.00 12.06.2021 Repository neu erstellt  
- erster Upload der Daten und der Anleitung  
