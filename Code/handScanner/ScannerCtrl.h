#ifndef ScannerCtrl_h
#define ScannerCtrl_h

#include "StopSensor.h"

#include "AFMotor.h"

// Base motor parameters
#define BASE_CHANNEL 1
#define BASE_MAX_ANGLE 300
const float BASE_STEP_PER_DEGREE = 7.47;

// Camera motor parameters
#define CAMERA_CHANNEL 2
#define CAMERA_MAX_DIST 350
const float CAMERA_STEPS_PER_MM = 10;

// End-Stop - limit switches PINs
#define CAMERA_LIMIT_SWITCH_PIN 9
#define BASE_LIMIT_SWITCH_PIN 10

// max possible steps - for case of StopSensor failure
#define ANGLE_MAX_STEPS 0 //1000, 0 - for now without StopSensors
#define HEIGHT_MAX_STEPS 0 //1000, 0 - for now without StopSensors

// full scan bands:
#define SCAN_BAND_HEIGHT_MM 50

// Interrupts for emergency stop
#define interruptPin 2

const bool DEBUG_SCANNER = true; //false;

enum errType {
  err_ok = 0,
  err_exceeds_limits = 1,
  err_fullscan_base_turn,
  err_fullscan_camera_move
};



class Scanner
{
public:
  Scanner();
  void init();
  
  errType baseTurn(int toDegree);
  void setAngularSpeed(uint32_t degrees_per_sec);
  
  errType cameraMove(int toPos);
  void setHeightSpeed(uint32_t mm_per_sec);

  uint8_t doFullScan();
  void emergencyStop();

  void releaseMotors();

  uint32_t getBaseAngle();
  uint32_t getCameraPosition();
  
private:
  uint8_t safeMove(AF_Stepper &motor, uint32_t &current_position, uint32_t toPos, uint32_t limit);
  void ResetBaseMotor();
  void ResetCameraMotor();
  AF_Stepper CameraMotor, BaseMotor;
  StopSensor CameraStop, BaseStop;

  uint32_t baseAngle;
  uint32_t mCameraPosition;
  uint32_t steps;
};

#endif
