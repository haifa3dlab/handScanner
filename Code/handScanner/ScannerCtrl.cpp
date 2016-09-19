#include "ScannerCtrl.h"


/**
 * Description: class default c'tor, also instantianite  the AF_Stepper objects
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: 
 * @return:
 * 
 * Constants: CAMERA_DEGREE_PER_STEP, CAMERA_CHANNEL, BASE_DEGREE_PER_STEP, BASE_CHANNEL
 */
Scanner::Scanner():CameraMotor(360 / CAMERA_DEGREE_PER_STEP, CAMERA_CHANNEL),BaseMotor(360 / BASE_DEGREE_PER_STEP, BASE_CHANNEL){;}


/**
 * Description: Initialization function, currently get called from main called (not c'tor) to initialize motors position
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: 
 * @return:
 * 
 * Constants: 
 */
void Scanner::init(){
    reset_motor(BaseMotor);
    reset_motor(CameraMotor);
}


/**
 * Description: Reset motor's position, function get called at initialization, motor moves back to limit-switched (end-stops)
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: &motor
 * @return: 
 * 
 * Constants: 
 */
void Scanner::reset_motor(AF_Stepper &motor){
    // turn motor until endstop reached
    // no int option:
    //  1. turn 1 step clockwise
    //  2. test if endstop pressed:
    //  3. if pressed break else continue
    bool ENDSTOP; // will be replaced with PIN read
    while ( ENDSTOP ){
        motor.step(1, FORWARD, SINGLE);
    }
    return;
}


/**
 * Description: turn the base, control the Base's motor, include protection from illegal moves
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: turn_degree (angle to move in unit degrees)
 * @return: error type
 * 
 * Constants: BASE_MAX_ANGLE, BASE_DEGREE_PER_STEP
 */
uint8_t Scanner::baseTurn(uint32_t turn_degree){
  // returns value for error log
  // 0 - ok
  // 1 - exceeds limits
  if (turn_degree + baseAngle >= BASE_MAX_ANGLE || turn_degree + baseAngle <= 0 ) return 1;
  if ( turn_degree >= 0 ){
    uint32_t steps = turn_degree  / BASE_DEGREE_PER_STEP ;
    BaseMotor.step(steps, FORWARD, SINGLE);
  }
  else {
    uint32_t steps = -turn_degree / BASE_DEGREE_PER_STEP ;
    BaseMotor.step(steps, BACKWARD, SINGLE);
  }
  baseAngle += turn_degree;
  return 0;
}


/**
 * Description: set Base motor speed in units of degree per sec
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: degrees_per_sec (set the base motor angular speed)
 * @return: 
 * 
 * Constants: 
 */
void Scanner::setAngularSpeed(uint32_t degrees_per_sec){
    // no input check or error handling
    BaseMotor.setSpeed( degrees_per_sec / 6 ); //  1 round/minute = 6 deg/sec
}


/**
 * Description: move the camera, control the Camera's motor
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: distance_mm (distance in mm, negative values to go up)
 * @return: error type
 * 
 * Constants: CAMERA_MAX_DIST, CAMERA_DEGREE_PER_STEP
 */
uint8_t Scanner::cameraClimb(uint32_t distance_mm){
  // returns value for error log
  // 0 - ok
  // 1 - exceeds limits
  if (distance_mm + mCameraPosition >= CAMERA_MAX_DIST || distance_mm + mCameraPosition <= 0 ) return 1;
  if ( distance_mm >= 0 ){
    uint32_t steps = distance_mm  / CAMERA_DEGREE_PER_STEP ;
    CameraMotor.step(steps, FORWARD, SINGLE);
  }
  else {
    uint32_t steps = -distance_mm / CAMERA_DEGREE_PER_STEP ;
    CameraMotor.step(steps, BACKWARD, SINGLE);
  }
  mCameraPosition += distance_mm;
  return 0;
}


/**
 * Description: set Camera motor speed in units of mm per sec
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: mm_per_sec
 * @return: 
 * 
 * Constants: CAMERA_STEPS_PER_MM, CAMERA_DEGREE_PER_STEP
 */
void Scanner::setHeightSpeed(uint32_t mm_per_sec){
    // no input check or error handling
    // 1 mm / sec = ( STEP_PER_MM )  steps / sec = (STEP_PER_MM * DEGREE_PER_STEP ) degree / sec = 
    // = ( STEP_PER_MM * DEGREE_PER_STEP * 60 ) degree / minute = STEP_PER_MM * DEGREE_PER_STEP / 6 rpm 
    CameraMotor.setSpeed( mm_per_sec * ( ( CAMERA_STEPS_PER_MM * CAMERA_DEGREE_PER_STEP )/ 6) ); 
}



/**
 * Description: calling this function initiate a full scan. basic function for automation
 * 
 * note: motors speed must be determined before calling this method (or deault values will apply)
 * 
 * 1. turn BASE_MAX_ANGLE
 * 2. lower camera in STEP_HEIGHT_MM amount
 * 3. turn back
 * 4. lower camera in STEP_HEIGHT_MM amount
 * 5. back to one until CAMERA_MAX_DIST reached
 * 
 * @Author: Hagai Solodar (19/09/2016)
 * 
 * @param: motor
 * @return: error type
 * 
 */
uint8_t Scanner::doFullScan(){
    const uint8_t STEP_HEIGHT_MM = 30;
    for (uint8_t curr_step = 0 ; curr_step < ( CAMERA_MAX_DIST / STEP_HEIGHT_MM ); curr_step++){
      if ( baseTurn(BASE_MAX_ANGLE) == 1 ) return 1; //error return value
      if ( cameraClimb(STEP_HEIGHT_MM) == 1 ) return 2; //error return value
      if ( baseTurn(-BASE_MAX_ANGLE) == 1) return 3; // error return value
      if ( cameraClimb(STEP_HEIGHT_MM) == 1) return 4; // error return value
    }
    return 0;
}



/**
 * Description: safe move (Function NOT IN USE!!!)
 * 
 * @Author: Hagai Solodar (19/09/2016)
 * 
 * @param: motor
 * @param: current_position
 * @param: ditance
 * @param: limit
 * @return: error type
 * 
 */
uint8_t Scanner::safeMove(AF_Stepper &motor, uint32_t &current_position, uint32_t distance, uint32_t limit){
  // returns value for error log
  // 0 - ok
  // 1 - exceeds limits
  if (distance + current_position >= limit || distance + current_position <= 0 ) return 1;
  if ( distance >= 0 ){
    uint32_t steps = distance  / BASE_DEGREE_PER_STEP ;
    CameraMotor.step(steps, FORWARD, SINGLE);
  }
  else {
    uint32_t steps = -distance / BASE_DEGREE_PER_STEP ;
    CameraMotor.step(steps, BACKWARD, SINGLE);
  }
  mCameraPosition += distance;
  return 0;  
}
/*
  

  emergencyStop();
*/
