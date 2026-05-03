#pragma region VEXcode Generated Robot Configuration
#include "vex.h"

using namespace vex;

brain Brain;

// Controller
controller Controller1 = controller(primary);

// Motors
motor Left_Front = motor(PORT17, ratio18_1, false);
motor Right_Front = motor(PORT4, ratio18_1, true);
motor Left_Back = motor(PORT10, ratio18_1, false);
motor Right_Back = motor(PORT2, ratio18_1, true);

// Sensors
inertial Inertial1 = inertial(PORT1);
vex::aivision AIVision1(PORT11, aivision::ALL_TAGS);

void vexcodeInit() {}
#pragma endregion

// ------------------- AUTO ALIGN FUNCTION -------------------
void align() {

  const int CENTER_X = 160;
  const int TARGET_WIDTH = 120;

  while (Controller1.ButtonX.pressing()) {

    AIVision1.takeSnapshot(aivision::ALL_TAGS);

    if (AIVision1.objectCount > 0) {

      aivision::object obj = AIVision1.largestObject;

      // ===== Controller display =====
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("Tag: %d", obj.id);

      // ===== Alignment error =====
      int error = obj.centerX - CENTER_X;
      double turnSpeed = error * 0.4;

      // Clamp turn speed
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
      }

    } else {
      // No tag found
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

// ------------------- PRE-AUTO -------------------
void pre_auton(void) {
  Brain.Screen.print("Pre-auton");
  Inertial1.calibrate();
  wait(1, seconds);
}

// ------------------- AUTONOMOUS -------------------
void autonomous(void) {
  Brain.Screen.print("Autonomous");
}

// ------------------- DRIVER CONTROL -------------------
void usercontrol(void) {

  double maxSpeed = 80;

  while (true) {

    // HOLD X FOR AUTO ALIGN
    if (Controller1.ButtonX.pressing()) {
      align();
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

// ------------------- MAIN -------------------
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
