#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor Left_Front = motor(PORT17, ratio18_1, true);

motor Right_Front = motor(PORT4, ratio18_1, false);

motor Left_Back = motor(PORT10, ratio18_1, true);

motor Right_Back = motor(PORT2, ratio18_1, false);

// AI Vision Color Descriptions
// AI Vision Code Descriptions
vex::aivision AIVision11(PORT11, aivision::ALL_TAGS);

inertial Inertial1 = inertial(PORT1);

motor Claw = motor(PORT3, ratio18_1, false);

motor arm = motor(PORT5, ratio18_1, false);



// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}



void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}

#pragma endregion VEXcode Generated Robot Configuration
#pragma region VEXcode Generated Robot Configuration
#include "vex.h"

using namespace vex;

// Le Controller
controller Controller1 = controller(primary);

#pragma endregion

// Alignment button
void align() {

  const int CENTER_X = 160;
  const int TARGET_WIDTH = 120;

  while (Controller1.ButtonX.pressing()) {

    AIVision11.takeSnapshot(aivision::ALL_TAGS);

    if (AIVision11.objectCount > 0) {

      aivision::object obj = AIVision11.largestObject;

      // Controller display 
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("Tag: %d", obj.id);

      // Lowkey Fire Alignment 
      int error = obj.centerX - CENTER_X;
      double turnSpeed = error * 0.4;

      // el turn speed
      if (turnSpeed > 50) turnSpeed = 50;
      if (turnSpeed < -50) turnSpeed = -50;

      // Drive toward tag
      if (obj.width < TARGET_WIDTH) {

        double leftSpeed = 30 + turnSpeed;
        double rightSpeed = 30 - turnSpeed;

        Left_Front.spin(forward, leftSpeed, percent);
        Left_Back.spin(forward, leftSpeed, percent);
        Right_Front.spin(forward, rightSpeed, percent);
        Right_Back.spin(forward, rightSpeed, percent);

      } else {
        Left_Front.stop();
        Left_Back.stop();
        Right_Front.stop();
        Right_Back.stop();
        // hehe line 67 tehehe
      }
      
    } else {
      // No tag found :(
      Left_Front.stop();
      Left_Back.stop();
      Right_Front.stop();
      Right_Back.stop();

      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("No Tag");
    }

    wait(20, msec);
  }

  // Stop when released
  Left_Front.stop();
  Left_Back.stop();
  Right_Front.stop();
  Right_Back.stop();
}

// PRE-AUTO 
void pre_auton(void) {
  Brain.Screen.print("Pre-auto");
  Inertial1.calibrate();
  wait(1, seconds);
}
/* TO DRIVE A DISTANCE
Left_Front.spinFor();
Left_Back.spinFor();
Right_Front.spinFor();
Right_Back.spinFor(); */

// AUTONOMOUS
void autonomous(void) {
  Brain.Screen.print("Autonomous");
  Inertial1.calibrate();
  // Initialise velocity  
  Left_Front.setVelocity(50, percent);
  Right_Front.setVelocity(50, percent);
  Left_Back.setVelocity(50, percent);
  Right_Back.setVelocity(50, percent);
  // Set stopping for the mechanisms to hold
  Claw.setStopping(hold);
  arm.setStopping(hold);
  // Pick up preload and lift arm
  Claw.spin(forward);
  arm.spin(forward);
  // Drive away from the starting point
  Left_Front.spinFor(forward, 25.3, turns);
  Right_Front.spinFor(forward, 25.3, turns);
  Right_Back.spinFor(forward, 25.3, turns);
  Left_Back.spinFor(forward, 25.3, turns);
  // Turn Left
  Left_Front.spinFor(forward, 1, turns);
  Left_Back.spinFor(forward, 1, turns);
  Right_Front.spinFor(reverse, 1, turns);
  Right_Back.spinFor(reverse, 1, turns);
  // Drive to goal
  Left_Front.spinFor(forward, 25.3, turns, false); // dont wait
  Left_Back.spinFor(forward, 25.3, turns, false);
  Right_Front.spinFor(forward, 25.3, turns, false);
  Right_Back.spinFor(forward, 25.3, turns, false);
  // Place pin
  Claw.spin(reverse);
  arm.spin(reverse);
  // Reverse from goal
  Left_Front.spinFor(reverse, 25.3, turns);
  Right_Front.spinFor(reverse, 25.3, turns);
  Left_Back.spinFor(reverse, 25.3, turns);
  Right_Back.spinFor(reverse, 25.3, turns);
  // Turn left to be facing the wall
  Left_Front.spinFor(forward, 1, turns);
  Left_Back.spinFor(forward, 1, turns);
  Right_Front.spinFor(reverse, 1, turns);
  Right_Back.spinFor(reverse, 1, turns);
  // Drive to colour switcher thingy
  Left_Front.spinFor(forward, 25.3, turns, false);
  Right_Front.spinFor(forward, 25.3, turns, false);
  Right_Back.spinFor(forward, 25.3, turns, false);
  Left_Back.spinFor(forward, 25.3, turns, false);
}

// DRIVER CONTROL
void usercontrol(void) {

  double maxSpeed = 80;

  while (true) {

    // HOLD X FOR AUTO ALIGN
    if (Controller1.ButtonX.pressing()) {
      align(); // Called the align function
      continue;
    }

    // Speed adjust
    if (Controller1.ButtonY.pressing()) {
      maxSpeed += 5;
      if (maxSpeed > 100) maxSpeed = 100;
      wait(200, msec);
    }

    if (Controller1.ButtonA.pressing()) {
      maxSpeed -= 5;
      if (maxSpeed < 20) maxSpeed = 20;
      wait(200, msec);
    }

    if (Controller1.ButtonL1.pressing()) {
      arm.spin(forward);
    }

     if (Controller1.ButtonL2.pressing()) {
      arm.spin(reverse);
    }

      if (Controller1.ButtonR1.pressing()) {
      Claw.spin(forward);
      Brain.Screen.print("spin");

    }

      if (Controller1.ButtonR2.pressing()) {
      Claw.spin(reverse);
        Brain.Screen.print("spin");
    }

    // Tank drive
    double leftSpeed = Controller1.Axis3.position();
    double rightSpeed = Controller1.Axis2.position();

    // Deadzone 
    if (abs(leftSpeed) < 5) leftSpeed = 0;
    if (abs(rightSpeed) < 5) rightSpeed = 0;

    leftSpeed *= (maxSpeed / 100.0);
    rightSpeed *= (maxSpeed / 100.0);

    // Left motors
    if (leftSpeed == 0) {
      Left_Front.stop();
      Left_Back.stop();
    } else {
      Left_Front.spin(forward, leftSpeed, percent);
      Left_Back.spin(forward, leftSpeed, percent);
    }

    // Right motors
    if (rightSpeed == 0) {
      Right_Front.stop();
      Right_Back.stop();
    } else {
      Right_Front.spin(forward, rightSpeed, percent);
      Right_Back.spin(forward, rightSpeed, percent);
    }

    wait(20, msec);
  }
}

int main() {
  vexcodeInit();

  competition Competition;
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
