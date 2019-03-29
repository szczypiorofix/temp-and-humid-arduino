#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>

#include <DHT.h>
#include <DHT_U.h>


/*

   LCM1602 & Arduino Uno
   VCC -> 5 V
   GND -> GND
   SCL -> A5
   SDA -> A4


   DHT11:
   VCC -> 5V
   GND -> GND
   DAT -> DIGITAL 2

*/


// ------------------------------ LCD ------------------------------

/**
    @param      lcd_Addr[in] I2C address of the IO expansion module. For I2CLCDextraIO,
                the address can be configured using the on board jumpers.
    @param      En[in] LCD En (Enable) pin connected to the IO extender module
    @param      Rw[in] LCD Rw (Read/write) pin connected to the IO extender module
    @param      Rs[in] LCD Rs (Reset) pin connected to the IO extender module
    @param      d4[in] LCD data 0 pin map on IO extender module
    @param      d5[in] LCD data 1 pin map on IO extender module
    @param      d6[in] LCD data 2 pin map on IO extender module
    @param      d7[in] LCD data 3 pin map on IO extender module
   
LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, 
                  uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 );
LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, 
                  uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                  uint8_t backlighPin, t_backlighPol pol); */
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int LCD_ROWS = 2;
const int LCD_COLS = 16;

// ------------------------------ DHT ------------------------------
#define DHT_PIN 2
DHT dht(DHT_PIN, DHT11);


const int DHT11_PIN = 2;

const int TEMP_MINSCALE = 15;
const int TEMP_MAXSCALE = 30;


// ------------------------------ TIMER ------------------------------
int timer = 0;
int maxTimer = 15; // 25 x 200 delay


// --------------------------- CUSTOM CHARS ------------------------------
byte customChar0[] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110
};

byte customChar1[] = {
  B00000,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000
};

byte customChar2[] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};

byte customChar3[] = {
  B00000,
  B11110,
  B00001,
  B00001,
  B00001,
  B00001,
  B11110,
  B00000
};


void setup() {

  // DHT
  dht.begin();

  // LCD & I2C
  lcd.begin(LCD_COLS, LCD_ROWS);  // LCD 2x16 Initialization
  lcd.createChar(0, customChar0);
  lcd.createChar(1, customChar1);
  lcd.createChar(2, customChar2);
  lcd.createChar(3, customChar3);
}


void displayOnLCD(int temp, int humid) {
  lcd.print((char) 0);
  for (int i = 0; i < LCD_COLS-2; i++) {
    if (i + (TEMP_MAXSCALE - TEMP_MINSCALE) < temp) {
      lcd.print((char) 2);
    }
    else lcd.print((char) 1);
  }
  lcd.print((char) 3);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humid);
  lcd.print("%");
  lcd.setCursor(LCD_COLS - 6, 1);
  lcd.print("T: ");
  lcd.print(temp);
  lcd.print( (char)223);
  lcd.print("C");
}


void loop() {
  if (timer > maxTimer) {
    lcd.clear();
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      lcd.print("Sensor error!");
      return;
    }
//    float hic = dht.computeHeatIndex(t, h, false);
    displayOnLCD(t, h);
    timer = 0;
  }

  delay(200);
  timer += 1;
}
