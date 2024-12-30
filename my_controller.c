#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/keyboard.h>
#include <webots/position_sensor.h>

#define TIME_STEP 8


static WbDeviceTag motors[9];
static WbDeviceTag position_sensors[7];

enum Joints {JOINT1, JOINT2, JOINT3, JOINT4, JOINT5, JOINT6, JOINT7, FINGER1, FINGER2};

void initializeDevices() {
  const char *joint_names[] = {
    "panda_joint1", "panda_joint2", "panda_joint3", "panda_joint4", "panda_joint5", "panda_joint6", "panda_joint7",
    "panda_finger_joint1", "panda_finger_joint2"
  };
  
  for (int i = 0; i < 9; i++) {
    motors[i] = wb_robot_get_device(joint_names[i]);
    if (i < 7) {
      position_sensors[i] = wb_motor_get_position_sensor(motors[i]);
      wb_position_sensor_enable(position_sensors[i], TIME_STEP);
    }
  }
}

void moveJoint(int joint_index, float angle) {
  double current_position = wb_position_sensor_get_value(position_sensors[joint_index]);
  double target_position = current_position + angle;
  wb_motor_set_position(motors[joint_index], target_position);
}

void controlRobot() {
  int key = wb_keyboard_get_key();
  switch (key) {
    case 'Q':  // 顺时针旋转第一个关节
      moveJoint(JOINT1, 0.01);
      break;
    case 'A':  // 逆时针旋转第一个关节
      moveJoint(JOINT1, -0.01);
      break;
    case 'W':
      moveJoint(JOINT2, 0.01);
      break;
    case 'S':
      moveJoint(JOINT2, -0.01);
      break;
    case 'E':
      moveJoint(JOINT3, 0.01);
      break;
    case 'D':
      moveJoint(JOINT3, -0.01);
      break;
    case 'R':
      moveJoint(JOINT4, 0.01);
      break;
    case 'F':
      moveJoint(JOINT4, -0.01);
      break;
    case 'T':
      moveJoint(JOINT5, 0.01);
      break;
    case 'G':
      moveJoint(JOINT5, -0.01);
      break;
    case 'Y':
      moveJoint(JOINT6, 0.01);
      break;
    case 'H':
      moveJoint(JOINT6, -0.01);
      break;
    case 'U':
      moveJoint(JOINT7, 0.01);
      break;
    case 'J':
      moveJoint(JOINT7, -0.01);
      break;
    case 'I':  // 夹爪闭合
      wb_motor_set_position(motors[FINGER1], 0.016);
      wb_motor_set_position(motors[FINGER2], 0.016);
      break;
    case 'K':  // 夹爪张开
      wb_motor_set_position(motors[FINGER1], 0.03);
      wb_motor_set_position(motors[FINGER2], 0.03);
      break;
    default:
      break;
  }
}

int main(int argc, char **argv) {
  wb_robot_init();
  initializeDevices();
  wb_keyboard_enable(TIME_STEP);
  while (wb_robot_step(TIME_STEP) != -1) {
    controlRobot();
  }
  wb_robot_cleanup();
  return 0;
}
