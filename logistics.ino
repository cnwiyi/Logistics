#include "sms.h"
#include "gps.h"

//const char* remoteNumber = ""; // Todo: make configurable 
char url[256];

void setup() {
   Serial.begin(9600);
   Serial.println("============================  Setup  ===============================");   
   led_setup();
   led_blink_f(2);
   sms_setup();
   gps_setup();
   drv_setup();
   Serial.println("====================================================================");
   led_blink_f(2);
}

void loop() {
  if (gps_getInfo() > 3) {
    drv_write(url);
  }
  else {
    Serial.println("Less then 4 satelites");
  }
  
  sms_readMsg();  
  Serial.println("====================================================================");
  
  delay(10000); // 10 seconds hold
}
