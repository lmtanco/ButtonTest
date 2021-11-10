// Button.cpp
//
// Original code by Edouard Renard
// https://roboticsbackend.com/arduino-object-oriented-programming-oop 
#include "Button.h"
#include "avr/wdt.h"

// TODO: need to probably use a default isr which asserts(false) like here https://gist.github.com/jlesech/3089916 
void (*Button::userCallback)() = NULL;
Button* Button::b = NULL;

void Button::privateCallback() {
  if (userCallback) {
      userCallback();
      // TODO: Disable pin interrupt
      // TODO: Enable WDT interrupt
    
  }

// TODO: WDT ISR and change FALLING to RISING, etc. 

Button::Button(byte pin) {
  this->pin = pin;
  lastReading = HIGH;
  init();
  b=this;
}

void Button::init() {
  pinMode(pin, INPUT_PULLUP);
  update();
  // TODO: Need to think what to do if pins are not the right ones. 
}

void Button::update() {
    // You can handle the debounce of the button directly
    // in the class, so you don't have to think about it
    // elsewhere in your code
    byte newReading = digitalRead(pin);
    
    if (newReading != lastReading) {
      lastDebounceTime = millis();
    }

    if (millis() - lastDebounceTime > debounceDelay) {
      // Update the 'state' attribute only if debounce is checked
      state = newReading;
    }

    lastReading = newReading;
}

byte Button::getState() {
  update();
  return state;
}

bool Button::isPressed() {
  return (getState() == LOW);
}

void Button::setInterrupt(void(*isr)()) {
  userCallback = isr;
  attachInterrupt(digitalPinToInterrupt(pin), privateCallback, FALLING);
  //attachInterrupt(digitalPinToInterrupt(pin), isr, FALLING);
}
