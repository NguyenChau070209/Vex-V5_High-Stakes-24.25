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
motor leftMotorMotorA = motor(PORT10, ratio18_1, false);
motor leftMotorMotorB = motor(PORT8, ratio18_1, false);
motor_group leftMotor = motor_group(leftMotorMotorA, leftMotorMotorB);

motor rightMotorMotorA = motor(PORT9, ratio18_1, true);
motor rightMotorMotorB = motor(PORT7, ratio18_1, true);
motor_group rightMotor = motor_group(rightMotorMotorA, rightMotorMotorB);

inertial Inertial11 = inertial(PORT11);



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


// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;



// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS




#pragma endregion VEXcode Generated Robot Configuration


// Hàm điều khiển tiến/lùi kèm sửa hướng
void driveWithHeading(double targetAngle, double speed, double duration) {
    double startTime = Brain.timer(seconds);
    
    while (Brain.timer(seconds) - startTime < duration) {
        // Đọc góc hiện tại
        double currentAngle = Inertial11.heading();

        // Tính độ lệch
        double error = targetAngle - currentAngle;
        
        // Hiệu chỉnh tốc độ động cơ
        double correction = error * 0.5; // Hệ số sửa lỗi
        double leftSpeed = speed + correction;
        double rightSpeed = speed - correction;

        // Đảm bảo tốc độ không vượt quá 100 hoặc dưới -100
        leftSpeed = fmin(fmax(leftSpeed, -100), 100);
        rightSpeed = fmin(fmax(rightSpeed, -100), 100);

        // Đặt tốc độ động cơ
        leftMotor.spin(vex::directionType::fwd, leftSpeed, vex::velocityUnits::pct);
        rightMotor.spin(vex::directionType::fwd, rightSpeed, vex::velocityUnits::pct);

        // Chờ để ổn định
        vex::task::sleep(20);
    }

    // Dừng động cơ sau khi hết thời gian
    leftMotor.stop();
    rightMotor.stop();
}






// Hàm hiệu chỉnh hướng
void correctHeading(double targetAngle) {
    // Đọc góc hiện tại
    double currentAngle = Inertial11.heading(); 

    // Tính độ lệch
    double error = targetAngle - currentAngle;

    // Sửa góc (giảm sai số)
    while (fabs(error) > 1.0) {  // Dừng khi sai số nhỏ hơn 1 độ
        error = targetAngle - Inertial11.heading();

        // Nếu góc hiện tại thấp hơn mục tiêu, quay trái (với tốc độ âm cho động cơ phải)
        double speed = error * 0.5; // Hệ số hiệu chỉnh
        leftMotor.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
        rightMotor.spin(vex::directionType::rev, speed, vex::velocityUnits::pct);
        
        // Chờ để ổn định
        vex::task::sleep(20);
    }

    // Dừng động cơ
    leftMotor.stop();
    rightMotor.stop();
}







// "when started" hat block
int whenStarted1() {
  Inertial11.calibrate();

    // Khởi động cảm biến Inertial
    Inertial11.calibrate();
    while (Inertial11.isCalibrating()) {
        vex::task::sleep(100);
    }

    Brain.Screen.print("Inertial Sensor Ready!");
    //Góc rẽ
    //correctHeading(90);

    // Ví dụ: Điều chỉnh đến góc 90 độ và di chuyển thẳng
    driveWithHeading(90, 50, 2);  // Di chuyển thẳng với tốc độ 50% trong 2 giây

    // Đổi hướng tại chỗ, quay 180 độ
    //driveWithHeading(90, 0, 2); // Xoay 90 độ (không di chuyển tiến/lùi)

    // Quay lại vị trí ban đầu
    //driveWithHeading(90, -50, 3); // Quay lại góc 90 độ với tốc độ -50 trong 3 giây (di chuyển lùi)

    return 0;
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Chạy các thao tác khi robot được khởi động
  whenStarted1();
}
