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
#define CAMERA_LINIT_SWITCH_PIN 10
#define BASE_LIMIT_SWITCH_PIN 11

#include "AFMotor.h"

class Scanner{
public:
  Scanner();
  void init();
  
  uint8_t baseTurn(uint32_t turn_degree);
  void setAngularSpeed(uint32_t degrees_per_sec);
  
  uint8_t cameraClimb(uint32_t distance_mm);
  void setHeightSpeed(uint32_t mm_per_sec);

  uint8_t doFullScan();
  void emergencyStop();


  
private:
  uint8_t safeMove(AF_Stepper &motor, uint32_t &current_position, uint32_t distance, uint32_t limit);
  void reset_motor(AF_Stepper &motor);
  AF_Stepper CameraMotor, BaseMotor;

  uint32_t baseAngle;
  uint32_t mCameraPosition;
};

