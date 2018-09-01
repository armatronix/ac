/* Frigidair Gallery/Cool Connect Air Conditioner Comfort Assistant
    uses Circuit Payground Express from Adafruit to maintain room temperature @ the sensor to within a specified tolerance.
*/

#include <Adafruit_CircuitPlayground.h>

#if !defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS)
#error "Infrared support is only for the Circuit Playground Express, it doesn't work with the Classic version"
#endif

/* IR signals consist of a protocol number, a value, and a number of bits.
    Store all of these values for future use.
*/
uint8_t IR_protocol;
uint32_t IR_value;
uint16_t IR_bits;

//Frigidaire remote control codes
uint32_t onOff = 0x10AF8877;
uint32_t fanPlus = 0x10AF807F;
uint32_t fanMinus = 0x10AF20DF;
uint32_t fanAuto = 0x10AFF00F;
uint32_t modeCool = 0x10AF906F;
uint32_t modeEco = 0x10AF40BF;
uint32_t modeFan = 0x10AFE01F;
uint32_t tempPlus = 0x10AF708F;
uint32_t tempMinus = 0x10AFB04F;

bool onState = 1;
bool fanState = 0;
float tempF;
float newTemp;
float topTemp = 82;
float tuneTemp = 0;
float toleranceTemp = .5;
bool buttonL;
bool buttonR;
bool buttonLold;
bool buttonRold;

void setup() {
  CircuitPlayground.begin();
}

void loop() {

  buttonL = CircuitPlayground.leftButton();
  buttonR = CircuitPlayground.rightButton();
  tempF = runningAverage(CircuitPlayground.temperatureF());
  Serial.print(tempF);
  Serial.print(" ,");
  Serial.print(topTemp + tuneTemp);
  Serial.print(" ,");
  Serial.print(topTemp + tuneTemp + .1 * onState);
  Serial.print(" ,");
  Serial.print(topTemp + tuneTemp - .1 * fanState);
  Serial.print(" ,");
  Serial.print(topTemp + tuneTemp + toleranceTemp);
  Serial.print(" ,");
  Serial.println(topTemp + tuneTemp - toleranceTemp);
  digitalWrite(13, onState);

  if (onState == 0 && (tempF > (topTemp + tuneTemp))) {
    CircuitPlayground.irSend.send(1, tempMinus, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    CircuitPlayground.irSend.send(1, onOff, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    CircuitPlayground.irSend.send(1, modeCool, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    CircuitPlayground.irSend.send(1, fanMinus, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    onState = 1;
    fanState = 0;
  }

  if (onState == 1 && (tempF <= (topTemp + tuneTemp - toleranceTemp))) {
    CircuitPlayground.irSend.send(1, tempMinus, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    CircuitPlayground.irSend.send(1, onOff, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    onState = 0;
    fanState = 0;
  }

  if (onState == 1 && fanState == 0 && (tempF >= (topTemp + tuneTemp + toleranceTemp))) {
    CircuitPlayground.irSend.send(1, tempMinus, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    CircuitPlayground.irSend.send(1, fanPlus, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    CircuitPlayground.irSend.send(1, fanPlus, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    fanState = 1;
  }

  delay(20);

  if (buttonL == buttonLold) {
    if (buttonL) {
      if (tuneTemp > -5) {
        tuneTemp = (tuneTemp - .5);
      }
    } else {
    }
    buttonLold = !buttonL;
  }

  if (buttonR == buttonRold) {
    if (buttonR) {
      if (tuneTemp < 5) {
        tuneTemp = (tuneTemp + .5);
      }
    } else {
    }
    buttonRold = !buttonR;
  }

}

float runningAverage(float M) {
#define LM_SIZE 64
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
