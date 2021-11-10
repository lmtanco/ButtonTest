#include "Button.h"

Button b(2);
byte ledState = LOW;
void rti_2()
{
  ledState ^= 1;
  digitalWrite(13,ledState);
}



void setup() {
  pinMode(13, OUTPUT);
  pinMode(2,INPUT_PULLUP);
  digitalWrite(13,LOW);
  b.setInterrupt(rti_2);
  Serial.begin(9600);
}

void loop() {
  Serial.println(ledState);
  delay(2);
}

