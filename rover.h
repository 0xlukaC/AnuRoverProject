#ifndef ROVER_H
#define ROVER_H

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// =======================
// ARDUINO CORE SHIMS
// =======================

#ifndef HIGH
#define HIGH 0x1
#define LOW 0x0
#endif

#ifndef OUTPUT
#define OUTPUT 0x1
#define INPUT 0x0
#endif

// fake Arduino functions (so IDE stops complaining)
void pinMode(int, int);
void digitalWrite(int, int);
int digitalRead(int);

void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

void Serial_begin(long baud);
void Serial_println(const char *s);
void Serial_println(long v);
void Serial_print(const char *s);
void Serial_print(long v);

// map Arduino Serial → simple namespace stub
struct Serial_t {
  void begin(long b) { Serial_begin(b); }
  void println(const char *s) { Serial_println(s); }
  void println(long v) { Serial_println(v); }
  void print(const char *s) { Serial_print(s); }
  void print(long v) { Serial_print(v); }
};

extern Serial_t Serial;

// =======================
// SERVO STUB
// =======================

class Servo {
public:
  void attach(int pin);
  void write(int angle);
};

// =======================
// YOUR PINS
// =======================

extern const int enablePin1;
extern const int motor1Pin1;
extern const int motor1Pin2;

extern const int enablePin2;
extern const int motor2Pin1;
extern const int motor2Pin2;

extern const int triggerPin;
extern const int echoPin;

extern const int Green10;

// =======================
// CONSTANTS
// =======================

extern const int turn360;
extern const int cmPs;

extern const int TARGET;
extern const int WALL;

// =======================
// TYPES
// =======================

typedef enum TDir { LEFT, RIGHT } TDir;

// =======================
// HARDWARE OBJECTS
// =======================

extern Servo myServo;

// =======================
// YOUR FUNCTIONS
// =======================

void motorsStop();
void motorsForward(int time = 0);
void motorsForwardDistance(int distance);
void motorsBack(int time);

void turn(double theta, TDir dir);

void setServoAngle(int angle, int delayTime);
long getDistanceCM();

void objectDetectionTest();
void centreDrive();
void scan(int &left, int &front, int &right);
void turnTest();
void checkLeftRight(int &left, int &right);

#endif
