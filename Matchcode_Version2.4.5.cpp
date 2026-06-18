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

inertial Inertial1 = inertial(PORT1);

// AI Vision Color Descriptions
// AI Vision Code Descriptions
vex::aivision AIVision11(PORT11, aivision::ALL_TAGS);

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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       12223F                                                    */
/*    Created:      16/06/26                                                  */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code
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
  Left_Front.spinFor(forward, 25.3, turns, false);
  Right_Front.spinFor(forward, 25.3, turns, false);
  Right_Back.spinFor(forward, 25.3, turns), false;
  Left_Back.spinFor(forward, 25.3, turns);

  // Turn Left
  Left_Front.spinFor(forward, 1, turns, false);
  Left_Back.spinFor(forward, 1, turns, false);
  Right_Front.spinFor(reverse, 1, turns, false);
  Right_Back.spinFor(reverse, 1, turns);

  // Drive to goal
  Left_Front.spinFor(forward, 25.3, turns, false); 
  Left_Back.spinFor(forward, 25.3, turns, false);
  Right_Front.spinFor(forward, 25.3, turns, false);
  Right_Back.spinFor(forward, 25.3, turns, false);

  // Place pin
  Claw.spinFor(reverse, 1, turns, false);
  arm.spin(reverse);

  // Reverse from goal
  Left_Front.spinFor(reverse, 25.3, turns, false);
  Right_Front.spinFor(reverse, 25.3, turns, false);
  Left_Back.spinFor(reverse, 25.3, turns, false);
  Right_Back.spinFor(reverse, 25.3, turns);

  // Turn left to be facing the wall
  Left_Front.spinFor(forward, 1, turns, false);
  Left_Back.spinFor(forward, 1, turns, false);
  Right_Front.spinFor(reverse, 1, turns, false);
  Right_Back.spinFor(reverse, 1, turns);

  // Drive to colour switcher thingy
  Left_Front.spinFor(forward, 25.3, turns, false);
  Right_Front.spinFor(forward, 25.3, turns, false);
  Right_Back.spinFor(forward, 25.3, turns, false);
  Left_Back.spinFor(forward, 25.3, turns, false);
  
}
