#include "ADCTouch.h"
#include "SoftPWM.h"

//#define DEBUG

#define MODE1 200
#define MODE2 500
#define MODE3 780

#define SIZE 3

const static int sensorCh[SIZE] = {A0,A1,A2};
static int base[SIZE] = {0};

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  SoftPWMBegin();
  digitalWrite(LED_BUILTIN, HIGH);
  for(char i = 0 ; i < SIZE ; i++)
    base[i] = ADCTouch.read(sensorCh[i],500);
  //Serial.begin(230400);
  Serial.begin(2000000);
  while (!Serial);
  SoftPWMSet(LED_BUILTIN, 0);
  SoftPWMSetFadeTime(LED_BUILTIN, 10, 10);
}

void loop(){
  static unsigned tmp;
  unsigned total = 0;
  for(unsigned char i = 0 ; i < SIZE ; i++){ //take approx. 135-200ms
    const int tmp = ADCTouch.read(sensorCh[i]) - base[i];
    unsigned val = tmp < 0? 0 : tmp;
    total += val;
  }
  const char mode = total>MODE3? '3' : total>MODE2? '2' : total>MODE1? '1' : '0';
  SoftPWMSetPercent(LED_BUILTIN, mode == '3'? 100 : mode == '2' ? 25 : mode == '1' ? 5 : 0);
#ifdef DEBUG
  char buf[5] = {'\0'};
  const auto len = sprintf(buf, "%u\n", total);
  Serial.write(buf, len);
#else
  Serial.write(&mode, 1);
#endif
}
