#include "DHT.h"
#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>

/*
   LCM1602 & Arduino Uno
   VCC - > 5 V
   GND - GND
   SCL -> A5
   SDA -> A4


   DHT11:
   VCC -> 5V
   GND -> GND
   DAT -> DIGITAL 2
*/



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27

DHT dht;

int tempValue = 0;
int prevTempValue = 0;

int humidValue = 0;
int prevHumidValue = 0;


const int DHT11_PIN = 2;

int pinButton = 8; // button pin
int displayMode = 0; // 0 - normal mode, 1 - temperature mode, 2 - humidity mode
const int MAX_MODE = 2;
int timer = 0;
int maxTimer = 25;


void printOnLCD(int val1, int prevVal1, int val2, int prevVal2, int mode) {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
  switch (mode) {
    case 1: // TEMP 
      lcd.print("Temper:  ");
      lcd.print(val1);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Roznica: ");
      lcd.print(val1 - prevVal1);
      break;
    case 2: // HUMID
      lcd.print("Wilgot:  ");
      lcd.print(val2);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Roznica: ");
      lcd.print(val2 - prevVal2);
      break;
    default: // ALL
      lcd.print("Temper: ");
      lcd.print(val1);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Wilgot: ");
      lcd.print(val2);
      lcd.print("%");
      break;
  }
}


void setup() {

  pinMode(pinButton, INPUT); //button pin as an input

  // DHT11
  Serial.begin(9600);
  dht.setup(DHT11_PIN);

  // LCD & I2C
  lcd.begin(16, 2);  // LCD 2x16 Initialization

}

void loop() {

  if (timer > maxTimer) {

    prevTempValue = tempValue;
    tempValue = dht.getTemperature();

    prevHumidValue = humidValue;
    humidValue = dht.getHumidity();

    printOnLCD(tempValue, prevTempValue, humidValue, prevHumidValue, displayMode);
    timer = 0;
  }

  int stateButton = digitalRead(pinButton);

  if (stateButton == 1) {
    displayMode++;

    if (displayMode > MAX_MODE)
      displayMode = 0;

    printOnLCD(tempValue, prevTempValue, humidValue, prevHumidValue, displayMode);
  }

  delay(200);
  timer += 1;

}
