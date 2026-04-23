// PIN DECLARATIONS

// Motor 1
const int enablePin1 = 9;
const int motor1Pin1 = 4;
const int motor1Pin2 = 3;

// Motor 2
const int enablePin2 = 10;
const int motor2Pin1 = 6;
const int motor2Pin2 = 5;

// TIMING CONSTANTS (tune these)
const int STRAIGHT_TIME = 5310;
const int TURN_TIME = 2905;

// -------- MOTOR FUNCTIONS --------
void motorsForward() {
  // flipped so it's actually forward
  digitalWrite(motor1Pin1, LOW); digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW); digitalWrite(motor2Pin2, HIGH);
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

// SETUP
void setup() {
  pinMode(enablePin1, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  pinMode(enablePin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  motorsStop();
  delay(2000); // 2 second pause before starting
}

// LOOP
void loop() {
  delay(3000);
  // Drive rectangle (4 sides, 4 turns)
  for (int i = 0; i < 4; i++) {
    motorsForward();
    delay(STRAIGHT_TIME);
    motorsStop();
    delay(300); // brief pause before turning

    turnRight();
    delay(TURN_TIME);
    motorsStop();
    delay(300); // brief pause before next straight
  }

  // Stop after one rectangle
  while (true) {
    motorsStop();
  }
}