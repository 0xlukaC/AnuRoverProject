  #include <Servo.h>
  // https://canvas.anu.edu.au/courses/5696/assignments/24351
  // 123 / 30 (frames per second)
  const int turn360 = 3667; // ms to turn 360 degrees at full speed 4050  
  //  2(55/30) = 3.66666666667 = 3666.66666667 ms 
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


  const int TARGET = 9;   // ~9.5cm rounded
  const int WALL = 10;     // detection threshold


  Servo myServo;

  typedef enum TDir { LEFT, RIGHT } TDir; // turn direction

  void motorsStop() {
    digitalWrite(enablePin1, LOW);
    digitalWrite(enablePin2, LOW);
  }

  void motorsForward(int time = 0) {
    
    digitalWrite(motor1Pin1, LOW); digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW); digitalWrite(motor2Pin2, HIGH);
    digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
    if (time && time > 0) {
      delay(time);
      motorsStop(); 
    } 
  }

  // This is still blocking
  void motorsForwardDistance(int distance) {
    int time = (distance / cmPs) * 1000; // convert to ms
    motorsForward(time);
  
  }

  void motorsBack(int time) {
    digitalWrite(motor1Pin1, HIGH); digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH); digitalWrite(motor2Pin2, LOW);
    digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
    delay(time);
    motorsStop();
  }

  void motorsBackDistance(int distance) {
    int time = (distance / cmPs) * 1000; // convert to ms
    motorsBack(time);
  }

  
  // Chnaged Double to Int
  void turn(int theta, TDir dir ) {
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
    if (angle < 0 || angle > 230) {
      Serial.println("Invalid angle. Must be between 0 and 180.");
      return;
    }
    //angle -= abs(180); 
    angle = 180 - angle; // invert because 0 and 180 are swapped around
    myServo.attach(Green10);
    myServo.write(angle);
    if (delayTime && delayTime > 0) delay(delayTime);
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


  // second test


  void objectDetectionTest() {
    motorsForward();

    int angles[3] = {135 + 10, 90, 45 - 10}; // left, middle, right
    int i = 0;

    while (1) {
      setServoAngle(angles[i], 200);
      long distance = getDistanceCM();

      if (distance < 15) {
        motorsStop();
        motorsBack(500);
        return;
      }

      i = (i + 1) % 3; // cycle through 0,1,2
    }
  }


  void checkLeftRight(int &left, int &right) {
  setServoAngle(170, 200);
    right = getDistanceCM();

    setServoAngle(10, 200);
    left = getDistanceCM();

    // setServoAngle(90, 100); // reset forward
  }

  void turnTest() {
    int left, right;
    setServoAngle(90, 200);
    long ForwardDistance = getDistanceCM();
    if (ForwardDistance < 10) {
      motorsStop();
      motorsBack(300); // was 300
      checkLeftRight(left, right);
      // if (left > right)  turn(90, LEFT);
      // else turn(90, RIGHT);
      // int backupAngle = 45;
      
      // turn(backupAngle, (left > right) ? LEFT : RIGHT);
      
      // motorsBackDistance(4);
      TDir direction = (left > right) ? LEFT : RIGHT;
      motorsBack(500);
      turn(45, direction);
      motorsForward(300);
      turn(45, direction);
      

    }
    motorsForward(0);
  }

///////////////////////////////
/// START COPY FROM NVIM
//////////////////////////////const int USquareWidth = 19; // cm
const int USquareWidth = 19; // cm
const int SERVO_WAIT = 125;  // ms
const int sideLimit = 6;     // cm

void jankTurn(int theta, TDir direction, int time[], size_t length) {
  for (size_t i = 0; i < length; i++) {
    turn((int)round((double)theta / length), direction);
    motorsForward(time[i]);
  }
}

int recordAngle(int theta, int delay, int *dir) {
  setServoAngle(theta, delay);
  int dist = getDistanceCM();
  if (dir != NULL) *dir = dist;
  return dist;
}

void fixPosition(int left, int right, int front) {
  motorsStop(); // motors should already be stopped, but just in case

  int error = left - right;

  if (abs(error) > 2) {

    int theta = abs(error);

    if (theta > 25) theta = 25;

    int speeds[] = {100, 100, 100};
    TDir dir = (error > 0) ? LEFT : RIGHT;
    if (theta < 10)
      turn(theta, dir);
    else
      jankTurn(theta, dir, speeds, 2);
  }
}

void forwardEncounter(int front) {
  motorsStop();
  int left = recordAngle(10, SERVO_WAIT, NULL);
  int right = recordAngle(180, SERVO_WAIT, NULL);
  // if no walls either side pick left
  if (left > 18 && right > 18) {
    turn(90, LEFT);
    return;
  }

  int speeds[] = {300, 600, 100}; // 13.43cm takes ~2 seconds
  // one way is open
  motorsBackDistance(USquareWidth - front);
  if (left > right)
    jankTurn(90, LEFT, speeds, sizeof(speeds) / sizeof(speeds[0]));
  else
    jankTurn(90, RIGHT, speeds, sizeof(speeds) / sizeof(speeds[0]));
}

void navigateMaze() {
  int left, right;
  int leftDiag, rightDiag;

  int forwardDistance = recordAngle(90, SERVO_WAIT, NULL);

  // forward
  if (forwardDistance < 10) {
    forwardEncounter(forwardDistance);
  }

  recordAngle(10, SERVO_WAIT, &left);
  if (left <= sideLimit) {
    motorsStop();
    recordAngle(180, SERVO_WAIT, &right);
    fixPosition(left, right, forwardDistance);
  }

  // leftDiag
  recordAngle(60, SERVO_WAIT, &leftDiag);
  if (leftDiag < 5 && left > sideLimit + 1) {
    motorsStop();
    // recordAngle(10, SERVO_WAIT, &left);
    // recordAngle(180, SERVO_WAIT, &right);
    int checkForwards = recordAngle(90, SERVO_WAIT, NULL);
    if (checkForwards < 10) forwardEncounter(checkForwards);
    else forwardEncounter(leftDiag);
  }

  // forward
  forwardDistance = recordAngle(90, SERVO_WAIT, NULL);
  if (forwardDistance < 10) {
    forwardEncounter(forwardDistance);
  }

  recordAngle(180, SERVO_WAIT, &right);
  if (right <= sideLimit) {
    motorsStop();
    recordAngle(10, SERVO_WAIT, &left);
    fixPosition(left, right, forwardDistance);
  }

  // rightDiag
  recordAngle(120, SERVO_WAIT, &rightDiag);
  if (rightDiag < 5 && right > sideLimit + 1) {
    motorsStop();
    int checkForwards = recordAngle(90, SERVO_WAIT, NULL);
    if (checkForwards < 10) forwardEncounter(checkForwards);
    // recordAngle(170, SERVO_WAIT, &right);
    // recordAngle(10, SERVO_WAIT, &left);
    forwardEncounter(rightDiag);
  }

  motorsForward(0);
}

// what about when we're so angled to the side wall, the 90 scan picks it up as
// a wall
//    maybe we could use the diag then?
// or we just rotate like normal



////////////////////////
// END COPY FROM NVIM
////////////////////////////

  // SETUP
  void setup() {
    delay(2000);
    setServoAngle(90, 100);
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);  

    pinMode(enablePin1, OUTPUT);  // enablePin1
    pinMode(motor1Pin1, OUTPUT);  // motor1Pin1
    pinMode(motor1Pin2, OUTPUT);  // motor1Pin2

    pinMode(enablePin2, OUTPUT);  // enablePin2
    pinMode(motor2Pin1, OUTPUT);  // motor2Pin1
    pinMode(motor2Pin2, OUTPUT);  // motor2Pin2

    // objectDetectionTest();
    Serial.begin(9600); // note this will add computational lode, please comment out
    // turn(90, LEFT);
    // delay(700);
    // turn(90, RIGHT);
    // delay(700);
    // turn(45, LEFT);
    // delay(700);
    // turn(45, RIGHT);
    // turn(360, LEFT);
    // servoTest();
  }


  // LOOP
  void loop() {
  
    // turnTest();
    //  servoScanTest();
    navigateMaze();
    // servoTest();
  }

// 
// 
// 
// 
// 


  void testSonar(){
    // Serial.begin(9600);
    Serial.println(getDistanceCM());
    delay(100);
  }

 void servoTest() {
    setServoAngle(90, 500);   // pointing straight
    Serial.print("Angle 90 - Distance: ");
    Serial.println(getDistanceCM());
    Serial.flush(); // Wait for data to actually transmit

    setServoAngle(180, 500);  // pointing left
    Serial.print("Angle 180 - Distance: ");
    Serial.println(getDistanceCM());
    Serial.flush();

    setServoAngle(0, 500);    // pointing right
    Serial.print("Angle 0 - Distance: ");
    Serial.println(getDistanceCM());
    Serial.flush();
    
    setServoAngle(180, 500);  // back pointing left
    Serial.print("Angle 180 - Distance: ");
    Serial.println(getDistanceCM());
    Serial.flush();
}
