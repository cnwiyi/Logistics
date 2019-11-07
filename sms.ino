#include <LGSM.h>
#include <string.h>

char messageConfirm[] = "on/off/gps/config/setup";

bool sms_setup() {
  Serial.println("Intialize SIM for SMS");

  boolean s_ready = false;
  
  if (LSMS.ready()) {
    s_ready = true;
    Serial.println("SIM ready for sending SMS");
  } 
  else {
    Serial.println("SIM is missing");
  }

  return s_ready;
}

bool sms_sendMsg (const char* rNumber, char* message) {
  bool rc = false;
   
  Serial.print("Tel: ");
  Serial.println(rNumber);
  Serial.print("Msg: ");
  Serial.println(message);
        
  LSMS.beginSMS(rNumber);
  LSMS.print(message);
  
  if (LSMS.endSMS()) {
    Serial.println("SMS sent successfully");
    led_blink_n(1);
    rc = true;
  } 
  else {
    Serial.print("SMS failed to send");
    rc = false;
  }

  return rc;
}

void sms_readMsg () {
  char p_num[20];
  int len = 0;
  char message[500];

  // Check if there is new SMS 
  if(LSMS.available()) {
    led_blink_n(3);
    LSMS.remoteNumber(p_num, 20); // display Number part
    Serial.println("There is new message.");
        
    Serial.print("Number:");
    Serial.println(p_num);
    Serial.print("Content:"); // display Content part     

    while(true) {
      int v = LSMS.read();
      if(v < 0)
      break;

      message[len++] = (char)v;
      Serial.print((char)v);
    }

    // change to uppercase
    for(int i=0; message[i]; i++)
      message[i] = toupper(message[i]);

    Serial.println();
        
    LSMS.flush(); // delete message
        
    if(message[0] == 'O' && message[1] == 'N') {
      Serial.println("On - LED");
      led_turnOn();
    }
    else if(message[0] == 'O' && message[1] == 'F' && message[2] == 'F') {
      Serial.println("Off - LED");
      led_turnOff();
    }
    else if (message[0] == 'G' && message[1] == 'P' && message[2] == 'S') {
      Serial.println("GPS requested via SMS");
      sms_sendMsg(p_num, url);
//      sprintf(message, "Satalites: %d", num);
//      sms_sendMsg(p_num, message);
    }
    else if (message[0] == 'C' && message[1] == 'O' && message[2] == 'N' && message[3] == 'F' && message[4] == 'I' && message[5] == 'G') {
      Serial.println("Not supported yet");
    }
    else if (message[0] == 'S' && message[1] == 'E' && message[2] == 'T' && message[3] == 'U' && message[4] == 'P') {
      Serial.println("Not supported yet");
    }
    else {
      sms_sendMsg(p_num, messageConfirm); 
      Serial.print("Commands: on, off, gps, config. Error: "); 
      Serial.println(message);      
    }
  }
}
