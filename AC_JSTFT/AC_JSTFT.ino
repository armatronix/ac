/***************************************************
  Uses TFT Joystick Wing - cut 5/6 and bridge 9/10 on underside

 ****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "Adafruit_miniTFTWing.h"
#include <IRLibAll.h>            // First include the send base

Adafruit_miniTFTWing ss;
IRsend My_Sender;

//---------------------------------------------------------------------------
#define TFT_CS     9
#define TFT_DC     10
#define TFT_RST    -1  // you can also connect this to the Arduino reset
// in which case, set this #define pin to -1!

// For 0.96" and 1.44" and 1.8" TFT with ST7735 use
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

Adafruit_SHT31 sht31 = Adafruit_SHT31();


//Frigidaire remote control codes--------------------------------------------
uint32_t onOff = 0x10AF8877;
uint32_t fanPlus = 0x10AF807F;
uint32_t fanMinus = 0x10AF20DF;
uint32_t fanAuto = 0x10AFF00F;
uint32_t modeCool = 0x10AF906F;
uint32_t modeEco = 0x10AF40BF;
uint32_t modeFan = 0x10AFE01F;
uint32_t tempPlus = 0x10AF708F;
uint32_t tempMinus = 0x10AFB04F;

// variables -------------------------------------------------
int hGraphOld = 0;
int n = 0;

uint8_t IR_protocol;
uint32_t IR_value;
uint16_t IR_bits;

bool onState = 1;
bool fanState = 0;
float t;
float h;
int d = 10;
float setTemp = 83;
float swing = .5;
int blTimer = 0;
int blTime = 5;
int tGraph;
int hGraph;

//-------------------------------------------------------------
void setup(void) {
  pinMode(5, OUTPUT); //IR Receiver Enable
  pinMode(6, OUTPUT); //IR Receiver Enable
  pinMode(11, INPUT); //IR Receiver Output
  pinMode(12, OUTPUT); //IR Receiver Enable
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(12, LOW);

  sht31.begin(0x44);
  ss.begin();
  ss.tftReset();
  tft.initR(INITR_MINI160x80);   // initialize a ST7735S chip, mini display
  tft.setRotation(3); //sensor on top

  tft.fillScreen(ST77XX_BLACK);
  writeSetTemp();
  tft.drawFastHLine(0, 20, 180, ST77XX_RED);
  tft.drawFastHLine(0, 60, 180, ST77XX_BLUE);
  writeOn();
}

// ==============================================================

void loop() {
  for (n = 50; n < 150; n++) {
    if (blTimer < blTime) blTimer++;
    if (blTimer >= blTime) dim();
    readUI();
    readT();
    readUI();
    readH();
  }
  refresh();
}

// ==============================================================

void sendIR(uint32_t code) {
  My_Sender.send(1, code, 32);
  delay(d);
  My_Sender.send(1, 0xFFFFFFFF, 0);
  delay(d);
}

// read (+ write) functions ----------------------
void readUI(void) {
  uint32_t buttons = ss.readButtons();
  if (! (buttons & TFTWING_BUTTON_UP)) {
    setTemp += .5;
    writeSetTemp();
//    ss.EEPROMWrite8(0x01, setTemp); // writing setTemp to register 0x01
    blTimer = 0;
    ss.setBacklight(0x0000);
  }
  if (! (buttons & TFTWING_BUTTON_DOWN)) {
    setTemp -= .5;
    writeSetTemp();
//    ss.EEPROMWrite8(0x01, setTemp); // writing setTemp to register 0x01
    blTimer = 0;
    ss.setBacklight(0x0000);
  }
  if (! (buttons & TFTWING_BUTTON_A)) {
    sendIR(tempMinus);
    sendIR(onOff);
    blTimer = 0;
    ss.setBacklight(0x0000);
  }
  if (! (buttons & TFTWING_BUTTON_B)) {
    blTimer = 0;
    ss.setBacklight(0x0000);
  }
  if (! (buttons & TFTWING_BUTTON_LEFT)) {
    blTimer = 0;
    ss.setBacklight(0x0000);
  }
  if (! (buttons & TFTWING_BUTTON_RIGHT)) {
    blTimer = 0;
    ss.setBacklight(0x0000);
  }
  if (! (buttons & TFTWING_BUTTON_SELECT)) {
    blTimer = 0;
    ss.setBacklight(0x0000);
  }
  writeOn;
}

void readT(void) {
  t = 1.8 * sht31.readTemperature() + 32; // with Fahrenheit conversion
  if (onState == 0 && (t > (setTemp + swing))) {
    sendIR(tempMinus);
    sendIR(onOff);
    sendIR(modeCool);
    sendIR(fanMinus);
    onState = 1;
    fanState = 0;
  }
  if (onState == 1 && (t <= (setTemp - swing))) {
    sendIR(tempMinus);
    sendIR(onOff);
    onState = 0;
    fanState = 0;
  }
  if (onState == 1 && fanState == 0 && (t >= (setTemp + 2 * swing))) {
    sendIR(tempMinus);
    sendIR(fanPlus);
    sendIR(fanPlus);
    fanState = 1;
  }
  writeT();
  writeOn();
}

void readH(void) {
  h = sht31.readHumidity();
  writeH();
}

// write functions -------------------------------
void writeSetTemp(void) {
  tft.setTextSize(4);
  tft.setTextColor(0x7777, 0x0000);
  tft.setCursor(0, 27);
  tft.print(int(setTemp));
  tft.setTextSize(2);
  if ((setTemp - int(setTemp)) >= .5) {
    tft.print(".5");
  }
  else {
    tft.print("  ");
  }
}

void writeT(void) {
  tft.setTextSize(2);
  tft.setCursor(100, 0);
  tft.setTextColor(ST77XX_RED, 0x0000);
  tft.println(t);
  tGraph = (40 - map(t * 100, (setTemp - swing) * 100, (setTemp + swing) * 100, -20.0, 20.0));
  tft.drawPixel(n, tGraph, 0xFFFF);
}

void writeH(void) {
  tft.setCursor(100, 65);
  tft.setTextColor(ST77XX_GREEN, 0x0000);
  tft.print(h);

  int hGraph = (40 - map(h * 100, 40 * 100, 60 * 100, -20, 20));
  tft.drawFastHLine(150, hGraphOld, 10, 0x0000);
  tft.drawFastHLine(150, hGraph, 10, ST77XX_GREEN);
  hGraphOld = hGraph;
}

void writeOn(void) {
  tft.setTextSize(2);
  if (fanState && onState) {
    tft.setTextColor(ST77XX_BLACK, ST77XX_YELLOW);
    tft.setCursor(0, 0);
    tft.print("*ON*");
  }
  if (onState & !fanState) {
    tft.setTextColor(0xFFFF, ST77XX_BLUE);
    tft.setCursor(0, 0);
    tft.print("*on*");
  }
  if (!onState) {
    tft.setTextColor(0xFFFF, 0x0000);
    tft.setCursor(0, 0);
    tft.print("    ");
  }
}

void refresh(void) {
  tft.fillScreen(ST77XX_BLACK);
  writeSetTemp();
  writeT();
  writeH();
  tft.drawFastHLine(0, 20, 180, ST77XX_RED);
  tft.drawFastHLine(0, 60, 180, ST77XX_BLUE);
  writeOn();
}

void dim(void) {
  ss.setBacklight(0xFFFF);
}

