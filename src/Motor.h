#include <Arduino.h>

class Motor {
public:
  Motor(int dir1Pin, int dir2Pin, int speedPin);
  ~Motor();

  enum direction {
    stop,
    forward,
    backward
  }; // mogelijke richtigen om in te rijden

  void rename(String newName);
  String getName() { return motorName; };
  void connect(int dir1Pin, int dir2Pin, int speedPin);
  void drive(direction dir, int sp);
  void move(direction dir, int sp, unsigned long durMillis);
  void enableDebuggingOutput() { debugging = true; };
  void disableDebuggingOutput() { debugging = false; };

private:
  byte dir1Pin, dir2Pin, speedPin;
  bool connected = false;
  String motorName = "Motor";
  bool debugging = true;
  byte maxSpeed = 255;
};

Motor::Motor(int dir1Pin, int dir2Pin, int speedPin) {
    connect(dir1Pin, dir2Pin, speedPin);
}

Motor::~Motor() {}

void Motor::connect(int motorDir1Pin, int motorDir2Pin, int motorSpeedPin) {
  dir1Pin = motorDir1Pin;
  dir2Pin = motorDir2Pin;
  speedPin = motorSpeedPin;
  connected = true;
}

void Motor::rename(String newName) { motorName = newName; }

void Motor::drive(direction dir, int sp) {

  if (connected == false) {
    Serial.println(
        "Error: no motor connected.");
    return;
  }
  float speed;
  switch (dir) {
  case forward:
    speed = sp / 255.0 * maxSpeed;
    analogWrite(speedPin, speed);
    digitalWrite(dir1Pin, LOW);
    digitalWrite(dir2Pin, HIGH);
    if (debugging) {
      Serial.print(motorName);
      Serial.print(" rijdt vooruit aan een snelheid van ");
      Serial.print(speed / 2.55);
      Serial.println("%.");
    }
    break;
  case stop:
    analogWrite(speedPin, 0);
    digitalWrite(dir1Pin, LOW);
    digitalWrite(dir2Pin, LOW);
    if (debugging) {
      Serial.print(motorName);
      Serial.println(" staat stil.");
    }
    break;
  case backward:
    speed = sp / 255.0 * maxSpeed;
    analogWrite(speedPin, speed);
    digitalWrite(dir1Pin, HIGH);
    digitalWrite(dir2Pin, LOW);
    if (debugging) {
      Serial.print(motorName);
      Serial.print(" rijdt achteruit aan een snelheid van ");
      Serial.print(speed / 2.55);
      Serial.println("%.");
    }
    break;
  default:
    digitalWrite(dir1Pin, LOW);
    digitalWrite(dir2Pin, LOW);
    Serial.print("Motor Direction Error");
    break;
  }
}

void Motor::move(direction dir, int sp, unsigned long durMillis){
  unsigned long startTime = millis();
  while(millis() - startTime < durMillis){
    drive(dir, sp);
  }
  drive(stop,0);
}