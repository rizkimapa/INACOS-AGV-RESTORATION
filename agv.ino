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
  NewPing(trig_1, echo_1, 30), 
  NewPing(trig_2, echo_2, 30), 
  NewPing(trig_3, echo_3, 30)
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

int i, j;
int k, l = 0;
int sonarValue[3];

/* PROGRAM MAIN BODY */

void setup() {
  Serial.begin(115200);
  Serial.println("Inisiasi Pin");
  leftServo.attach(servo_L);
  rightServo.attach(servo_R);
  leftServo.write(60);
  rightServo.write(120);
  motors.setSpeed(255);
}

void loop() {
    k = 0;
    l = 0;
    Serial.println("GO!");
    motors.forward();
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
      Serial.println("Kecepatan motor saat ini: ");
      Serial.print(motors.getSpeedA());
      Serial.print(" ");
      Serial.print(motors.getSpeedB());
      Serial.println("");
    }
  } while((sonarValue[0] > 20 && sonarValue[1] > 20 && sonarValue[2] > 20));

  if(sonarValue[1] <= 20)
  {
    Serial.println("Jarak sensor tengah ke objek: ");
    Serial.print(sonarValue[1]);
    Serial.println("");
    motors.setSpeed(80);
    Serial.println("Kecepatan motor saat ini: ");
    Serial.print(motors.getSpeedA());
    Serial.print(" ");
    Serial.print(motors.getSpeedB());
    Serial.println("");
    if(sonarValue[1] <= 10)
    {
      Serial.println("Objek dibawah 100cm !!!");
      forwardLoop();
      if (k < l)
      {
        Serial.println("Motor akan berbelok ke Kiri");
        motors.forwardB();
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
        motors.forward();
        Serial.println("Motor kembali berjalan lurus");
        motors.setSpeed(255);
      }

      else
      {
        Serial.println("Motor akan berbelok ke kanan");
        motors.forwardA();
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
        motors.forward();
        Serial.println("Motor kembali berjalan lurus");
        motors.setSpeed(255);
      }
    }
  }

  else if(sonarValue[2] <= 10 && sonarValue[1] >= 10 && sonarValue[0] >= 10) {
    motors.setSpeedA(80);
    delay(50);
    Serial.println("Sensor kanan mendeteksi terlalu dekat!!!"); 
    sonarValue[2] = sonar[2].ping_cm();
    Serial.println(sonarValue[2]);
    if (sonarValue[2] == 0) sonarValue[2] = 9999;
    Serial.println(sonarValue[2]);
    while(sonarValue[2] <= 10)
    {
      delay(100);
      sonarValue[2] = sonar[2].ping_cm();
      if (sonarValue[2] == 0) sonarValue[2] = 9999;
      Serial.println("Kecepatan motor saat ini: ");
      Serial.print(motors.getSpeedA());
      Serial.print(" ");
      Serial.print(motors.getSpeedB());
      Serial.println("");
    }
    motors.setSpeed(255);
  }

  else if(sonarValue[0] <= 10 && sonarValue[1] >= 10 && sonarValue[2] >= 10) {
    motors.setSpeedB(80);
    delay(50);
    Serial.println("Sensor kiri mendeteksi terlalu dekat!!!"); 
    sonarValue[0] = sonar[0].ping_cm();
    if (sonarValue[0] == 0) sonarValue[0] = 9999;
    while(sonarValue[0] <= 10)
    {
      delay(100);
      sonarValue[0] = sonar[0].ping_cm();
      if (sonarValue[0] == 0) sonarValue[0] = 9999;
      Serial.println("Kecepatan motor saat ini: ");
      Serial.print(motors.getSpeedA());
      Serial.print(" ");
      Serial.print(motors.getSpeedB());
      Serial.println("");
    }

    motors.setSpeed(255);
  }

  else motors.setSpeed(255);
  
}

void forwardLoop() {
    int m, n;
    motors.stop();
    for(i = 1; i <= 5; i++)
      {
        j = i * 8;
        rightServo.write(120 + j);
        sonarValue[2] = sonar[2].ping_cm();
        if (sonarValue[2] == 0) sonarValue[2] = 9999;
        if (k == 0) k = sonarValue[2];
        if (k > sonarValue[2]) k = sonarValue[2];
        Serial.print("Nilai terendah sensor kanan: ");
        Serial.println(k);
        delay(50);
        leftServo.write(60 - j);
        sonarValue[0] = sonar[0].ping_cm();
        if (sonarValue[0] == 0) sonarValue[0] = 9999;
        if(l == 0) l = sonarValue[0];
        if (l > sonarValue[0]) l = sonarValue[0];
        Serial.print("Nilai terendah sensor kiri: ");
        Serial.println(l);
        delay(50);
      }
      rightServo.write(120);
      leftServo.write(60);
      delay(50);
      if(k <= 10 && l <= 10) {
        Serial.println("AGV akan mundur untuk mencari celah");
        motors.backward();
        delay(1000);
        k = 0;
        l = 0;
        forwardLoop();
      }
}
