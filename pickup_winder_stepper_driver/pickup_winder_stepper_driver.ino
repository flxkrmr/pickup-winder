#include <AccelStepper.h>

const int MOTOR_STEP_PIN = 6;
const int MOTOR_DIRECTION_PIN = 5;

const int SPEED_PIN_0 = 2;
const int SPEED_PIN_1 = 3;
const int SPEED_PIN_2 = 4;
const int SPEED_PIN_3 = 7;

const int ROUND_PIN = 8;

const int MAX_SPEED = 6500;

int speedPinReadCounter = 0;
const int SPEED_PIN_READ_COUNTER_MAX = 2000;


AccelStepper stepper(AccelStepper::DRIVER, MOTOR_STEP_PIN , MOTOR_DIRECTION_PIN);


void setup() {
  stepper.setMaxSpeed(MAX_SPEED);
  //stepper.setAcceleration(100);
  stepper.setSpeed(0);

  pinMode(SPEED_PIN_0, INPUT);
  pinMode(SPEED_PIN_1, INPUT);
  pinMode(SPEED_PIN_2, INPUT);
  pinMode(SPEED_PIN_3, INPUT);

  pinMode(ROUND_PIN, OUTPUT);
}

int speedPin0 = 0;
int speedPin1 = 0;
int speedPin2 = 0;
int speedPin3 = 0;
int speedVal = 0;

int speedFromPins(int in) {
  in &= 15;
  return (((double)in) / 16.0) * MAX_SPEED;
}

void loop() {
  if(speedPinReadCounter > SPEED_PIN_READ_COUNTER_MAX) {
    speedPinReadCounter = 0;

    speedPin0 = digitalRead(SPEED_PIN_0);
    speedPin1 = digitalRead(SPEED_PIN_1);
    speedPin2 = digitalRead(SPEED_PIN_2);
    speedPin3 = digitalRead(SPEED_PIN_3);

    speedVal = speedPin0 | 
          speedPin1 << 1 | 
          speedPin2 << 2 |
          speedPin3 << 3;
          
    stepper.setSpeed(speedFromPins(speedVal));

    digitalWrite(ROUND_PIN, (stepper.currentPosition() % 1600) > 800);
  }

  stepper.runSpeed();
  speedPinReadCounter++;
}
