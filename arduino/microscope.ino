#include <Stepper.h>

int lastPosChange_M2 = 0;

// True if new data are available
volatile bool newData = false;

// True if a motor has been selected
bool motorSelected = false;

// Name of the motor that has been selected
String motorSelection = "M1";

// Variables for command buffering
const byte numChars = 32;
char recievedChars[numChars];

// Number of steps per 360°
const int spr = 2048;

// Stepper initialization
Stepper m1(spr, 8, 10, 9, 11);
Stepper m2(spr, 4, 6, 5, 7);

// LEDs
#define PIN_LED_M1 13
#define PIN_LED_M2 2
#define PIN_LED_M22 22

// Buttons for controling the motors manually
#define PIN_BUTTON_M1 12
#define PIN_BUTTON_M2 3
#define PIN_BUTTON_M22 24

void setup() {
  Serial.begin(9600);

  m1.setSpeed(5);
  m2.setSpeed(5);
  
  // Pin modes
  pinMode(PIN_LED_M1, OUTPUT);
  pinMode(PIN_BUTTON_M1, INPUT_PULLUP);
  pinMode(PIN_LED_M2, OUTPUT);
  pinMode(PIN_BUTTON_M2, INPUT_PULLUP);
  pinMode(PIN_LED_M22, OUTPUT);
  pinMode(PIN_BUTTON_M22, INPUT_PULLUP);
}

void loop() {
  // First of all, handle the manual buttons
  if (digitalRead(PIN_BUTTON_M1) == LOW) {
    digitalWrite(PIN_LED_M1, HIGH);
    m1.step(10);
    
    // Clear all data that might have been read, cause manual mode is more important
    newData = false;
    
    return;
  } else {
    digitalWrite(PIN_LED_M1, LOW);
  }
  if (digitalRead(PIN_BUTTON_M2) == LOW) {
    digitalWrite(PIN_LED_M2, HIGH);
    m2.step(-10);
    
    // Clear all data that might have been read, cause manual mode is more important
    newData = false;
    
    return;
  } else {
    digitalWrite(PIN_LED_M2, LOW);
  }
  if (digitalRead(PIN_BUTTON_M22) == LOW) {
    digitalWrite(PIN_LED_M22, HIGH);
    m2.step(10);
    
    // Clear all data that might have been read, cause manual mode is more important
    newData = false;
    
    return;
  } else {
    digitalWrite(PIN_LED_M22, LOW);
  }
  
  recieveData();
  handleCommand();
}

void recieveData() {
  static byte ndx = 0;

  while (Serial.available() > 0 && newData == false) {
    char rc = Serial.read();
    if (rc != '\n') {
      recievedChars[ndx] = rc;

      if (ndx < numChars - 1) {
        ndx++;
      }
    } else {
      recievedChars[ndx] = '\0';
      ndx = 0;
      newData = true;
    }
  }
}

void handleCommand() {
  if (newData == true) {
    // Data available, test for reset
    if (strcmp(recievedChars, "RESET") == 0) {
      motorSelected = false;
      newData = false;
      return;
    }
    
    if (motorSelected) {
      String runSteps = recievedChars;
      // Motor selected, read run
      if (motorSelection == "M1") {
        m1.step(runSteps.toInt());
      } else if (motorSelection == "M2") {
        int steps = runSteps.toInt();
        if ((lastPosChange_M2 < 0 && steps > 0) ||
            (lastPosChange_M2 > 0 && steps < 0)) {
          if (steps > 0) {
            m2.step(1900);
          } else {
            m2.step(-1900); 
          }
        }
        lastPosChange_M2 = steps;
        m2.step(steps);
      }
      motorSelected = false;
      Serial.write("READY\n");
    } else {
      // Motor not selected, select motor
      if (strcmp(recievedChars, "M1") == 0) {
        motorSelection = "M1";
      } else if (strcmp(recievedChars, "M2") == 0) {
        motorSelection = "M2";
      }
      motorSelected = true;
    }
    newData = false;
  }
}
