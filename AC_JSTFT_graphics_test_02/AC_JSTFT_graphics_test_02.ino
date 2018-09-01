/***************************************************

 ****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "Adafruit_miniTFTWing.h"

Adafruit_miniTFTWing ss;

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     9  //moved pins
#define TFT_RST    -1  // you can also connect this to the Arduino reset
// in which case, set this #define pin to -1!
#define TFT_DC     10  //moved pins

// For 0.96" and 1.44" and 1.8" TFT with ST7735 use
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

Adafruit_SHT31 sht31 = Adafruit_SHT31();

float p = 3.1415926;

void setup(void) {
  ss.begin();
  ss.tftReset();
  ss.setBacklight(0x7777); //set the backlight to something
  // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
  tft.initR(INITR_MINI160x80);   // initialize a ST7735S chip, mini display
  tft.setTextSize(2);
  tft.fillScreen(ST77XX_BLACK);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);

  sht31.begin(0x44);

}

void loop() {
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_RED);
  float t = 1.8 * sht31.readTemperature() + 32; //with Fahrenheit conversion
  float h = sht31.readHumidity();
  tft.fillScreen(ST77XX_BLACK);
  tft.println(t);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(h);

}



float runningAverage(float M) {
#define LM_SIZE 10
  static float LM[LM_SIZE];    // LastMeasurements
  static byte index = 0;
  static float sum = 0;
  static float count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return (sum / count);
}
