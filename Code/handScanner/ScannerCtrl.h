#ifndef ScannerCtrl_h
#define ScannerCtrl_h

/*
 * Adafruit Motor shield library doc:
 * https://learn.adafruit.com/adafruit-motor-shield
 * https://learn.adafruit.com/adafruit-motor-shield/faq
 * library: https://learn.adafruit.com/adafruit-motor-shield/downloads
 */
#include "AFMotor.h"

#include "StopSensor.h"

#include "Debuggable.h"

#define SEC_PER_MIN 60

// For steppers we use:
#define STEP_PER_REV 200

// minimal speed for (de)accel:
#define MIN_STEPS_PER_SEC 4

// Base motor parameters
#define BASE_CHANNEL 2
#define BASE_MAX_ANGLE 300
const float BASE_STEP_PER_DEGREE = 7.47F;

// Camera motor parameters
#define CAMERA_CHANNEL 1
#define CAMERA_MAX_DIST 212
const float CAMERA_STEPS_PER_MM = 8.0F;

// End-Stop - limit switches PINs
#define CAMERA_LIMIT_SWITCH_PIN 9
#define BASE_LIMIT_SWITCH_PIN 10

// max possible steps - for case of StopSensor failure
#define ANGLE_MAX_STEPS 0 //1000, 0 - for now without StopSensors
#define HEIGHT_MAX_STEPS 0 //1000, 0 - for now without StopSensors

// (de)acceleration params:
#define CAMERA_MOUNT_RAD 275     // mm - copy from params.scad !

#define STEPS_PER_ACCEL_ROUND 1
// Max base mount tips acceleration in g - earth gravity acceleration
const float maxAccelG = 0.001F;
const float gEarth = 9806.65F;    // mm/(sec^2)
const float maxAccelDeg = (maxAccelG * gEarth) * 180.0F / (PI * CAMERA_MOUNT_RAD);  // deg/(sec^2)
const float maxAccelStepsPerSqSec = maxAccelDeg * BASE_STEP_PER_DEGREE; // steps/(sec^2)

// full scan bands:
#define SCAN_BAND_HEIGHT_MM 50
const int fullScanMinDeg = 0;
const int fullScanMaxDeg = BASE_MAX_ANGLE;
const int fullScanMinDist = 0;
const int fullScanMaxDist = CAMERA_MAX_DIST;

// Interrupts for emergency stop
#define interruptPin 2

const bool DEBUG_SCANNER = true; //false;

enum errType {
  err_ok = 0,
  err_exceeds_limits = 1,
  err_fullscan_base_turn,
  err_fullscan_camera_move,
  err_bad_cmd,
  err_bad_param
};



class Scanner : public Debuggable
{
public:
  Scanner();
  void init();
  
  errType baseTurn(int toDegree);
  errType baseTurnRel(int diffDegree);
  void setAngularSpeed(int degrees_per_sec);
  
  errType cameraMove(int toPos);
  errType cameraMoveRel(int diff_mm);
  void setHeightSpeed(int mm_per_sec);

  errType doFullScan();
  void emergencyStop();

  void releaseMotors();

  int getBaseAngle();
  int getCameraPosition();

  static float stepsPerSecToRPM(int steps_per_sec);

protected:
  int slowStartStop(AF_Stepper& motor, int& inout_fullSpeed, float accelStepsPerSqSec, long maxSteps, bool forward = true, bool speedUp = true);
  void ResetBaseMotor();
  void ResetCameraMotor();
  AF_Stepper CameraMotor, BaseMotor;
  StopSensor CameraStop, BaseStop;

  int mBaseAngle;
  int mCameraPosition;

  int angularSpeed; // steps/sec
  int heightSpeed; // steps/sec
};

#endif
