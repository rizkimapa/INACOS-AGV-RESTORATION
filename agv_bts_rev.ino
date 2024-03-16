#include <NewPing.h>
#include <Servo.h>
#include <BTS7960.h>

// Function Declaration
void forwardLoop();
void motorForward(int speed);
void motorBackward(int speed);
void motorStop();

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

const int EN1 = 28;
const int EN2 = 29;
const int L1_PWM = 12;
const int R1_PWM = 13;
const int L2_PWM = 14;
const int R2_PWM = 15;

BTS7960 motor1(EN1, L1_PWM, R1_PWM);
BTS7960 motor2(EN2, L2_PWM, R2_PWM);

// Other variables that might be used

int i, j;
int k, l = 0;
int sonarValue[3];
uint8_t speed = 255;

/* PROGRAM MAIN BODY */

void setup() {
  Serial.begin(115200);
  Serial.println("Inisiasi Pin");
  leftServo.attach(servo_L);
  rightServo.attach(servo_R);
  leftServo.write(60);
  rightServo.write(120);
}

void loop() {
    k = 0;
    l = 0;
    Serial.println("GO!");
    motorForward(speed);
  do{
    for(i = 0; i < 3; i++)
    {
      delay(50);
      sonarValue[i] = sonar[i].ping_cm();
      if(sonarValue[i] == 0) sonarValue[i] = 9999;
      Serial.print("Servo ke-");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(sonarValue[i]);
      Serial.println(" ");
      Serial.println("");
    }
  } while((sonarValue[0] > 200 && sonarValue[1] > 200 && sonarValue[2] > 200));

  if(sonarValue[1] <= 200)
  {
    Serial.println("Jarak sensor tengah ke objek: ");
    Serial.print(sonarValue[1]);
    Serial.println("");
    speed = 100;
    motorForward(speed);
    if(sonarValue[1] <= 100)
    {
      Serial.println("Objek dibawah 100cm !!!");
      forwardLoop();
      if (k < l)
      {
        Serial.println("Motor akan berbelok ke Kiri");
        motor1.Stop();
        motor2.TurnRight(80);
        delay(500);
        sonarValue[2] = sonar[2].ping_cm();
        if (sonarValue[2] == 0) sonarValue[2] = 9999;
        delay(50);
        while(sonarValue[2] != 9999)
        {
          sonarValue[2] = sonar[2].ping_cm();
          if (sonarValue[2] == 0) sonarValue[2] = 9999;
          Serial.print("Jarak objek dari sensor kanan: ");
          Serial.println(sonarValue[2]);
          delay(100);
        }
        motorForward(255);
        Serial.println("Motor kembali berjalan lurus");
      }

      else
      {
        Serial.println("Motor akan berbelok ke kanan");
        motor2.Stop();
        motor1.TurnLeft(80);
        delay(500);
        sonarValue[0] = sonar[0].ping_cm();
        if (sonarValue[0] == 0) sonarValue[0] = 9999;
        delay(50);
        while(sonarValue[0] != 9999)
        {
          sonarValue[0] = sonar[0].ping_cm();
          if (sonarValue[0] == 0) sonarValue[0] = 9999;
          Serial.print("Jarak objek dari sensor kiri: ");
          Serial.println(sonarValue[0]);
          delay(100);
        }
        motorForward(255);
        Serial.println("Motor kembali berjalan lurus");
      }
    }
  }

  else if(sonarValue[0] <= 200 && sonarValue[1] >= 200 && sonarValue[2] >= 200) {
    motor2.TurnRight(80);
    sonarValue[0] = sonar[0].ping_cm();
    if (sonarValue[0] == 0) sonarValue[0] = 9999;
    while(sonarValue[0] != 9999) delay(100);
    motorForward(255);
  }

  else if(sonarValue[2] <= 200 && sonarValue[1] >= 100 && sonarValue[0] >= 200) {
    motor1.TurnLeft(80);
    sonarValue[2] = sonar[2].ping_cm();
    if (sonarValue[2] == 0) sonarValue[2] = 9999;
    while(sonarValue[2] != 9999) delay(100);
    motorForward(255);
  }

  else motorForward(255);
  
}

void forwardLoop() {
    motorStop();
    for(i = 1; i <= 5; i++)
      {
        j = i * 8;
        rightServo.write(120 + j);
        sonarValue[2] = sonar[2].ping_cm();
        if (sonarValue[2] == 0) sonarValue[2] = 9999;
        if (k == 0) k = sonarValue[2];
        if (k > sonarValue[2]) k = sonarValue[2];
        Serial.print("Nilai terendah sensor kanan: ");
        Serial.println(sonarValue[2]);
        delay(50);
        leftServo.write(60 - j);
        sonarValue[0] = sonar[0].ping_cm();
        if (sonarValue[0] == 0) sonarValue[0] = 9999;
        if(l == 0) l = sonarValue[0];
        if (l > sonarValue[0]) l = sonarValue[0];
        Serial.print("Nilai terendah sensor kiri: ");
        Serial.println(sonarValue[0]);
      }
      rightServo.write(120);
      leftServo.write(60);

      if(k <= 100 && l <= 100) {
        motorStop();
      }
}

void motorForward(int speed)
{
  motor1.TurnLeft(speed);
  motor2.TurnRight(speed);
}

void motorBackward(int speed)
{
  motor2.TurnLeft(speed);
  motor1.TurnRight(speed);
}

void motorStop()
{
  motor1.Stop();
  motor2.Stop();
}
