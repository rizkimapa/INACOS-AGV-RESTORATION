#include <NewPing.h>
#include <Servo.h>
#include <L298NX2.h>

// Ultrasonic Sensor Pins

const int trig_1 = 22;
const int echo_1 = 23;
const int trig_2 = 24;
const int echo_2 = 25;
const int trig_3 = 26;
const int echo_3 = 27;

NewPing leftPing = NewPing(trig_1, echo_1, 400);
NewPing middlePing = NewPing(trig_2, echo_2, 400);
NewPing rightPing = NewPing(trig_3, echo_3, 400);

// Servo Pins

const int servo_L = 8;
const int servo_R = 9;

Servo leftServo;
Servo rightServo;

// Motor Driver Pins

const int EN_A = 13;
const int EN_B = 12;
const int IN_1 = 28;
const int IN_2 = 29;
const int IN_3 = 30;
const int IN_4 = 31;

L298NX2 motors(EN_A, IN_1, IN_2, EN_B, IN_3, IN_4);

// Other variables that might be used

int i;

/* PROGRAM MAIN BODY */

void setup() {
  Serial.begin(9600);
  leftServo.attach(servo_L);
  rightServo.attach(servo_R);
  leftServo.write(90);
  rightServo.write(90);
}

void loop() {


}
