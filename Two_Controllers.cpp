#pragma region VEXcode Generated Robot Configuration
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

brain Brain;


#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)


motor Left_Front = motor(PORT17, ratio18_1, false);

motor Right_Front = motor(PORT4, ratio18_1, false);

motor Left_Back = motor(PORT10, ratio18_1, false);

motor Right_Back = motor(PORT2, ratio18_1, false);

motor Claw = motor(PORT3, ratio18_1, false);

motor arm = motor(PORT5, ratio18_1, false);

inertial Inertial1 = inertial(PORT1);


vex::aivision AIVision11(PORT11);

controller Controller1 = controller(primary);
controller Controller2 = controller(partner);


void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  srand(seed);
}



void vexcodeInit() {

  initializeRandomSeed(); 
}

void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}

bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration


// ----------------------------------------------------------------------------
//
//    Project: Two Controller Competiton Code
//    Author: 12223F (Flamingos)
//    Created: 20/08/26
//    Configuration: Competition Robot
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
    Controller1.Screen.print("Controller 1");
    Controller2.Screen.print("Controller 2");

    Inertial1.calibrate();

    wait(1, seconds);
}

// Stop drivetrain
void stopDrive() {

    Left_Front.stop(hold);
    Left_Back.stop(hold);
    Right_Front.stop(hold);
    Right_Back.stop(hold);
}

// Auto helpers
// Drive all four drivetrain motors together
void driveFor(directionType direction, double motorTurns) {

    // Target heading is whatever the robot's heading is
    // when this function begins.
    double targetHeading = Inertial1.rotation(degrees);

    // Heading-hold proportional gain
    double Kp = 1.2;

    // Reset motor positions
    Left_Front.resetPosition();
    Left_Back.resetPosition();
    Right_Front.resetPosition();
    Right_Back.resetPosition();

    // Start all motors
    Left_Front.spin(direction, 50, percent);
    Left_Back.spin(direction, 50, percent);
    Right_Front.spin(direction, 50, percent);
    Right_Back.spin(direction, 50, percent);

    while (fabs(Left_Front.position(turns)) < motorTurns) {

        double currentHeading = Inertial1.rotation(degrees);

        double error = targetHeading - currentHeading;

        double correction = error * Kp;

        // Limit correction
        if (correction > 20)
            correction = 20;

        if (correction < -20)
            correction = -20;

        double leftSpeed;
        double rightSpeed;

        if (direction == forward) {
            leftSpeed = 50 + correction;
            rightSpeed = 50 - correction;
        } else {
            leftSpeed = 50 - correction;
            rightSpeed = 50 + correction;
        }

        // Limit speeds
        if (leftSpeed > 100)
            leftSpeed = 100;

        if (leftSpeed < -100)
            leftSpeed = -100;

        if (rightSpeed > 100)
            rightSpeed = 100;

        if (rightSpeed < -100)
            rightSpeed = -100;

        Left_Front.spin(direction, fabs(leftSpeed), percent);
        Left_Back.spin(direction, fabs(leftSpeed), percent);

        Right_Front.spin(direction, fabs(rightSpeed), percent);
        Right_Back.spin(direction, fabs(rightSpeed), percent);

        wait(10, msec);
    }

    stopDrive();
}


// Drive a precise distance in millimeters while holding heading
void driveStraightMM(double distanceMM, directionType direction) {

    // 3.25 inch wheel
    const double WHEEL_DIAMETER_MM = 3.25 * 25.4;

    // Wheel circumference
    const double WHEEL_CIRCUMFERENCE_MM =
        WHEEL_DIAMETER_MM * M_PI;

    // Convert distance to wheel revolutions
    double wheelTurns =
        distanceMM / WHEEL_CIRCUMFERENCE_MM;

    // Target heading
    double targetHeading = Inertial1.rotation(degrees);

    // Heading proportional gain
    double Kp = 1.2;

    // Reset motor positions
    Left_Front.resetPosition();
    Left_Back.resetPosition();
    Right_Front.resetPosition();
    Right_Back.resetPosition();

    while (fabs(Left_Front.position(turns)) < wheelTurns) {

        double currentHeading = Inertial1.rotation(degrees);

        double error = targetHeading - currentHeading;

        double correction = error * Kp;

        // Limit correction
        if (correction > 20)
            correction = 20;

        if (correction < -20)
            correction = -20;

        double baseSpeed = 50;

        double leftSpeed;
        double rightSpeed;

        if (direction == forward) {
            leftSpeed = baseSpeed + correction;
            rightSpeed = baseSpeed - correction;
        } else {
            leftSpeed = baseSpeed - correction;
            rightSpeed = baseSpeed + correction;
        }

        // Limit speeds
        if (leftSpeed > 100)
            leftSpeed = 100;

        if (leftSpeed < -100)
            leftSpeed = -100;

        if (rightSpeed > 100)
            rightSpeed = 100;

        if (rightSpeed < -100)
            rightSpeed = -100;

        Left_Front.spin(direction, fabs(leftSpeed), percent);
        Left_Back.spin(direction, fabs(leftSpeed), percent);

        Right_Front.spin(direction, fabs(rightSpeed), percent);
        Right_Back.spin(direction, fabs(rightSpeed), percent);

        wait(10, msec);
    }

    stopDrive();
}


// Turn left using inertial sensor
void turnLeft(double targetDegrees) {

    double startRotation = Inertial1.rotation(degrees);
    double targetRotation = startRotation + targetDegrees;

    // PID constants
    double Kp = 0.9;
    double Kd = 0.25;

    double previousError = targetDegrees;

    int elapsed = 0;

    while (true) {

        double currentRotation = Inertial1.rotation(degrees);

        double error = targetRotation - currentRotation;

        // Stop when within 1 degree
        if (fabs(error) <= 1)
            break;

        double derivative = error - previousError;

        double turnSpeed =
            (Kp * error) + (Kd * derivative);

        // Minimum turning speed
        if (turnSpeed < 12)
            turnSpeed = 12;

        // Maximum turning speed
        if (turnSpeed > 60)
            turnSpeed = 60;

        Left_Front.spin(forward, turnSpeed, percent);
        Left_Back.spin(forward, turnSpeed, percent);

        Right_Front.spin(reverse, turnSpeed, percent);
        Right_Back.spin(reverse, turnSpeed, percent);

        previousError = error;

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

    // PID constants
    double Kp = 0.9;
    double Kd = 0.25;

    double previousError = -targetDegrees;

    int elapsed = 0;

    while (true) {

        double currentRotation = Inertial1.rotation(degrees);

        double error = targetRotation - currentRotation;

        // Stop when within 1 degree
        if (fabs(error) <= 1)
            break;

        double derivative = error - previousError;

        double turnSpeed =
            (Kp * fabs(error)) + (Kd * fabs(derivative));

        // Minimum turning speed
        if (turnSpeed < 12)
            turnSpeed = 12;

        // Maximum turning speed
        if (turnSpeed > 60)
            turnSpeed = 60;

        Left_Front.spin(reverse, turnSpeed, percent);
        Left_Back.spin(reverse, turnSpeed, percent);

        Right_Front.spin(forward, turnSpeed, percent);
        Right_Back.spin(forward, turnSpeed, percent);

        previousError = error;

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

    Claw.spin(reverse);
      wait(600, msec);
    Claw.stop(hold);

    arm.spin(reverse);
      wait(1000, msec);
    arm.stop(hold);

    // Drive away from start
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Drive 1");

    driveStraightMM(600, forward);

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

    driveStraightMM(600, forward);

    stopDrive();

    wait(100, msec);


    // Place pin
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Place Pin");

    arm.spin(forward);
      wait(1000, msec);
    arm.stop(hold);

    Claw.spin(forward);
      wait(600, msec);
    Claw.stop(hold);

    wait(500, msec);

    // Reverse
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Reverse");

    driveStraightMM(600, reverse);

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

        // Arm
        if (Controller2.ButtonL1.pressing()) {
            arm.spin(forward);
        } else if (Controller2.ButtonL2.pressing()) {
            arm.spin(reverse);
        } else {
            arm.stop(hold);
        }
      
        // Claw
        if (Controller2.ButtonR1.pressing()) {
            Claw.spin(forward);
        } else if (Controller2.ButtonR2.pressing()) {
            Claw.spin(reverse);
        } else {
            Claw.stop(hold);
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
