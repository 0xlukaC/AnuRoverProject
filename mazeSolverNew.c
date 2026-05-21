#include "./rover.h"
#include <cmath>

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
  int speeds1[] = {70, 70};
  // if no walls either side pick left
  if (left > 18 && right > 18) {
    jankTurn(90, LEFT, speeds1, sizeof(speeds1) / sizeof(speeds1[0]));
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
    recordAngle(170, SERVO_WAIT, &right);
    fixPosition(left, right, forwardDistance);
  }

  // leftDiag
  recordAngle(60, SERVO_WAIT, &leftDiag);
  if (leftDiag < 5 && left > sideLimit + 1) {
    motorsStop();
    // recordAngle(10, SERVO_WAIT, &left);
    // recordAngle(170, SERVO_WAIT, &right);
    int checkForwards = recordAngle(90, SERVO_WAIT, NULL);
    if (checkForwards < 10)
      forwardEncounter(checkForwards);
    else
      forwardEncounter(leftDiag);
  }

  // forward
  forwardDistance = recordAngle(90, SERVO_WAIT, NULL);
  if (forwardDistance < 10) {
    forwardEncounter(forwardDistance);
  }

  recordAngle(170, SERVO_WAIT, &right);
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
