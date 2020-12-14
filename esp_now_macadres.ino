#include <ESP8266WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP8266 Kart MAC Adresi:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
