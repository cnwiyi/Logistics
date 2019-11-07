#include <LGPS.h>

gpsSentenceInfoStruct info;

char buff[256];
unsigned long t = 3000;
unsigned long change;
byte scrno;
boolean main = true;

static unsigned char getComma(unsigned char num, const char *str) {
  unsigned char i, j = 0;
  int len = strlen(str);
  for (i = 0; i < len; i ++) {
    if (str[i] == ',')
      j++;
    if (j == num)
      return i + 1;
  }
  return 0;
}

static double getDoubleNumber(const char *s) {
  char buf[10];
  unsigned char i;
  double rev;

  i = getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev = atof(buf);
  return rev;
}

static double getIntNumber(const char *s) {
  char buf[10];
  unsigned char i;
  double rev;

  i = getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev = atoi(buf);
  return rev;
}

Cordinates_t parseGPGGA(const char* GPGGAstr) {
  double  latitude, longitude;
  char    latdir, londir;
  double  dilution, alt, hg;
  int     tmp, hour, minute, second;
  Cordinates_t cordinates;

  if (GPGGAstr[0] == '$') {
    tmp = getComma(1, GPGGAstr);
    hour     = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
    minute   = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
    second    = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');

    sprintf(buff, "UTC timer %2d-%2d-%2d", hour, minute, second);
    Serial.println(buff);

    tmp = getComma(2, GPGGAstr);
    latitude = getDoubleNumber(&GPGGAstr[tmp]);
    tmp = getComma(3, GPGGAstr);
    latdir = GPGGAstr[tmp];
    
    tmp = getComma(4, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp]);
    tmp = getComma(5, GPGGAstr);
    londir = GPGGAstr[tmp];

    int latdeg = latitude / 100;
    int londeg = longitude / 100;
    double latmin = latitude - (((double)latdeg) * 100);
    double lonmin = longitude - (((double)londeg) * 100);

    if (latdir=='N')
      cordinates.latitude = latdeg+(latmin/60);
    else 
      cordinates.latitude = -1*(latdeg+(latmin/60));
    
    if (londir=='E')
      cordinates.longitude =  londeg+(lonmin/60);
    else
      cordinates.longitude = -1*(londeg+(lonmin/60));

    tmp = getComma(7, GPGGAstr);
    cordinates.satalites = getIntNumber(&GPGGAstr[tmp]);
    sprintf(buff, "Satellites number = %d ", cordinates.satalites);
    Serial.println(buff);

//    sprintf(buff, "http://maps.google.com/maps?q=%5.6f,%5.6f",
//                  (latdir=='N')? latdeg+(latmin/60) : -1*(latdeg+(latmin/60)), 
//                  (londir=='E')? londeg+(lonmin/60) : -1*(londeg+(lonmin/60)));
    sprintf(buff, "http://maps.google.com/maps?q=%5.6f,%5.6f", cordinates.latitude, cordinates.longitude);
    Serial.println(buff);

    strcpy(url,buff); // save global

    tmp = getComma(8, GPGGAstr);
    dilution = getDoubleNumber(&GPGGAstr[tmp]);
//    sprintf(buff, "Horizontal dilution = %10.4f ", dilution);
//    Serial.println(buff);

    tmp = getComma(9, GPGGAstr);
    alt = getDoubleNumber(&GPGGAstr[tmp]);
//    sprintf(buff, "Altitude (meters) = %10.4f ", alt);
//    Serial.println(buff);

    tmp = getComma(11, GPGGAstr);
    hg = getDoubleNumber(&GPGGAstr[tmp]);
//    sprintf(buff, "Height from mean sea level (geoid) = %10.4f", hg);
//    Serial.println(buff);
  }
  else {
    Serial.println("Not get data");
  }

  return cordinates;
}

void gps_setup() {
  Serial.println("Turning GPS power on");
  LGPS.powerOn();
  Serial.print("GPS Powering on, waiting 3 seconds");
  delay(3000);
  Serial.println("...GPS ready");
}

int gps_getInfo() {
  LGPS.getData(&info);
  Serial.print("Raw GPS Data: ");
  Serial.println((char*)info.GPGGA);

  Cordinates_t cordinates = parseGPGGA((const char*)info.GPGGA);
  return cordinates.satalites;
}
