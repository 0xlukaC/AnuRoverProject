#include <Servo.h>
// https://canvas.anu.edu.au/courses/5696/assignments/24351
// 123 / 30 (frames per second)
const int turn360 = 4050; // ms to turn 360 degrees at full speed
const int cmPs = 7; // 7 cm per second

const int enablePin1 = 11;  // Green
const int motor1Pin1 = 6;  // Orange
const int motor1Pin2 = 5;  // Yellow

const int motor2Pin2 = 3;   // Grey
const int motor2Pin1 = 4;   // Blue
const int enablePin2 = 9;  // Purple

const int triggerPin = 12;
const int echoPin = 13;

const int Green10 = 10; // servo
Servo myServo;

typedef enum TDir { LEFT, RIGHT } TDir; // turn direction

void motorsStop() {
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2, LOW);
}

void motorsForward(int time) {
  
  digitalWrite(motor1Pin1, LOW); digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW); digitalWrite(motor2Pin2, HIGH);
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  if (time && time > 0) {
    delay(time);
    motorsStop(); 
  } 
}

void motorsBack(int time) {
  digitalWrite(motor1Pin1, HIGH); digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH); digitalWrite(motor2Pin2, LOW);
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  delay(time);
  motorsStop();
}

 

void turn(double theta, TDir dir ) {
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  if (dir == RIGHT) {
    digitalWrite(motor1Pin1, LOW); digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH); digitalWrite(motor2Pin2, LOW);
  } else { // LEFT
    digitalWrite(motor1Pin1, HIGH); digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW); digitalWrite(motor2Pin2, HIGH);
  }
  
  delay(turn360 * (theta / 360.0));
  motorsStop();
}


void setServoAngle (int angle, int delayTime) {
  if (angle < 0 || angle > 180) {
    Serial.println("Invalid angle. Must be between 0 and 180.");
    return;
  }
  myServo.attach(Green10);
  myServo.write(angle);
  delay(delayTime);
}


long getDistanceCM() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  return duration * 0.034 / 2;
}


// void firstTest() {
//   delay(2000);
//   motorsForward();
//   delay(2000);
//   motorsStop();
//   delay(3000);
//   motorsBack();
//   delay(2000);
//   motorsStop();
// }

// second test
void objectDetection () {
  while (true) {
    long distance = getDistanceCM();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 20) { // if object is closer than 20 cm
      motorsStop();
      setServoAngle(90, 1000); // look left
      delay(1000);
      setServoAngle(0, 1000); // look right
      delay(1000);
      setServoAngle(45, 1000); // look forward
      delay(1000);
      motorsBack(1000); // back up for 1 second
    } else {
      motorsForward(500); // move forward for 0.5 seconds
    }
    delay(500); // wait before next distance check
  }
}





// SETUP
void setup() {
  // delay(2000);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);  

  pinMode(enablePin1, OUTPUT);  // enablePin1
  pinMode(motor1Pin1, OUTPUT);  // motor1Pin1
  pinMode(motor1Pin2, OUTPUT);  // motor1Pin2

  pinMode(enablePin2, OUTPUT);  // enablePin2
  pinMode(motor2Pin1, OUTPUT);  // motor2Pin1
  pinMode(motor2Pin2, OUTPUT);  // motor2Pin2

  // 7.3cm in 1 second
  // motorsForward(1000);
  // motorsStop();
  // delay(2000); // 2 second pause before starting
  // motorsForward();
  // delay(1000);
  // motorsStop();
 
 
  //firstTest();
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

  Serial.begin(9600);
  Serial.println(getDistanceCM());
  delay(100);
}

/* HOW TO NAVIGATE MAZE:
  variable, best Direction = the best angle the rover can go after it has scanned


*/




