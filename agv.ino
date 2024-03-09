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
  motors.forward();
}

void loop() {
  /* Logikanya gimana?
  1. Defaultnya depan ga detek apa-apa, ultrasonik anglenya semua kedepan, kalau clear maju terus
  2. Kalau depan detek object 2M, motor melambat, servo gerak dan ultrasonik detek bagian samping
  3. Kalau 
  */
  do{
    for(i = 0; i < 3; i++)
    {
      delay(50);
      sonarValue[i] = sonar[i].ping_cm();
      Serial.print(sonarValue[i]);
      if(sonarValue[i] == 0) sonarValue[i] = 9999;
    }
  } while((sonarValue[0] > 200 && sonarValue[1] > 200 && sonarValue[2] > 200));

  if(sonarValue[1] <= 200) {
    if(sonarValue[1] <= 100)
    {
      motors.stop();
      for(i = 0; i < 5; i++)
      {
        j = i * 8;
        rightServo.write(120 + j);
        sonarValue[2] = sonar[2].ping_cm();
        if(k == 0) k = sonarValue[2];
        else if (k > sonarValue[2]) k = sonarValue[2];
        else ;
        delay(50);
        leftServo.write(60 - j);
        sonarValue[0] = sonar[0].ping_cm();
        if(l == 0) l = sonarValue[2];
        else if (l > sonarValue[2]) l = sonarValue[2];
        else ;
      }

      if()
      {

      }
      
      else if(sonarValue[0] <= 100)
      {

      }

      else if(sonarValue[2] <= 100)
      {
        
      }
    }
    motors.setSpeed(80);
    if(sonarValue[0] <= 200) {
      motors.setSpeedA(255);
      delay(50);
    }

    else {
      motors.setSpeedB(255);
      delay(50);
    }

    motors.setSpeed(80);
  }

  else if(sonarValue[0] <= 200) {
    motors.setSpeed(80);
  }

  else if(sonarValue[2] <= 200) {
    motors.setSpeed(80);
  }

  else motors.setSpeed(255);
  
}
