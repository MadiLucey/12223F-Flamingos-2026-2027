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
controller Controller1 = controller(primary);
motor Left_Front = motor(PORT17, ratio18_1, false);

motor Right_Front = motor(PORT4, ratio18_1, true);

motor Left_Back = motor(PORT10, ratio18_1, false);

motor Right_Back = motor(PORT2, ratio18_1, true);

inertial Inertial1 = inertial(PORT1);



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



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

// ----------------------------------------------------------------------------
//                                                                            
//    Project: Override match code                                            
//    Author: 12223F Flamingos
//    Created: 14/04/26
//    Configuration:        
//                                                                            
// ----------------------------------------------------------------------------

// Include the V5 Library
#include "vex.h"

// Allows for easier use of the VEX Library
using namespace vex;

// Begin project code

void preAutonomous(void) {
  // actions to do when the program starts
  Brain.Screen.clearScreen();
  Brain.Screen.print("pre-auto");
  Inertial1.calibrate();
  wait(1, seconds);
}

void autonomous(void) {
  Brain.Screen.clearScreen();
  Brain.Screen.print("auto running");
  // place automonous code here
}

void userControl(void) {
  Brain.Screen.clearScreen();

  double maxSpeed = 80; // percent (starts at 80%)

  while (true) {

    // --- BUTTON CONTROLS ---
    if (Controller1.ButtonY.pressing()) {
      maxSpeed += 5;
      if (maxSpeed > 100) maxSpeed = 100;
      wait(200, msec); // debounce delay
    }

    if (Controller1.ButtonA.pressing()) {
      maxSpeed -= 5;
      if (maxSpeed < 20) maxSpeed = 20;
      wait(200, msec); // debounce delay
    }

    // --- JOYSTICK INPUT ---
    double leftSpeed = Controller1.Axis3.position();
    double rightSpeed = Controller1.Axis2.position();

    // deadzone
    if (abs(leftSpeed) < 5) leftSpeed = 0;
    if (abs(rightSpeed) < 5) rightSpeed = 0;

    // apply max speed scaling
    leftSpeed = leftSpeed * (maxSpeed / 100.0);
    rightSpeed = rightSpeed * (maxSpeed / 100.0);

    // left side
    if (leftSpeed == 0) {
      Left_Front.stop();
      Left_Back.stop();
    } else {
      Left_Front.spin(forward, leftSpeed, percent);
      Left_Back.spin(forward, leftSpeed, percent);
    }

    // right side
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
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // create competition instance
  competition Competition;

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(userControl);

  // Run the pre-autonomous function.
  preAutonomous();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
