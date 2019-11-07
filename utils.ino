#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>

//uncomment the storage you want to use
#define Drv LFlash          // use Internal 10M Flash
// #define Drv LSD           // use SD card

char file[15] = "rtrv-log.txt";

void drv_setup()
{
  Serial.print("Initializing memory...");
  pinMode(10, OUTPUT); //needed for SD card
  if(!Drv.begin()) {
    Serial.println("Error initalizing memory.");  
  }
  else
    Serial.println("Driver ready");
}

void drv_write(String str) 
{
  LFile dataFile = Drv.open(file, FILE_WRITE);
  
  if (dataFile) {
    dataFile.println(str);
    dataFile.close();
    Serial.println("File written.");
   }
   else 
     Serial.println("Error opening file.");
}
