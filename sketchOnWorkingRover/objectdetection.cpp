void objectDetectionTest() {
  while (1) {
    setServoAngle(90, 300);
    long distanceFront = getDistanceCM();

    if (distanceFront < 10) {

      motorsStop();
      motorsBack(300);

      setServoAngle(0, 300);
      long distanceRight = getDistanceCM();

      setServoAngle(180, 300);
      long distanceLeft = getDistanceCM();

      if (distanceRight > distanceLeft) {
        turn(90, RIGHT);
        return;
      } else {
        turn(90, LEFT);
        return;
      }

    } else {
      motorsFowardDistance(5);
    }
    objectDetection();
  }
}


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
