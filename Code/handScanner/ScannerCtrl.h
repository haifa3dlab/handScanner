#ifndef ScannerCtrl_h
#define ScannerCtrl_h

#include "AFMotor.h"

// Base motor parameters
#define BASE_CHANNEL 1
#define BASE_MAX_ANGLE 300
#define BASE_DEGREE_PER_STEP 1.8 

// Camera motor parameters
#define CAMERA_CHANNEL 2
#define CAMERA_STEPS_PER_MM 12.50 
#define CAMERA_MAX_DIST 150
#define CAMERA_DEGREE_PER_STEP 1.8 // steps per revolution = 360 / 1.8

// End-Stop - limit switches PINs
#define CAMERA_LIMIT_SWITCH_PIN 10
#define BASE_LIMIT_SWITCH_PIN 11

// Interrupts for emergency stop
#define interruptPin 2

const bool DEBUG_SCANNER = false;

enum errType {
  err_ok = 0,
  err_exceeds_limits = 1,
};



class Scanner{
public:
  Scanner();
  void init();
  
  errType baseTurn(int turn_degree);
  void setAngularSpeed(uint32_t degrees_per_sec);
  
  errType cameraMove(int distance_mm);
  void setHeightSpeed(uint32_t mm_per_sec);

  uint8_t doFullScan();
  static void emergencyStop();

  void releaseMotors();

  uint32_t getBaseAngle();
  uint32_t getCameraPosition();
  
private:
  uint8_t safeMove(AF_Stepper &motor, uint32_t &current_position, uint32_t distance, uint32_t limit);
  void ResetBaseMotor();
  void ResetCameraMotor();
  AF_Stepper CameraMotor, BaseMotor;

  uint32_t baseAngle;
  uint32_t mCameraPosition;
  uint32_t steps;
};

#endif
