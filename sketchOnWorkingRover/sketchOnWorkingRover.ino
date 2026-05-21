  #include <Servo.h>
  // https://canvas.anu.edu.au/courses/5696/assignments/24351
  // 123 / 30 (frames per second)
  const int turn360 = 4100; // ms to turn 360 degrees at full speed 4050
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
    if (angle < 0 || angle > 180) {
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
      setServoAngle(angles[i], 300);
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



  // SETUP
  void setup() {
    delay(2000);
    // setServoAngle(90, 100);
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
    turn(360, LEFT);
  }


  // LOOP
  void loop() {
  
    // turnTest();
    //  servoScanTest();
  }

// 
// 
// 
// 
// 


  void testSonar(){
    Serial.begin(9600);
    Serial.println(getDistanceCM());
    delay(100);
  }

  void servoTest() {
    setServoAngle(90, 500);   // pointing straight
    setServoAngle(180, 500);  // pointing left
    setServoAngle(0 , 500); // pointing right
    setServoAngle(180, 500); // back pointing left
  }
