#include <SoftwareSerial.h>

// LED Strip Pins
#define LED_RED 7
#define LED_GREEN 8
#define LED_BLUE 4

// Ultrasonic Pins
#define TRIG 12
#define ECHO 11

// Left/Right Motor Pins [FWD: Forward / BWD: Backward]
#define MOTOR_L_FWD 5
#define MOTOR_L_BWD 6
#define MOTOR_R_BWD 9
#define MOTOR_R_FWD 10

// Bluetooth Serial Communication
SoftwareSerial BTserial(2, 3); // RX | TX

// Global Variables
int SPEED = 50;
const float LR_BALANCE = 1.0f;
char input;
long duration;
long cm;

void setup() {
  BTserial.begin(9600);
  Serial.begin(9600);
  
  pinMode(MOTOR_L_FWD, OUTPUT);
  pinMode(MOTOR_L_BWD, OUTPUT);
  pinMode(MOTOR_R_BWD, OUTPUT);
  pinMode(MOTOR_R_FWD, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  // *** ULTRASONIC SENSOR TRIGGER AND ECHO RECEIVING *** //
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.println(cm);

  // Get Bluetooth Phone App Input if Available
  if(BTserial.available()) {
    input = BTserial.read();
  }

  // Carry Out Functions Based On User Input
  if(input == 'W') moveForward();
  else if(input == 'S' && cm > 20) moveBackward();
  else if(input == 'A') pivotLeft();
  else if(input == 'D') pivotRight();
  else if(input == 'J') slowSpeed();
  else if(input == 'K') midSpeed();
  else if(input == 'L') fastSpeed();
  else noMove();

  delay(100);
}

void moveForward() {
  analogWrite(MOTOR_L_FWD, SPEED);  // Set Left Motor Forward Speed to SPEED
  analogWrite(MOTOR_L_BWD, 0);      // Set Left Motor Backward Speed to 0
  
  analogWrite(MOTOR_R_FWD, SPEED);  // Set Right Motor Forward Speed to SPEED
  analogWrite(MOTOR_R_BWD, 0);      // Set Right Motor Backward Speed to 0

  digitalWrite(LED_RED, HIGH);      // Note: LED Logic is Reversed. [HIGH means OFF, LOW means ON]
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);
}

void moveBackward() {
  analogWrite(MOTOR_L_FWD, 0);
  analogWrite(MOTOR_L_BWD, SPEED);
  
  analogWrite(MOTOR_R_FWD, 0);
  analogWrite(MOTOR_R_BWD, SPEED);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);
}

void pivotLeft() {
  analogWrite(MOTOR_L_FWD, 0);
  analogWrite(MOTOR_L_BWD, SPEED);
  
  analogWrite(MOTOR_R_FWD, SPEED);
  analogWrite(MOTOR_R_BWD, 0);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
}

void pivotRight() {
  analogWrite(MOTOR_L_FWD, SPEED);
  analogWrite(MOTOR_L_BWD, 0);
  
  analogWrite(MOTOR_R_FWD, 0);
  analogWrite(MOTOR_R_BWD, SPEED);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
}

void noMove() {
  analogWrite(MOTOR_L_FWD, 0);
  analogWrite(MOTOR_L_BWD, 0);

  analogWrite(MOTOR_R_FWD, 0);
  analogWrite(MOTOR_R_BWD, 0);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

void slowSpeed() {
  SPEED = 50;
}

void midSpeed() {
  SPEED = 100;
}

void fastSpeed() {
  SPEED = 150;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
