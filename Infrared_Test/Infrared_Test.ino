#include <Bounce2.h>

/* Infrared_Record.ino Example sketch for IRLib2 and Circuit Playground Express
   Illustrates how to receive an IR signal, decode and save it.
   Then retransmit it whenever you push the left pushbutton.
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
float tempF;
float newTemp;
float topTemp = 90;
float tuneTemp = 0;
float toleranceTemp = 1;
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
  Serial.println(" F: ");

  if (onState == 0 && (tempF > (topTemp - tuneTemp))) {
    CircuitPlayground.irSend.send(1, onOff, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    onState = 1;
    digitalWrite(13, HIGH );
  }

  if (onState == 1 && (tempF <= (topTemp - tuneTemp - toleranceTemp))) {
    CircuitPlayground.irSend.send(1, onOff, 32);
    CircuitPlayground.irSend.send(1, 0xFFFFFFFF, 0);
    onState = 0;
    digitalWrite(13, LOW );
  }

  delay(10);

  if (buttonL == buttonLold) {
    Serial.print("Left Button:  ");
    if (buttonL) {
      Serial.println("DOWN");
      digitalWrite(13, HIGH );
    } else {
      Serial.println("  UP");
      digitalWrite(13, LOW );
    }
    buttonLold = !buttonL;
  }

  if (buttonR == buttonRold) {
    Serial.print("Right Button: ");
    if (buttonR) {
      Serial.println("DOWN");
      digitalWrite(13, HIGH );
    } else {
      Serial.println("  UP");
      digitalWrite(13, LOW );
    }
    Serial.print("  tempF: ");
    Serial.println(tempF);
    buttonRold = !buttonR;
  }

}

float runningAverage(float M) {
#define LM_SIZE 100
  static float LM[LM_SIZE];      // LastMeasurements
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

  return sum / count;
}
















