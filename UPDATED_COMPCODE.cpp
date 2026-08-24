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

motor Left_Back = motor(PORT14, ratio18_1, false);

motor Right_Back = motor(PORT19, ratio18_1, true);

controller Controller1 = controller(primary);
// AI Vision Color Descriptions
// AI Vision Code Descriptions
vex::aivision AIVision11(PORT11, aivision::ALL_TAGS);

inertial Inertial1 = inertial(PORT1);

motor Claw = motor(PORT3, ratio18_1, false);

motor arm = motor(PORT8, ratio18_1, false);



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
// define variables used for controlling motors based on controller inputs
bool Controller1RightShoulderControlMotorsStopped = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // check the ButtonR1/ButtonR2 status to control arm
      if (Controller1.ButtonR1.pressing()) {
        arm.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing()) {
        arm.spin(reverse);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        arm.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration


// ----------------------------------------------------------------------------
//
//    Project:
//    Author:
//    Created:
//    Configuration:
//
// ----------------------------------------------------------------------------


#include "vex.h"

using namespace vex;

// Begin project code

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

            // Alignment
            int error = obj.centerX - CENTER_X;
            double turnSpeed = error * 0.4;

            // Limit turn speed
            if (turnSpeed > 50)
                turnSpeed = 50;

            if (turnSpeed < -50)
                turnSpeed = -50;

            // Drive toward tag
            if (obj.width < TARGET_WIDTH) {

                double leftSpeed = 30 + turnSpeed;
                double rightSpeed = 30 - turnSpeed;

                Left_Front.spin(forward, leftSpeed, percent);
                Left_Back.spin(forward, leftSpeed, percent);

                Right_Front.spin(forward, rightSpeed, percent);
                Right_Back.spin(forward, rightSpeed, percent);

            } else {

                Left_Front.stop(hold);
                Left_Back.stop(hold);
                Right_Front.stop(hold);
                Right_Back.stop(hold);
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

// Pre-auto
void pre_auton(void) {

    Brain.Screen.print("Pre-auto");

    Inertial1.calibrate();

    wait(1, seconds);
}



// Auto helpers
// Drive all four drivetrain motors together
void driveFor(directionType direction, double motorTurns) {

    Left_Front.spinFor(direction, motorTurns, turns, false);
    Left_Back.spinFor(direction, motorTurns, turns, false);
    Right_Front.spinFor(direction, motorTurns, turns, false);
    Right_Back.spinFor(direction, motorTurns, turns);
}


// Stop drivetrain
void stopDrive() {

    Left_Front.stop(hold);
    Left_Back.stop(hold);
    Right_Front.stop(hold);
    Right_Back.stop(hold);
}


// Turn left using inertial sensor
void turnLeft(double targetDegrees) {

    double startRotation = Inertial1.rotation(degrees);
    double targetRotation = startRotation + targetDegrees;

    Left_Front.spin(forward, 40, percent);
    Left_Back.spin(forward, 40, percent);

    Right_Front.spin(reverse, 40, percent);
    Right_Back.spin(reverse, 40, percent);

    int elapsed = 0;

    while (Inertial1.rotation(degrees) < targetRotation - 1) {

        wait(10, msec);

        elapsed += 10;

        // Safety timeout
        if (elapsed >= 3000) {
            break;
        }
    }

    stopDrive();

    wait(100, msec);
}


// Turn right using inertial sensor
void turnRight(double targetDegrees) {

    double startRotation = Inertial1.rotation(degrees);
    double targetRotation = startRotation - targetDegrees;

    Left_Front.spin(reverse, 40, percent);
    Left_Back.spin(reverse, 40, percent);

    Right_Front.spin(forward, 40, percent);
    Right_Back.spin(forward, 40, percent);

    int elapsed = 0;

    while (Inertial1.rotation(degrees) > targetRotation + 1) {

        wait(10, msec);

        elapsed += 10;

        // Safety timeout
        if (elapsed >= 3000) {
            break;
        }
    }

    stopDrive();

    wait(100, msec);
}


// Autonomous period
void autonomous(void) {

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Autonomous");


    // Initialise velocity
    Left_Front.setVelocity(50, percent);
    Right_Front.setVelocity(50, percent);
    Left_Back.setVelocity(50, percent);
    Right_Back.setVelocity(50, percent);


    // Set stopping for mechanisms
    Claw.setStopping(hold);
    arm.setStopping(hold);

    // Calibrate inertial sensor
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("Calibrating Inertial");

    Inertial1.calibrate();

    while (Inertial1.isCalibrating()) {
        wait(20, msec);
    }

    Inertial1.setRotation(0, degrees);

    // Pickup preload
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Preload");

    Claw.spin(forward);
    arm.spin(forward);

    wait(600, msec);

    Claw.stop(hold);
    arm.stop(hold);

    // Drive away from start
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Drive 1");

    driveFor(forward, 25.3);

    stopDrive();

    wait(100, msec);

    // Turn left 90 degrees
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Turn Left");

    turnLeft(90);

    // Drive to goal
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Drive Goal");

    driveFor(forward, 25.3);

    stopDrive();

    wait(100, msec);


    // Place pin
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Place Pin");

    Claw.spin(reverse);
    arm.spin(reverse);

    wait(500, msec);

    Claw.stop(hold);
    arm.stop(hold);

    // Reverse
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Reverse");

    driveFor(reverse, 25.3);

    stopDrive();

    wait(100, msec);

    // Done :D
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("AUTO COMPLETE");
    stopDrive();
}

// Driver control
void usercontrol(void) {
  arm.setStopping(hold);
  Claw.setStopping

    double maxSpeed = 80;

    while (true) {

        // HOLD X FOR AUTO ALIGN
        if (Controller1.ButtonX.pressing()) {
            align();
            continue;
        }

        // Speed adjust button
        if (Controller1.ButtonY.pressing()) {
            maxSpeed += 5;
            if (maxSpeed > 100)
                maxSpeed = 100;
            wait(200, msec);
        }

        if (Controller1.ButtonA.pressing()) {
            maxSpeed -= 5;
            if (maxSpeed < 20)
                maxSpeed = 20;
            wait(200, msec);
        }

        // Arm
        if (Controller1.ButtonL1.pressing()) {
            arm.spin(forward);
        } else if (Controller1.ButtonL2.pressing()) {
            arm.spin(reverse);
        } else {
            arm.stop(hold);
        }
      
        // Claw
        if (Controller1.ButtonR1.pressing()) {
            Claw.spin(forward);
        } else if (Controller1.ButtonR2.pressing()) {
            Claw.spin(reverse);
        } else {
            Claw.stop(hold);
        }

        // Drivetrain stuff
        double leftSpeed = Controller1.Axis3.position();
        double rightSpeed = Controller1.Axis2.position();

        // Deadzone
        if (abs(leftSpeed) < 5)
            leftSpeed = 0;
        if (abs(rightSpeed) < 5)
            rightSpeed = 0;
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
