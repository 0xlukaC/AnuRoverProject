#include <Servo.h>
const int enablePin1 = 11;  // Green
const int motor1Pin1 = 6;  // Orange
const int motor1Pin2 = 5;  // Yellow

const int motor2Pin2 = 3;   // Grey
const int motor2Pin1 = 4;   // Blue
const int enablePin2 = 9;  // Purple

const int Green10 = 10; // servo

Servo myServo;

// -------- MOTOR FUNCTIONS --------
void motorsForward() {
  // flipped so it's actually forward
  digitalWrite(motor1Pin1, LOW); digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW); digitalWrite(motor2Pin2, HIGH);
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
}

void motorsBack() {
  digitalWrite(motor1Pin1, HIGH); digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH); digitalWrite(motor2Pin2, LOW);
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
}

void turnRight() {
  digitalWrite(motor1Pin1, LOW); digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH); digitalWrite(motor2Pin2, LOW);
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
}

void motorsStop() {
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2, LOW);
}

void servoTest() {
  myServo.attach(10);
  myServo.write(0);
  delay(3000);
  myServo.write(90);
  delay(3000);
  myServo.write(180);
  delay(3000);
}

void firstTest() {
  delay(2000);
  motorsForward();
  delay(2000);
  motorsStop();
  delay(3000);
  motorsBack();
  delay(2000);
  motorsStop();
}

// SETUP
void setup() {
  pinMode(enablePin1, OUTPUT);  // enablePin1
  pinMode(motor1Pin1, OUTPUT);  // motor1Pin1
  pinMode(motor1Pin2, OUTPUT);  // motor1Pin2

  pinMode(enablePin2, OUTPUT);  // enablePin2
  pinMode(motor2Pin1, OUTPUT);  // motor2Pin1
  pinMode(motor2Pin2, OUTPUT);  // motor2Pin2

  // 7.3cm in 1 second

  // motorsStop();
  // delay(2000); // 2 second pause before starting
  // motorsForward();
  // delay(1000);
  // motorsStop();

  firstTest();
}




// LOOP
void loop() {
  // delay(3000);
  // // Drive rectangle (4 sides, 4 turns)
  // for (int i = 0; i < 4; i++) {
  //   motorsForward();
  //   delay(STRAIGHT_TIME);
  //   motorsStop();
  //   delay(300); // brief pause before turning

  //   turnRight();
  //   delay(TURN_TIME);
  //   motorsStop();
  //   delay(300); // brief pause before next straight
  // }

  // // Stop after one rectangle
  // while (true) {
  //   motorsStop();
  // }
}






