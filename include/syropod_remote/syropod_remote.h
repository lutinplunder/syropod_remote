#ifndef SYROPOD_REMOTE_H
#define SYROPOD_REMOTE_H
/*******************************************************************************************************************//**
 *  @file    syropod_remote.h
 *  @brief   Header file for Syropod Remote node.
 *
 *  @author  Fletcher Talbot (fletcher.talbot@csiro.au)
 *  @date    June 2017
 *  @version 0.5.0
 *
 *  CSIRO Autonomous Systems Laboratory
 *  Queensland Centre for Advanced Technologies
 *  PO Box 883, Kenmore, QLD 4069, Australia
 *
 *  (c) Copyright CSIRO 2017
 *
 *  All rights reserved, no part of this program may be used
 *  without explicit permission of CSIRO
 *
***********************************************************************************************************************/

#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <sensor_msgs/Joy.h>
#include <syropod_remote/AndroidSensor.h>
#include <syropod_remote/AndroidJoy.h>

#include <syropod_highlevel_controller/standard_includes.h>
#include <syropod_highlevel_controller/parameters_and_states.h>

#define NO_ROTATION 0.0
#define ENABLE_ROTATION 1.0
#define ROTATE_CLOCKWISE -1.0
#define ROTATE_COUNTERCLOCKWISE 1.0

#define DEAD_ZONE 0.05

enum JoypadButtonIndex
{
  A_BUTTON,
  B_BUTTON,
  X_BUTTON,
  Y_BUTTON,
  LEFT_BUMPER,
  RIGHT_BUMPER,
  BACK,
  START,
  LOGITECH,
  LEFT_JOYSTICK,
  RIGHT_JOYSTICK,
};

enum JoypadAxisIndex
{
  PRIMARY_X,
  PRIMARY_Y,
  PRIMARY_Z,
  SECONDARY_X,
  SECONDARY_Y,
  SECONDARY_Z,
  DPAD_LEFT_RIGHT,
  DPAD_UP_DOWN,
};

enum InterfaceType
{
  JOYPAD,
  TABLET_JOY,
  TABLET_SENSOR,
  KEYBOARD,
  UNASSIGNED = -1,
};

enum TipVelocityInputMode
{
  XY_MODE,
  ZY_MODE,
  TIP_VELOCITY_INPUT_MODE_COUNT,
};

struct SyropodRemoteParameters 
{
  Parameter<int> publish_rate;
  Parameter<bool> invert_compass;
  Parameter<bool> invert_imu;
  Parameter<int> imu_sensitivity;
};

class Remote
{
public:
  Remote(void);
  
  inline SystemState getSystemState(void) { return system_state_; };
  inline void setLegCount(int count) { leg_count_ = count; };
  inline void resetKonamiCode(void) { konami_code_ = 0; };
  
  void updateSystemState(void);
  void checkKonamiCode(void);
  void updateRobotState(void);
  void updateGaitSelection(void);
  void updateCruiseControlMode(void);
  void updatePlannerMode(void);
  void updatePosingMode(void);
  void updatePoseResetMode(void);
  void updateParameterAdjustment(void);
  void updatePrimaryLegSelection(void);
  void updateSecondaryLegSelection(void);
  void updatePrimaryLegState(void);
  void updateSecondaryLegState(void);
  void updateDesiredVelocity(void);
  void updateDesiredPose(void);
  void updateTipVelocityModes(void);
  void updatePrimaryTipVelocity(void);
  void updateSecondaryTipVelocity(void);
  
  void applyDeadZone(geometry_msgs::Point* axis);
  void applyDeadZone(sensor_msgs::Joy* joy);
  
  void resetMessages(void);
  void publishMessages(void);
  
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  void keyCallback(const sensor_msgs::Joy::ConstPtr& key);
  void androidJoyCallback(const syropod_remote::AndroidJoy::ConstPtr& control);
  void androidSensorCallback(const syropod_remote::AndroidSensor::ConstPtr& control);
  
  /**
   * Body velocity data from external source
   * @params[in] twist Input body velocity message data
   */
  void externalBodyVelocityCallback(const geometry_msgs::Twist &twist);
  
  /**
   * Pose velocity data from external source
   * @params[in] twist Input pose velocity message data
   */
  void externalPoseVelocityCallback(const geometry_msgs::Twist &twist);

private:
  SyropodRemoteParameters params_;
  sensor_msgs::Joy joypad_control_;
  syropod_remote::AndroidJoy android_joy_control_;
  syropod_remote::AndroidSensor android_sensor_control_;
  
  ros::Subscriber android_sensor_sub_;
  ros::Subscriber android_joy_sub_;
  ros::Subscriber joypad_sub_;
  ros::Subscriber keyboard_sub_;
  ros::Subscriber auto_navigation_sub_;
  
  ros::Subscriber external_body_velocity_pub_;
  ros::Subscriber external_pose_velocity_pub_;
  
  ros::Publisher desired_velocity_pub_;
  ros::Publisher desired_pose_pub_;
  ros::Publisher primary_tip_velocity_pub_;
  ros::Publisher secondary_tip_velocity_pub_;
  
  ros::Publisher system_state_pub_;
	ros::Publisher robot_state_pub_;
  ros::Publisher gait_selection_pub_;
  ros::Publisher posing_mode_pub_;
  ros::Publisher cruise_control_pub_;
  ros::Publisher auto_navigation_pub_;
  ros::Publisher planner_mode_pub_;
  ros::Publisher primary_leg_selection_pub_;
  ros::Publisher secondary_leg_selection_pub_;
  ros::Publisher primary_leg_state_pub_;
  ros::Publisher secondary_leg_state_pub_;
  ros::Publisher parameter_selection_pub_;
  ros::Publisher parameter_adjustment_pub_;
  ros::Publisher pose_reset_pub_;
  
  SystemState system_state_ = SUSPENDED;
  RobotState robot_state_ = PACKED;
  GaitDesignation gait_selection_ = GAIT_UNDESIGNATED;
  CruiseControlMode cruise_control_mode_ = CRUISE_CONTROL_OFF;
  PlannerMode planner_mode_ = PLANNER_MODE_OFF;
  PosingMode posing_mode_ = NO_POSING;
  PoseResetMode pose_reset_mode_ = NO_RESET;
  LegDesignation primary_leg_selection_ = LEG_UNDESIGNATED;
  LegDesignation secondary_leg_selection_ = LEG_UNDESIGNATED;
  LegState primary_leg_state_ = WALKING;
  LegState secondary_leg_state_ = WALKING;
  ParameterSelection parameter_selection_ = NO_PARAMETER_SELECTION;
  TipVelocityInputMode primary_tip_velocity_input_mode_ = XY_MODE;
  TipVelocityInputMode secondary_tip_velocity_input_mode_ = XY_MODE;
  
  geometry_msgs::Twist desired_velocity_msg_;
  geometry_msgs::Twist desired_pose_msg_; 
  geometry_msgs::Twist external_body_velocity_msg_;
  geometry_msgs::Twist external_pose_velocity_msg_;
  
  geometry_msgs::Point primary_tip_velocity_msg_;
  geometry_msgs::Point secondary_tip_velocity_msg_;
  std_msgs::Int8 system_state_msg_;
  std_msgs::Int8 robot_state_msg_;
  std_msgs::Int8 gait_selection_msg_;
  std_msgs::Int8 cruise_control_mode_msg_;
  std_msgs::Int8 planner_mode_msg_;
  std_msgs::Int8 auto_navigation_mode_msg_;
  std_msgs::Int8 posing_mode_msg_;
  std_msgs::Int8 pose_reset_mode_msg_;
  std_msgs::Int8 primary_leg_selection_msg_;
  std_msgs::Int8 secondary_leg_selection_msg_;
  std_msgs::Int8 primary_leg_state_msg_;
  std_msgs::Int8 secondary_leg_state_msg_;
  std_msgs::Int8 parameter_selection_msg_;
  std_msgs::Int8 parameter_adjustment_msg_;

  //Debounce booleans for buttons
  bool debounce_logitech_ = true;
  bool debounce_start_ = true;
  bool debounce_back_ = true;
  bool debounce_a_ = true;
  bool debounce_b_ = true;
  bool debounce_x_ = true;
  bool debounce_y_ = true;
  bool debounce_dpad_ = true;
  bool debounce_left_bumper_ = true;
  bool debounce_right_bumper_ = true; 
  bool debounce_left_joystick_ = true;
  bool debounce_right_joystick_ = true;

  int leg_count_;

  bool primary_z_axis_corrected_ = true;
  bool secondary_z_axis_corrected_ = true;

  int konami_code_ = 0;
  
  string current_priority_interface_;
  string default_priority_interface_;
  bool priority_interface_overridden_ = false;
  InterfaceType current_interface_type_ = UNASSIGNED;
};

/***********************************************************************************************************************
***********************************************************************************************************************/
#endif /* SYROPOD_REMOTE_H */

