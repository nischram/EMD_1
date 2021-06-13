#ifndef __HOMEMATIC_H_
#define __HOMEMATIC_H_

int readStateHM(bool sprint, int ise_id, const char *isename) {
    int retValue = -1;
    if (!wifiTimeout){
      char url[128];
      HTTPClient http;
      http.setTimeout(3000);
      sprintf (url, "http://%s/config/xmlapi/state.cgi?datapoint_id=%i", IP_HM, ise_id);
      http.begin(url);
      int httpCode = http.GET();
      String request = "error";
      if (httpCode > 0) {
        request = http.getString();
      }
      http.end();
      char str[256], *cutOff, *outID, *outValue;
      request.toCharArray(str, 256);
      char delim[] = "'";
      cutOff = strtok(str, delim);
      outID = strtok(NULL, delim);
      cutOff = strtok(NULL, delim);
      outValue = strtok(NULL, delim);
      cutOff = strtok(NULL, delim);
      if (strcmp ("true",outValue) == 0){
          retValue = 1;
      }
      else if (strcmp ("false",outValue) == 0){
          retValue = 0;
      }
      if (sprint){
        Serial.print("read HM ID      :  ");
        Serial.print(outID);
        Serial.print(" = ");
        Serial.print(isename);
        Serial.print(" State: ");
        Serial.println(retValue);
      }
    }
    else {
      retValue= -100;
      if (sprint){
        Serial.print("read HM         :  ");
        Serial.print("Wifi is timeout, ");
        Serial.print(isename);
        Serial.print(" return: ");
        Serial.println(retValue);
      }
    }
    return retValue;
}
int sendHM(bool sprint, int ise_id, const char *isename, const char* format, ...) {
    if (!wifiTimeout){
      va_list argptr;
      char value[128];
      va_start(argptr, format);
      vsprintf(value,  format, argptr);
      
      char url[256];
      HTTPClient http;
      http.setTimeout(3000);
      snprintf (url, (size_t)128, "http://%s/config/xmlapi/statechange.cgi?ise_id=%i&new_value=%s", IP_HM, ise_id, value);
      http.begin(url);
      int httpCode = http.GET();
      String request = "error";
      if (httpCode > 0) {
        request = http.getString();
        if (sprint){
          Serial.print("send to HM       :  \"");Serial.print(value);
          Serial.print("\" ISE_Name : \"");Serial.print(isename);Serial.println("\"");
        }
      }
      if (httpCode != 200) {
        if (sprint){
          Serial.println("send error");
        }
        return -1;
      }
      http.end();
      return 1;
    }
    else {
      if (sprint){
        Serial.print("send to HM       :  ");
        Serial.print("Wifi is timeout, ");
        Serial.println(isename);
      }
    }
    return -200;
}
#endif // __HOMEMATIC_H_
