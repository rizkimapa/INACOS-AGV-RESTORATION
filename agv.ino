#include <NewPing.h>
#include <Servo.h>
#include <L298NX2.h>

// Function Declaration
void forwardLoop();

// Ultrasonic Sensor Pins

const int trig_1 = 22;
const int echo_1 = 23;
const int trig_2 = 24;
const int echo_2 = 25;
const int trig_3 = 26;
const int echo_3 = 27;

NewPing sonar[3] = {
  NewPing(trig_1, echo_1, 300), 
  NewPing(trig_2, echo_2, 300), 
  NewPing(trig_3, echo_3, 300)
};

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

uint8_t i, j;
uint8_t k, l = 0;
uint8_t sonarValue[3];

/* PROGRAM MAIN BODY */

void setup() {
  Serial.begin(115200);
  leftServo.attach(servo_L);
  rightServo.attach(servo_R);
  leftServo.write(60);
  rightServo.write(120);
  motors.setSpeed(255);
}

void loop() {
    k = 0;
    l = 0;
    motors.forward();
  do{
    for(i = 0; i < 3; i++)
    {
      delay(50);
      sonarValue[i] = sonar[i].ping_cm();
      Serial.print(sonarValue[i]);
      if(sonarValue[i] == 0) sonarValue[i] = 9999;
    }
  } while((sonarValue[0] > 200 && sonarValue[1] > 200 && sonarValue[2] > 200));

  if(sonarValue[1] <= 200)
  {
    motors.setSpeed(80);
    if(sonarValue[1] <= 100)
    {
      forwardLoop();
      if (k <= 100)
      {
        motors.forwardB();
        delay(500);
        while(sonar(2).ping_cm() != 9999) delay(100);
        motors.forward();
        motors.setSpeed(255);
      }

      else
      {
        motors.forwardA();
        delay(500);
        while(sonar(0).ping_cm() != 9999) delay(100);
        motors.forward();
        motors.setSpeed(255);
      }
    }
  }

  else if(sonarValue[0] <= 200 && sonarValue[1] >= 200 && sonarValue[2] >= 200) {
    motors.setSpeedB(80);
    while(sonar[0].ping_cm() != 9999) delay(100);
    motors.setSpeed(255);
  }

  else if(sonarValue[2] <= 200 && sonarValue[1] >= 100 && sonarValue[0] >= 200) {
    motors.setSpeedA(80);
    while(sonar[2].ping_cm() != 9999) delay(100);
    motors.setSpeed(255);
  }

  else motors.setSpeed(255);
  
}

void forwardLoop() {
    motors.stop();
    for(i = 0; i < 5; i++)
      {
        j = i * 8;
        rightServo.write(120 + j);
        sonarValue[2] = sonar[2].ping_cm();
        if(k == 0) k = sonarValue[2];
        if (k > sonarValue[2]) k = sonarValue[2];
        delay(50);
        leftServo.write(60 - j);
        sonarValue[0] = sonar[0].ping_cm();
        if(l == 0) l = sonarValue[0];
        if (l > sonarValue[0]) l = sonarValue[0];
      }
      rightServo.write(120);
      leftServo.write(60);

      if(k <= 100 && l <= 100) {
        motors.backward();
        delay(1000);
        forwardLoop();
      }
}
