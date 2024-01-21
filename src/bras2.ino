#include "RotaryEncoder.h"
#include <SoftwareSerial.h>

#define BUTTON_PIN 1
#define MAX_ANGLE 180
#define MIN_ANGLE -180
#define k 50

RotaryEncoder encoder1(4, 5);  // (DT, CLK) BASE_ROTATION
RotaryEncoder encoder2(8, 9);  // (DT, CLK) BRAS_2
RotaryEncoder encoder3(2, 3);  // (DT, CLK) PINCE_BASE 
RotaryEncoder encoder4(6, 7);  // (DT, CLK) BRAS_1
RotaryEncoder encoders[4] = {encoder3, encoder4, encoder2, encoder1} ;

SoftwareSerial BTSerial(13, 12); // Changed TX, RX pins to 13, 12

int pos[4] = {0,145,90,120};
int newPos[4] = {-1,-1,-1,-1};
int oldPosPasAPas = 0;
bool isOpen = true;
bool oldValueButton = false;
int index = 0;
int last100Values[k]; 

void setup() {
  BTSerial.begin(38400);
  Serial.begin(9600); // ouvre le port série
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println(serialisedString(pos));
}

void loop() {
  updateValues();
}


void updateValues(){

  encoders[0].tick();
  int newpos = encoders[0].getPosition();
  if (newpos != oldPosPasAPas){
    pos[0] = newpos - oldPosPasAPas;
    oldPosPasAPas = newpos;
    Serial.println(serialisedString(pos));
    BTSerial.write(serialisedString(pos).c_str());
  }
  else{
    pos[0] = 0;
  }
  
  for (int i = 1; i < 4; i++){
      encoders[i].tick();
      int newpos = encoders[i].getPosition();
      if (pos[i] != newpos) {
        pos[i] = newpos; 
        Serial.println(serialisedString(pos));
        BTSerial.write(serialisedString(pos).c_str());
      }
    }
  updateIsOpen();
}

int encoderValueToAngle(int value){
  return map(value%20, -20, 20, MIN_ANGLE, MAX_ANGLE);
}

String serialisedString(int pos[4]){
  int tab[4] = {
      0,
      110, // min 45 - 145
      120, // min 40 - 180
      120 // min 60 - 270
  };
  String s = "";
  s += String(pos[0]*10)+":";
  s += String(-encoderValueToAngle(pos[1])+tab[1])+":";
  s += String(encoderValueToAngle(pos[2])+tab[2])+":";
  s += String(-encoderValueToAngle(pos[3])+tab[3])+":";
  s += isOpen;
  s += "q";
  return s;
}

void updateIsOpen()
{
  bool buttonState = analogRead(BUTTON_PIN) <= 0;
  last100Values[index] = buttonState;
  index = (index+1)%k;
  int valueButton = sumTab(last100Values, k);

  if(valueButton != oldValueButton){
    oldValueButton = valueButton;
    if (valueButton){
      isOpen = !isOpen;
      Serial.println(serialisedString(pos));
      BTSerial.write(serialisedString(pos).c_str());
    }
  }
}

bool sumTab(int tab[], int len){
  int sum = 0;
  for (int i = 0; i< len; i++){
    sum += tab[i];
  }
  return sum > len/2;
}
