/** @file gen6_demo_sketch
 *  @brief A simple demo for controlling a stepper motor with a testio interface
 *  
 *  Created 20210324
 *  Revised -
 *  
 *  @author E. Jongerius, J.M. Brandsma (C)EJE Electronics BV
 *  
 */

#include <AccelStepper.h>

//Pin definitions when using Sanguino pinout
#define Y_nEN  (24)      //Or A0 for analog (a7)
#define Y_OPTO  (25)    //Or A1 for analog (a6)
#define THERM (26)      //Or A2 for analog (a5)
#define Z_STEP  (27)    //Or A3 for analog (a4)
#define Z_DIR  (28)     //Or A4 for analog (a3)
#define Z_nEN  (29)      //Or A5 for analog (a2)
#define Z_OPTO  (30)    //Or A6 for analog (a1)
#define E_STEP  (4)     //b4
#define E_nEN  (3)       //b3
#define E_DIR  (2)      //b2
#define Y_STEP  (23)    //c7
#define Y_DIR (22)      //c6
#define X_OPTO  (20)    //c4
#define X_nEN  (19)      //c3
#define X_DIR  (18)     //c2
#define SDA  (17)       //c1
#define SCL  (16)       //c0
#define X_STEP  (15)    //d7
#define HEAT  (14)      //d6

AccelStepper zStepper(AccelStepper::DRIVER, Z_STEP, Z_DIR);
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(HEAT, OUTPUT);
  pinMode(Z_OPTO, INPUT);
  pinMode(Z_nEN, OUTPUT);
  pinMode(Z_STEP, OUTPUT);
  digitalWrite(Z_nEN, 1);

  zStepper.setAcceleration(50000);
  zStepper.setMaxSpeed(5000);
}

// key=value => value
// Assumes that line does not contain line endings
String testioGetValue(String line) {
  return line.substring(line.indexOf('=') + 1);
}

int testioGetInt(String line) {
  return testioGetValue(line).toInt();
}

long testioGetLong(String line) {
  return atol(testioGetValue(line).c_str());
}

float testioGetFloat(String line) {
  return testioGetValue(line).toFloat();
}

void handleCommand(String command) {
  if (command.startsWith("z_target=")) {
    zStepper.moveTo(testioGetLong(command));
    Serial.print("z_target=");
    Serial.println(testioGetInt(command));
    
  } else if (command.startsWith("z_target?")) {
    Serial.print("z_target=");
    Serial.println(zStepper.targetPosition());
    
  } else if (command.startsWith("z_pos=")) {
    long pos = testioGetLong(command);
    zStepper.setCurrentPosition(pos);
    Serial.print("z_pos=");
    Serial.println(pos);
    
  } else if (command == "z_pos?") {
    Serial.print("z_pos=");
    Serial.println(zStepper.currentPosition());
    
  } else if (command.startsWith("z_accel=")) {
    float accel = testioGetFloat(command);
    zStepper.setAcceleration(accel);
    Serial.print("z_accel=");
    Serial.println(accel);

  } else if (command.startsWith("z_max_speed=")) {
    float speed = testioGetFloat(command);
    zStepper.setMaxSpeed(speed);
    Serial.print("z_max_speed=");
    Serial.println(speed);

  } else if (command == "z_max_speed?") {
    Serial.print("z_max_speed=");
    Serial.println(zStepper.maxSpeed());
    
  } else if (command.startsWith("z_enable=")) {
    if (testioGetInt(command)) {
      digitalWrite(Z_nEN, 0);
      Serial.println("z_enable=1");
    } else {
      digitalWrite(Z_nEN, 1);
      Serial.println("z_enable=0");
    }
    
  } else if (command == "z_enable?") {
    Serial.print("z_enable=");
    Serial.println(!digitalRead(Z_nEN));
    
  } else {
    Serial.print("Unknown command: ");
    Serial.println(command);
    
  }
}

String command;
void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c != '\r' && c != '\n') {
      command += c;
    } else {
      handleCommand(command);
      command = "";
    }
  }

  zStepper.run();
  
  digitalWrite(HEAT, digitalRead(Z_OPTO));
}
