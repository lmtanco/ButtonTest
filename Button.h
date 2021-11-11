// Button.h
//
// Original code by Edouard Renard
// https://roboticsbackend.com/arduino-object-oriented-programming-oop 

#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <Arduino.h>

class Button {
  
  private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
    static void (*userCallback)();
    static Button* b;
    
  public:
    Button(byte pin);

    void init();
    void update();

    byte getState();
    bool isPressed();

    void setInterrupt(void(*isr)());
    static void debounceAndCallback();
};

#endif