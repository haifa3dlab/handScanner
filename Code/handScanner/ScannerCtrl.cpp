// Hand Scaneer library

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif

#include "ScannerCtrl.h"


/**
 * Description: class default c'tor, also instantianite  the AF_Stepper objects
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: 
 * @return:
 * 
 * Constants: CAMERA_CHANNEL, BASE_CHANNEL, CAMERA_LIMIT_SWITCH_PIN), BaseStop(BASE_LIMIT_SWITCH_PIN
 */
Scanner::Scanner() : CameraMotor(200, CAMERA_CHANNEL), BaseMotor(200, BASE_CHANNEL), CameraStop(CAMERA_LIMIT_SWITCH_PIN), BaseStop(BASE_LIMIT_SWITCH_PIN)
{
    Serial.println("Enter Scanner C'tor");
    mCameraPosition = 0;
    baseAngle = 0;
}


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
    Serial.println("Enters Scanner::init()");
    BaseMotor.setSpeed(20);
    CameraMotor.setSpeed(40);
    ResetCameraMotor();
    ResetBaseMotor();
}


/**
 * Description: Reset Camera motor position, function get called at initialization, motor moves back to limit-switched (end-stops)
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: 
 * @return: 
 * 
 * Constants: 
 */
void Scanner::ResetCameraMotor(){
    // turn motor until endstop reached
    // no int option:
    //  1. turn 1 step clockwise
    //  2. test if endstop pressed:
    //  3. if pressed break else continue
    // will be replaced with PIN read
    if ( DEBUG_SCANNER ) Serial.println("Scanner::ResetCameraMotor");
    int WatchDog = HEIGHT_MAX_STEPS;
    while ( !CameraStop.getState() ){
        CameraMotor.step(1, BACKWARD, MICROSTEP);
        if ( WatchDog-- <= 0 ) break;
    }
    return;
}


/**
 * Description: Reset Base motor's position, function get called at initialization, motor moves back to limit-switched (end-stops)
 *              each step the SW check if the input from END_STOP pin is high (polling)
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param:
 * @return: 
 * 
 * Constants: 
 */
void Scanner::ResetBaseMotor(){
    //  1. turn 1 step clockwise
    //  2. test if endstop pressed:
    //  3. if pressed break else continue
    // will be replaced with PIN read
     if ( DEBUG_SCANNER ) Serial.println("Scanner::ResetBaseMotor");
    int WatchDog = ANGLE_MAX_STEPS;
    while ( !BaseStop.getState() ){
        BaseMotor.step(1, BACKWARD, MICROSTEP);
        if ( WatchDog-- <= 0 ) break;
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
 * Constants: BASE_MAX_ANGLE, BASE_STEP_PER_DEGREE
 */
errType Scanner::baseTurn(int toDegree){
  // returns value for error log
  // 0 - ok
  // 1 - exceeds limits
  if ( DEBUG_SCANNER ) Serial.println("Scanner::baseTurn");
  if ( toDegree > BASE_MAX_ANGLE || toDegree < 0 )
  { 
    if ( DEBUG_SCANNER ) Serial.println("err_exceeds_limits"); 
    return err_exceeds_limits;
  }
  int turn_degree = toDegree - baseAngle;
  if ( turn_degree >= 0 ){
    steps = round(turn_degree  * BASE_STEP_PER_DEGREE);
    if ( DEBUG_SCANNER ) {Serial.print("Steps Forward: "); Serial.println(steps, DEC);}
    BaseMotor.step(steps, FORWARD, SINGLE);
  }
  else {
    steps = -round(turn_degree * BASE_STEP_PER_DEGREE);
    if ( DEBUG_SCANNER ) { Serial.print("Steps Backwarrd: "); Serial.println(steps, DEC);}
    BaseMotor.step(steps, BACKWARD, SINGLE);
  }
  baseAngle += turn_degree;
  if ( DEBUG_SCANNER ) { 
    Serial.print("Base Angle: "); 
    Serial.println(baseAngle, DEC);
  }
  return err_ok;
}


/**
 * Description: set Base motor speed in units of degree per sec
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: degrees_per_sec (set the base motor angular speed)
 * @return: 
 * 
 * Constants: BASE_STEP_PER_DEGREE
 */
void Scanner::setAngularSpeed(uint32_t degrees_per_sec){
    // no input check or error handling
    if ( DEBUG_SCANNER ) {
      Serial.println("Scanner::setAngularSpeed");
      Serial.print("Angular Speed in degress per second: ");
      Serial.println(degrees_per_sec, DEC);
    }
    BaseMotor.setSpeed( round(degrees_per_sec * BASE_STEP_PER_DEGREE) );
}


/**
 * Description: move the camera, control the Camera's motor
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: distance_mm (distance in mm, negative values to go up)
 * @return: error type
 * 
 * Constants: CAMERA_MAX_DIST, CAMERA_STEPS_PER_MM
 */
errType Scanner::cameraMove(int toPos){
  // returns value for error log
  // 0 - ok
  // 1 - exceeds limits
  if ( DEBUG_SCANNER ) Serial.println("Scanner::cameraMove");
  if ( toPos > CAMERA_MAX_DIST || toPos < 0 )
  { 
    if ( DEBUG_SCANNER ) Serial.println("ERROR: err_exceeds_limits");
    return err_exceeds_limits;
  }
  int distance_mm = toPos - mCameraPosition;
  if ( distance_mm >= 0 ){
    uint32_t steps = round(distance_mm * CAMERA_STEPS_PER_MM);
     if ( DEBUG_SCANNER ) {Serial.print("Steps Forward: "); Serial.println(steps, DEC);}
    CameraMotor.step(steps, FORWARD, SINGLE);
  }
  else {
    uint32_t steps = -round(distance_mm * CAMERA_STEPS_PER_MM);
     if ( DEBUG_SCANNER ) {Serial.print("Steps Backward: "); Serial.println(steps, DEC);}
    CameraMotor.step(steps, BACKWARD, SINGLE);
  }
  mCameraPosition += distance_mm;
  if ( DEBUG_SCANNER ) {
    Serial.print("Camera Position: ");
    Serial.println(mCameraPosition, DEC);
  }
  return err_ok;
}


/**
 * Description: set Camera motor speed in units of mm per sec
 * 
 * @Author Hagai Solodar (19/9/2016)
 * 
 * @param: mm_per_sec
 * @return: 
 * 
 * Constants: CAMERA_STEPS_PER_MM
 */
void Scanner::setHeightSpeed(uint32_t mm_per_sec){
    // no input check or error handling
    if ( DEBUG_SCANNER ) Serial.println("Scanner::setHeightSpeed");
    if ( DEBUG_SCANNER ) { Serial.print("Camera Travel Speed in mm per second: "); Serial.println(mm_per_sec, DEC);}
    CameraMotor.setSpeed( round(mm_per_sec * CAMERA_STEPS_PER_MM) );
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
uint8_t Scanner::doFullScan()
{
    if ( DEBUG_SCANNER )Serial.println("Scanner::doFullScan");

    int nextCameraPos = fullScanMinDist;
     // move to start
    cameraMove(nextCameraPos);
    baseTurn(0);

    for ( nextCameraPos += SCAN_BAND_HEIGHT_MM;
          nextCameraPos <= fullScanMaxDist;
          nextCameraPos += SCAN_BAND_HEIGHT_MM)
    {
      if ( baseTurn( (baseAngle < (fullScanMaxDeg + fullScanMinDeg)/2) ? fullScanMaxDeg : fullScanMinDeg) != 0) 
        return err_fullscan_base_turn; //error return value
      if ( cameraMove(nextCameraPos) != 0)
        return err_fullscan_camera_move; //error return value
    }
    releaseMotors(); // don't heat the steppers if not needed.
    return err_ok;
}


/**
 * Description: "Release" motor for power consuption and heat generation saving
 * 
 * @Author: Hagai Solodar (21/09/2016)
 * 
 * @param:
 * @return: 
 * 
 */
void Scanner::releaseMotors()
{
  if ( DEBUG_SCANNER )Serial.println("Scanner::releaseMotors");
  CameraMotor.release();
  BaseMotor.release();
}

/**
 * Description: returns the base covered angle in degree
 * 
 * @Author Hagai Solodar (21/9/2016)
 * 
 * @param: 
 * @return: base angle in degree 
 */
uint32_t Scanner::getBaseAngle(){
  return baseAngle;
}
 /**
 * Description: return the camera's position in mm (distance covered form end-stop)
 * 
 * @Author Hagai Solodar (21/9/2016)
 * 
 * @param: 
 * @return: cameraPosition 
 */
uint32_t Scanner::getCameraPosition(){
  return mCameraPosition;
}


 /**
 * Description: ISR - Interrupt Seervice Routine   
 *              method is "attahched" to certain interrupts pin (2 or 3 for UNO) and called with "button" to halt
 *              all execution
 *              
 * @Author Hagai Solodar (21/9/2016)
 * 
 * @param: ISR do not have params 
 * @return: and do not returns any values
 */
void Scanner::emergencyStop()
{
  releaseMotors();
  // Note: Maybe we need totally disconnect
  //   the Adafruit board from electric power.
}

