#include <Adafruit_CircuitPlayground.h>

bool buttonL;
bool buttonR;
bool buttonLold;
bool buttonRold;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {

  buttonL = CircuitPlayground.leftButton();
  buttonR = CircuitPlayground.rightButton();
  delay(10);

  if (buttonL == buttonLold) {
    Serial.print("Left Button: ");
    if (buttonL) {
      Serial.print("DOWN");
      digitalWrite(13, HIGH );
    } else {
      Serial.print("  UP");
      digitalWrite(13, LOW );
    }
    Serial.println();
    buttonLold = !buttonL;
  }

  if (buttonR == buttonRold) {
    Serial.print("   Right Button: ");
    if (buttonR) {
      Adown();
      Serial.print("DOWN");
      digitalWrite(13, HIGH );
    } else {
      Serial.print("  UP");
      digitalWrite(13, LOW );
    }
    Serial.println();
    buttonRold = !buttonR;
  }

}


