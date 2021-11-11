// Button.cpp
//
// Original code by Edouard Renard
// https://roboticsbackend.com/arduino-object-oriented-programming-oop
#include "Button.h"
#include "avr/wdt.h"

// TODO: need to probably use a default isr which asserts(false) like here https://gist.github.com/jlesech/3089916
void (*Button::userCallback)() = NULL;
byte change = FALLING;  // TODO ALLOW FOR OTHER PINS
void Button::debounceAndCallback() {
  if (userCallback) {
    // Disable pin interrupt. TODO: allow for other pins.
    detachInterrupt(digitalPinToInterrupt(2));
    if (change == FALLING) {
      userCallback();
      change = RISING;
    } else {
      change = FALLING;
    }
    // Reset watchdog.
    wdt_reset();
    // Enable changes in WDT control
    WDTCSR = (1 << WDCE) | (1 << WDE);
    // Start watchdog timer interrupt with 32ms prescaler
    WDTCSR = (1 << WDIE) | (1 << WDP0);
  }
}
// TODO: WDT ISR and change FALLING to RISING, etc.
ISR(WDT_vect) {
  // Reenable interrupt at pin for current type of change.
  attachInterrupt(digitalPinToInterrupt(2), Button::debounceAndCallback, change);
  // Disable watchdog interrupt
  wdt_reset();
  // Enable changes in WDT control
  WDTCSR = (1 << WDCE) | (1 << WDE);
  // Disable
  WDTCSR = 0;
}

Button::Button(byte pin) {
  this->pin = pin;
  lastReading = HIGH;
  init();
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

void Button::setInterrupt(void (*isr)()) {
  userCallback = isr;
  attachInterrupt(digitalPinToInterrupt(pin), debounceAndCallback, FALLING);
}
