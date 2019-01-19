#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int SPEED_PIN_0 = 53;
const int SPEED_PIN_1 = 51;
const int SPEED_PIN_2 = 49;
const int SPEED_PIN_3 = 52;

const int SPEED_POT_PIN = A0;
int speedPotPinVal = 0;

const int ROUND_PIN = 50;
int rounds = 0;
int roundReadCounter = 0;
const int ROUND_READ_COUNTER_MAX = 5000;
int roundReadLastVal = 0;
int roundRead = 0;

int updateSpeedCounter = 0;
const int UPDATE_SPEED_COUNTER_MAX = 10000;

const float RPS_MAX = (int) 6500.0/1600.0;
float rps = 0;

char rpsString[8];

LiquidCrystal_I2C lcd(0x27, 16, 4);

void setupDisplay() {
  lcd.begin();
  lcd.print("hello");
  lcd.clear();
  lcd.print("Speed:  0.00 rps");
  lcd.setCursor(0,1);
  lcd.print("Turns:  0");
}

void updateDisplaySpeed() {
  lcd.setCursor(7, 0);
  rps = ((float) speedPotPinVal) * RPS_MAX / 15.0;
  dtostrf(rps, 5, 2, rpsString);
  lcd.print(rpsString);
}

void updateDisplayRounds() {;
  lcd.setCursor(8,1);
  lcd.print(rounds);
}

void updateSpeed() {  
  speedPotPinVal = analogRead(SPEED_POT_PIN) >> 6;
  digitalWrite(SPEED_PIN_0, speedPotPinVal & 1);
  digitalWrite(SPEED_PIN_1, speedPotPinVal & 2);
  digitalWrite(SPEED_PIN_2, speedPotPinVal & 4);
  digitalWrite(SPEED_PIN_3, speedPotPinVal & 8);

  updateDisplaySpeed();
}

void updateRounds() {
  roundRead = digitalRead(ROUND_PIN);
  // if there is a change in the round pin value
  if(roundRead != roundReadLastVal) {
    // if it's the beginning of a round
    if(roundRead == 0) {
      rounds++;
      updateDisplayRounds();
    }
    roundReadLastVal = roundRead;      
  }    
}

void setup() {
  pinMode(SPEED_PIN_0, OUTPUT);
  pinMode(SPEED_PIN_1, OUTPUT);
  pinMode(SPEED_PIN_2, OUTPUT);
  pinMode(SPEED_PIN_3, OUTPUT);

  digitalWrite(SPEED_PIN_0, LOW);
  digitalWrite(SPEED_PIN_1, LOW);
  digitalWrite(SPEED_PIN_2, LOW);
  digitalWrite(SPEED_PIN_3, LOW);

  pinMode(ROUND_PIN, INPUT);

  setupDisplay();
  Serial.begin(9600);
}


void loop() {
  if(updateSpeedCounter > UPDATE_SPEED_COUNTER_MAX) {
    updateSpeed();
    updateSpeedCounter = 0;
  }

  if(roundReadCounter > ROUND_READ_COUNTER_MAX) {
    updateRounds();
    roundReadCounter = 0;
  }

  updateSpeedCounter++;
  roundReadCounter++;
}
