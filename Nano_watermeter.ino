#include <EEPROM.h>
const int analogInPin = A7;  // Analog input pin that the photo transistor is attached to
const int irOutPin = 3; // Digital output pin that the IR-LED is attached to
const int ledOutPin = 12; // Signal LED output pin
int triggerLevelLow = 30;
int triggerLevelHigh = 60;
boolean triggerState = false;
long counter;

int sensorValue = 0;  // value read from the photo transistor when ir LED is off
long start = 25690;
void eepromWriteLong(long lo, int adr) {
// long Wert in das EEPROM schreiben  
// Eingabe : adr Speicherplatz 
// Eingabe : lo Zahl, Wertebereich -2.147.483.648 bis 2.147.483.647
//
// Matthias Busse 23.5.2014 Version 1.0

byte by;

  for(int i=0;i< 4;i++) {
    by = (lo >> ((3-i)*8)) & 0x000000ff; 
    EEPROM.write(adr+i, by);
  }
} // eepromWriteLong

long eepromReadLong(int adr) {
// long int Wert aus 4 Byte EEPROM lesen
// Eingabe : adr bis adr+3
// Ausgabe : long Wert
// 
// Matthias Busse 23.5.2014 Version 1.0

long lo=0;

  for(int i=0;i< 3;i++){
    lo += EEPROM.read(adr+i);
    lo = lo << 8;
  }
  lo += EEPROM.read(adr+3);
  return lo;
} // eepromReadLong



void setup()
{
  pinMode(irOutPin, OUTPUT);
  pinMode(ledOutPin, OUTPUT);
  digitalWrite(ledOutPin, LOW);
  digitalWrite(irOutPin, HIGH);
  Serial.begin(9600);
  eepromWriteLong(start,1);
}

void loop()
{
  //Test
  sensorValue = analogRead(analogInPin);
  if (sensorValue > triggerLevelHigh)
  {  
    if (triggerState == false)
    {
      triggerState = true;
      digitalWrite(ledOutPin, HIGH);
    }
  } else if (sensorValue < triggerLevelLow)
  {
    if (triggerState == true)
    {
      triggerState = false;
      digitalWrite(ledOutPin, LOW);  
      counter=eepromReadLong(1);
      counter = counter+1;
      eepromWriteLong(counter,1);
      Serial.println(counter);

      
    }    
  }
  delay(100);  
}
