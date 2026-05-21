#include "./rover.h"
#include <cmath>

// redefine TDir to contain Back and fowards

struct prevMoves {
  int time;
  TDir dir;

  struct prevMoves *next;
};

int *recordAngle(int theta, int delay, int *dir) {
  setServoAngle(theta, delay);
  int dist = getDistanceCM();
  if (dir != NULL)
    dir = &dist;
  return dir;
}

// distance from left wall, distance from right wall, distance from front wall,
// do not touch walls
void fixPosition(int left, int right, int front) {
  // given a square is 19cm^2 and our rover is 16cm long

  // NOTE: ASSUME MOTORS ARE STOPPED
  const int TARGET = 9; // centre of 19cm corridor ≈ 9.5cm
  const int SAFE_FRONT = 5;
  const int DEADZONE = 5;

  // case 1: we have a wall infront and we are too close to both front and
  // adjacent case 2: we are too close to adjacent
  //

  // case where were almost touching adjacent wall
  while (left <= 3 || right <= 3) {
    if (front < SAFE_FRONT) {
      turn(20, (left < right) ? LEFT : RIGHT);
      motorsBackDistance(3);
      turn(20, (left >= right) ? LEFT : RIGHT); // undo it
    } else {
      // opposite direction
      turn(50, (left < right) ? LEFT : RIGHT);
      motorsBackDistance(5);
      turn(50, (left >= right) ? LEFT : RIGHT); // undo it
    }
    left = *recordAngle(0, 300, NULL);
    right = *recordAngle(180, 300, NULL);
  }

  int error = left - right;

  // already centred → go straight
  if (abs(error) <= DEADZONE)
    return;

  if (error > 0)
    turn(3, LEFT); // more space on left → drift left
  else
    turn(3, RIGHT); // more space on right → drift right
}

void forwardEncounter(int distance) {
  int left = *recordAngle(10, 200, NULL);
  int right = *recordAngle(170, 200, NULL);
  // bools
  int collisionTRight = right > 16;
  int colliisionTLeft = left > 16; // the rover is 16.5 cm long
  int gLeft = left <= 15;
  int gRight = right <= 15;

  motorsStop();

  // WARNING:
  fixPosition(left, right, distance);

  if (gLeft && gRight) { // you are boxed in; the only way to go is backwards
    int newDistance = 7 + (19 - distance);
    motorsBackDistance(newDistance); // go 7 back into the previous squre

    // maybe just call this function again
  }
  // we need to check whether rotating would hit a wall or not

  // both ways are open This could also mean you back is to the wall
  if (!gLeft && !gRight) {
    // motorsBack(300); // choose left |
    turn(90, LEFT);
  }

  // one way is open
  motorsBack(300);
  if (left > right)
    turn(90, LEFT);
  else
    turn(90, RIGHT);

  // if (lef)
}

const int sideLimit = 3;
const int diagLimit = 5;

void navigateMaze() {
  int left, right;
  int leftDiag, rightDiag;

  int forwardDistance = *recordAngle(
      90, 150, NULL); // is deconstructing the pointer the right thing?
  if (forwardDistance < 10) {
    forwardEncounter(forwardDistance);
  }

  recordAngle(45, 10, &leftDiag);
  if (leftDiag <=
      diagLimit) { // pretty much just treat this as a regular wall in front
    forwardEncounter(leftDiag);
  }

  recordAngle(10, 50, &left);
  if (left <= sideLimit) { // stop, check we're not boxed in/facing a 45 wall
    fixPosition(left, right, 40);
  }

  forwardDistance = *recordAngle(90, 150, NULL);
  if (forwardDistance < 10) {
    forwardEncounter(forwardDistance);
  }

  recordAngle(135, 10, &rightDiag);
  if (rightDiag < diagLimit) {
    forwardEncounter(rightDiag);
  }

  recordAngle(170, 50, &right);
  if (right < sideLimit) {
    fixPosition(left, right, 40);
  }

  fixPosition(left, right, 40); // 40 > square length
  // recordAngle(135, 10, &rightDiag);
  // if (rightDiag < diagLimit) {}

  motorsForward(0);
}

// So you're in a straight line,
//  check left and right, adjust accordingly but keep checking fowards,
//
//  you encounter a wall either perpendicular or 45 degress
//    stop. look left and right (fully)
//      if one direction is open go that way
//      if both directions are open, choose left
//      if both left and right are closed off, go backwards one move
//        assume straight ahead is a wall. choose either left or right.
//        if once again, that's not valid, repeat.

int main() { return 0; }
