#include "./rover.h"

// redefine TDir to contain Back and fowards

struct prevMoves {
  int time;
  TDir dir;

  struct prevMoves *next;
};

int *recordAngle(int theta, int delay, int *dir) {
  setServoAngle(theta, delay);
  int dist = getDistanceCM();
  if (dir != NULL) dir = &dist;
  return dir;
}

// distance from left wall, distance from right wall, distance from front wall,
// do not touch walls
void fixPosition(int left, int right, int front) {
  // given a square is 19cm^2 and our rover is 16cm long

  const int TARGET = 9; // centre of 19cm corridor ≈ 9.5cm
  const int SAFE_FRONT = 11;
  const int DEADZONE = 2;

  // --- FRONT WALL HANDLING ---
  if (front < SAFE_FRONT) {
    motorsStop();
    motorsBack(300);

    // choose direction with more space
    if (left > right) {
      turn(90, LEFT);
    } else {
      turn(90, RIGHT);
    }
    return;
  }

  // --- LEFT/RIGHT CENTRING ERROR ---
  int error = (left - right);

  // already centred → go straight
  if (abs(error) <= DEADZONE) {
    motorsForward();
    return;
  }

  // --- CORRECTION ---
  if (error > 0) {
    // more space on left → drift left
    turn(3, LEFT);
  } else {
    // more space on right → drift right
    turn(3, RIGHT);
  }

  motorsForward();
}

void forwardEncounter(int distance) {
  int left = *recordAngle(10, 200, NULL);
  int right = *recordAngle(170, 200, NULL);
  int collisionTRight = right > 16;
  int colliisionTLeft = left > 16; // bool  the rover is 16.5 cm long
  int gLeft = left <= 10;
  int gRight = right <= 10;

  if (gLeft && gRight) { // you are boxed in; the only way to go is backwards
    distance = 7 + (19 - distance) motorsBackDistance(
                       distance); // go 7 back into the previous squre

    // maybe just call this function again
  }
  // we need to check whether rotating would hit a wall or not

  if (!gLeft && !gRight) { // both ways are open
  }

  // one way is open
  // if (left > 10

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
  if (leftDiag <= diagLimit) {
  }

  recordAngle(10, 50, &left);
  if (left <= sideLimit) { // stop, check we're not boxed in/facing a 45 wall
  }

  // recordAngle(45, 10, &leftDiag);
  // if (leftDiag <= 5) {}

  forwardDistance = *recordAngle(90, 150, NULL);
  if (forwardDistance < 10) {
  }

  recordAngle(135, 10, &rightDiag);
  if (rightDiag < diagLimit) {
  }

  recordAngle(170, 50, &right);
  if (right < sideLimit) {
  }

  if (left - right > 15) {
  }

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
