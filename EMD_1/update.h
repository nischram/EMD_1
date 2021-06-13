
/* 
 Name:      SD_Update.ino
 Created:   12.09.2017 15:07:17
 Author:    Frederik Merz <frederik.merz@novalight.de>
 Purpose:   Update firmware from SD card

 Steps:
   1. Flash this image to the ESP32 an run it
   2. Copy update.bin to a SD-Card, you can basically
      compile this or any other example
      then copy and rename the app binary to the sd card root
   3. Connect SD-Card as shown in SD_MMC example, 
      this can also be adapted for SPI
   3. After successfull update and reboot, ESP32 shall start the new app
*/

#include <Update.h>
#include <FS.h>
#include <SD_MMC.h>

char updateFile[64], updateInfo1[128], updateInfo2[128], updateInfo3[128];
int findUpdate = 0, returnUpdate = 0;

// perform the actual update from a given stream
void performUpdate(Stream &updateSource, size_t updateSize) {
   if (Update.begin(updateSize)) {      
      size_t written = Update.writeStream(updateSource);
      if (written == updateSize) {
         Serial.println("Written : " + String(written) + " successfully");
      }
      else {
         Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
      }
      if (Update.end()) {
         Serial.println("Update done!");
         if (Update.isFinished()) {
            Serial.println("Update successfully completed. Rebooting.");
            sprintf(updateInfo2, "Erfolgreich!");
            returnUpdate = 1;
         }
         else {
            Serial.println("Update not finished? Something went wrong!");
            sprintf(updateInfo2, "etwas lief falsch!");
            returnUpdate = -1;
         }
      }
      else {
         Serial.println("Error Occurred. Error #: " + String(Update.getError()));
         sprintf(updateInfo2, "Error: %d",Update.getError());
         returnUpdate = -1;
      }

   }
   else
   {
      Serial.println("Not enough space to begin Update");
      sprintf(updateInfo2, "kein Speicher frei!");
      returnUpdate = -1;
   }
}

// check given FS for valid update.bin and perform update if available
void updateFromFS(fs::FS &fs, char const * filepath) {
   File updateBin = SD.open(filepath);
   if (updateBin) {
      if(updateBin.isDirectory()){
         Serial.print("Error, ");Serial.print(filepath);Serial.println(" is not a file");
         sprintf(updateInfo2, "ist keine Datei!");
         returnUpdate = -1;
         updateBin.close();
         return;
      }

      size_t updateSize = updateBin.size();

      if (updateSize > 0) {
         Serial.print("Try to start update with: ");Serial.println(filepath);
         performUpdate(updateBin, updateSize);
      }
      else {
         Serial.println("Error, file is empty");
         sprintf(updateInfo2, "Datei ist leer!");
         returnUpdate = -1;
      }

      updateBin.close();
    
      // whe finished remove the binary from sd card to indicate end of the process
      SD.remove(filepath);      
   }
   else {
      Serial.print("Could not load  ");Serial.print(filepath);Serial.println("  from sd root");
      sprintf(updateInfo2, "Datei nicht gefunden!");
      returnUpdate = -1;
   }
}
bool existsUpdateBin(){
    char filenameBin[24];
    sprintf(filenameBin, "/update/EMD_1.bin");
    if (SD.exists(filenameBin)) return true;
    else return false;
}
void checkUpdateBin(char * filepath){
    findUpdate = 1;
    while(findUpdate > 0){
      if (existsUpdateBin()) break;
      findUpdate--;
    }
    sprintf(filepath, "/update/EMD_1.bin",findUpdate);
    sprintf(updateInfo1, "Update: EMD_1.bin",findUpdate);
}
bool checkSdCard(){
  if (SD.exists("/"))return true;
  else return false;
}
