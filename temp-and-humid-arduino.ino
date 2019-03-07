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
*  LCM1602 & Arduino Uno
*  VCC - > 5 V
*  GND - GND
*  SCL -> A5
*  SDA -> A4
*

*  DHT11:
*  VCC -> 5V
*  GND -> GND
*  DAT -> DIGITAL 2
*/



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27

DHT dht;

float tempValue = 0.0f;
float prevTempValue = 0.0f;

float humidValue = 0.0f;
float prevHumidValue = 0.0f;


const int DHT11_PIN = 2;

int pinButton = 8; // button pin
int displayMode = 0; // 0 - normal mode, 1 - temperature mode, 2 - humidity mode
int timer = 0;
int maxTimer = 10;




void printOnLCD(float val1, float prevVal1, float val2, float prevVal2, int mode) {
   String firstFloatChar  = String(6);
   String secondFloatChar = String(6);
   String firstLine  = String(15);
   String secondLine = String(15);

   switch(mode) {
      case 1: // TEMP
         firstFloatChar = String(val1, 2);
         firstLine = String("Temp:    " + firstFloatChar);
         secondFloatChar = String(val1 - prevVal1, 2);
         secondLine = "Roznica: " + secondFloatChar;
         break;
      case 2: // HUMID
         firstFloatChar = String(val2, 2);
         firstLine = String("Wilg:    " + firstFloatChar);
         secondFloatChar = String(val2 - prevVal2, 2);
         secondLine = "Roznica: " + secondFloatChar;
         break;
      default: // ALL
         firstFloatChar = String(val1, 2);
         firstLine = String("Temp: " + firstFloatChar);
         secondFloatChar = String(val2, 2);
         secondLine = "Wilg: " + secondFloatChar;
         break;
   }
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print(firstLine);
   lcd.setCursor(0, 1);
   lcd.print(secondLine);
}


void setup() {

   pinMode(pinButton, INPUT); //button pin is input
  
   // DHT11
   Serial.begin(9600);
   dht.setup(DHT11_PIN);

   // LCD & I2C
   lcd.begin(16,2);   // Inicjalizacja LCD 2x16

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

   int stateButton = digitalRead(pinButton); //read the state of the button

   if (stateButton == 1) {
      displayMode++;
      if (displayMode > 2)
         displayMode = 0;
      printOnLCD(tempValue, prevTempValue, humidValue, prevHumidValue, displayMode);
   }

   delay(200);
   timer += 1;

}
