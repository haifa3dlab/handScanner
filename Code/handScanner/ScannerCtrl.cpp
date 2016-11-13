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
 * Constants: CAMERA_CHANNEL, BASE_CHANNEL, CAMERA_LIMIT_SWITCH_PIN, BASE_LIMIT_SWITCH_PIN
 */
Scanner::Scanner() : 
  CameraMotor(STEP_PER_REV, CAMERA_CHANNEL),
  BaseMotor(200, BASE_CHANNEL),
  CameraStop(CAMERA_LIMIT_SWITCH_PIN),
  BaseStop(BASE_LIMIT_SWITCH_PIN)
{
    pcomm->println(F("Enter Scanner C'tor"));
    mCameraPosition = 0;
    mBaseAngle = 0;
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
    pcomm->println(F("Scanner::init()"));

    angularSpeed = round(15/*deg/sec*/ * BASE_STEP_PER_DEGREE);
    BaseMotor.setSpeed( round(stepsPerSecToRPM(angularSpeed)) );
    
    heightSpeed = round(50/*mm/sec*/ * CAMERA_STEPS_PER_MM);
    CameraMotor.setSpeed( round(stepsPerSecToRPM(heightSpeed)) );
    
    ResetCameraMotor();
    ResetBaseMotor();
}


/**
 * Description: Reset Camera motor position, function get called at initialization, motor moves back to limit-switched (end-stops)
 * 
 * @Author Hagai Solodar (19/9/2016), Benny Godlin (27/9/2016)
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
    if ( DEBUG_SCANNER ) pcomm->println(F("Scanner::ResetCameraMotor"));
    int WatchDog = HEIGHT_MAX_STEPS;
    while ( !CameraStop.getState() ){
        CameraMotor.step(1, BACKWARD, MICROSTEP);
        if ( WatchDog-- <= 0 ) break;
    }
    mCameraPosition = 0;
    return;
}


/**
 * Description: Reset Base motor's position, function get called at initialization, motor moves back to limit-switched (end-stops)
 *              each step the SW check if the input from END_STOP pin is high (polling)
 * 
 * @Author Hagai Solodar (19/9/2016), Benny Godlin (27/9/2016)
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
     if ( DEBUG_SCANNER ) pcomm->println(F("Scanner::ResetBaseMotor"));
    int WatchDog = ANGLE_MAX_STEPS;
    while ( !BaseStop.getState() ){
        BaseMotor.step(1, BACKWARD, MICROSTEP);
        if ( WatchDog-- <= 0 ) break;
    }
    mBaseAngle = 0;
    return;
}

/**
 * Description: turn the base, control the Base's motor, include protection from illegal moves
 * 
 * @Author Hagai Solodar (19/9/2016), Benny Godlin (27/9/2016)
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
  if ( DEBUG_SCANNER ) pcomm->println(F("Scanner::baseTurn"));
  if ( toDegree > BASE_MAX_ANGLE || toDegree < 0 )
  { 
    if ( DEBUG_SCANNER ) pcomm->println(F("err_exceeds_limits")); 
    return err_exceeds_limits;
  }
  int turn_degree = toDegree - mBaseAngle;
  bool forward = ( turn_degree >= 0 );
  int steps = round(abs(turn_degree) * BASE_STEP_PER_DEGREE);
  if ( DEBUG_SCANNER ) {
    pcomm->print(forward ? F("Steps Forward: ") : F("Steps Backwarrd: "));
    pcomm->println(steps, DEC);
  }

  if (steps == 0) {
      if ( DEBUG_SCANNER ) { pcomm->println("No need to move (steps == 0) !"); }
      //return err_ok;
  }
  else {
    BaseMotor.setSpeed( round(stepsPerSecToRPM(angularSpeed)) );
    int fullSpeed = angularSpeed; // (steps/sec) may be updated by the speedUp call to slowStartStop()
    int speedUpSteps = slowStartStop(BaseMotor, /*inout*/fullSpeed, maxAccelStepsPerSqSec, steps/2, forward, /*speedUp=*/true);
    
    int fullSpeedSteps = steps - 2*speedUpSteps;
    if (fullSpeedSteps > 0) {
      if (fullSpeed < angularSpeed)
        BaseMotor.setSpeed( round(stepsPerSecToRPM(fullSpeed)) );
      BaseMotor.step(fullSpeedSteps, (forward ? FORWARD : BACKWARD), SINGLE);
      //BaseMotor.step(fullSpeedSteps * MICROSTEPS, (forward ? FORWARD : BACKWARD), MICROSTEP);
    } else {
      pcomm->print(F("Acceleration and de-acceleration takes to much, remaining steps: "));
      pcomm->println(speedUpSteps, DEC);
      pcomm->println(F("Base position may be wrong !"));
    }
    int slowDownSteps = slowStartStop(BaseMotor, fullSpeed, maxAccelStepsPerSqSec, speedUpSteps, forward, /*speedUp=*/false);
  }
  
  mBaseAngle += turn_degree;
  if ( DEBUG_SCANNER ) { 
    pcomm->print(F("Base Angle: ")); 
    pcomm->println(mBaseAngle, DEC);
  }
  return err_ok;
}


/**
 * Description: turn the base relative to current position uses baseTurn() to execute
 *
 * @Author Benny Godlin (27/10/2016)
 *
 * @param: diffDegree (how many degrees to turn relative to current position)
 * @return: error type
 * 
 * Constants:
 */
errType Scanner::baseTurnRel(int diffDegree){
  int newAngle = mBaseAngle + diffDegree;
  if (newAngle < 0) newAngle = 0;
  else if (newAngle > BASE_MAX_ANGLE) newAngle = BASE_MAX_ANGLE;
  return baseTurn(newAngle);
}

/**
 * Description: set Base motor speed in units of degree per sec
 * 
 * @Author Hagai Solodar (19/9/2016), Benny Godlin (27/9/2016)
 * 
 * @param: degrees_per_sec (set the base motor angular speed)
 * @return: 
 * 
 * Constants: BASE_STEP_PER_DEGREE
 */
void Scanner::setAngularSpeed(int degrees_per_sec){
    // no input check or error handling
    if ( DEBUG_SCANNER ) {
      pcomm->println(F("Scanner::setAngularSpeed"));
      pcomm->print(F("Angular Speed in degress per second: "));
      pcomm->println(degrees_per_sec, DEC);
    }
    angularSpeed = round(degrees_per_sec * BASE_STEP_PER_DEGREE);
    BaseMotor.setSpeed( round(stepsPerSecToRPM(angularSpeed)) ); // RPM !
}


/**
 * Description: move the camera, control the Camera's motor
 * 
 * @Author Hagai Solodar (19/9/2016), Benny Godlin (27/9/2016)
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
  if ( DEBUG_SCANNER ) { pcomm->println("Scanner::cameraMove"); }
  if ( toPos > CAMERA_MAX_DIST || toPos < 0 )
  { 
    if ( DEBUG_SCANNER ) pcomm->println(F("*ERROR: err_exceeds_limits*"));
    return err_exceeds_limits;
  }
  int distance_mm = toPos - mCameraPosition;
  if ( distance_mm >= 0 ){
    int steps = round(distance_mm * CAMERA_STEPS_PER_MM);
    if ( DEBUG_SCANNER ) {pcomm->print(F("Steps Forward: ")); pcomm->println(steps, DEC);}
    CameraMotor.step(steps, FORWARD, SINGLE);
  }
  else {
    int steps = -round(distance_mm * CAMERA_STEPS_PER_MM);
    if ( DEBUG_SCANNER ) {pcomm->print(F("Steps Backward: ")); pcomm->println(steps, DEC);}
    CameraMotor.step(steps, BACKWARD, SINGLE);
  }
  mCameraPosition += distance_mm;
  if ( DEBUG_SCANNER ) {
    pcomm->print(F("Camera Position: "));
    pcomm->println(mCameraPosition, DEC);
  }
  CameraMotor.release();
  return err_ok;
}


/**
 * Description: move the camera relative to current position, uses cameraMove() to execute
 *
 * @Author Benny Godlin (27/10/2016)
 *
 * @param: diff_mm (how many mm to move relative to current position)
 * @return: error type
 *
 * Constants:
 */
errType Scanner::cameraMoveRel(int diff_mm){
  int newPos = mCameraPosition + diff_mm;
  if (newPos < 0) newPos = 0;
  else if (newPos > CAMERA_MAX_DIST) newPos = CAMERA_MAX_DIST;
  return cameraMove(newPos);
}

/**
 * Description: set Camera motor speed in units of mm per sec
 * 
 * @Author Hagai Solodar (19/9/2016), Benny Godlin (27/9/2016)
 * 
 * @param: mm_per_sec
 * @return: 
 * 
 * Constants: CAMERA_STEPS_PER_MM
 */
void Scanner::setHeightSpeed(int mm_per_sec){
    // no input check or error handling
    if ( DEBUG_SCANNER ) { pcomm->println(F("Scanner::setHeightSpeed")); }
    if ( DEBUG_SCANNER ) { pcomm->print(F("Camera Travel Speed in mm per second: ")); pcomm->println(mm_per_sec, DEC);}
    heightSpeed = round(mm_per_sec * CAMERA_STEPS_PER_MM);
    CameraMotor.setSpeed( round(stepsPerSecToRPM(heightSpeed)) );  // RPM !
}


#define MAXLONG (2147483647L)

static long diffMicros(long endMicros, long startMicros)
{
  long diff = endMicros - startMicros;
  if (diff < 0) diff += MAXLONG;
  return diff;
}

/**
 * Description: speed-up or slow-down to certain speed (steps/sec)
 *              at certain acceleration (steps/(sec^2))
 *
 * @Author Benny Godlin (25/10/2016)
 *
 * @param: motor (the motor to spped-up or slow down)
 *         inout_fullSpeed (the full speed to which accelerate or deccelerate in  steps/sec)
 *               the method also returns in inout_fullSpeed its last speed if we didn't reach the expected full-speed
 *         accel (max acceleration in steps/(sec^2), to bound force applied to mechanics)
 *         speedUp (if true we speedup to fullSpeed, otherwise we slow-down from fullSpeed to stop)
 *         forward (if true the motor should step forward, otherwise step backword)
 *
 * @return: number of steps performed during (de)acceleration
 *
 * Constants: STEPS_PER_ACCEL_ROUND
 */
int Scanner::slowStartStop(AF_Stepper& motor, int& inout_fullSpeed, float accelStepsPerSqSec, long maxSteps, bool forward/*=true*/, bool speedUp/*=true*/)
{
  if ( DEBUG_SCANNER ) { pcomm->println("sSS"/*"Scanner::slowStartStop"*/); }
  long curr_us, prev_us;
  long diff_us, expected_us, remains_us = 0;
  float cumul_us = 0.0F;
  long startSpeed, trgSpeed, currSpeed, diffSpeed;
  long stepCnt = 0;

  accelStepsPerSqSec *= MICROSTEPS;
  maxSteps *= MICROSTEPS;

  if (speedUp) {
    startSpeed = MIN_STEPS_PER_SEC; // first faze - min possible speed
    trgSpeed = inout_fullSpeed * MICROSTEPS;
  } else {
    startSpeed = inout_fullSpeed * MICROSTEPS;
    trgSpeed = 0;
  }
/*
  if (DEBUG_SCANNER) {
    pcomm->print(speedUp ? F("Accelerate ") : F("De-accelerate"));
    pcomm->print(F("from startSpeed "));
    pcomm->print(startSpeed);
    pcomm->print(F(" (steps/sec) to trgSpeed "));
    pcomm->print(trgSpeed);
    pcomm->print(F(" (steps/sec) to at accel "));
    pcomm->print(accelStepsPerSqSec, 2);
    pcomm->print(F(" (steps/sec^2) in maxSteps "));
    pcomm->println(maxSteps);    
  }
*/
  currSpeed = startSpeed;
  curr_us = micros();
  while( (speedUp ? currSpeed < trgSpeed : currSpeed > trgSpeed) &&
         stepCnt < maxSteps )
  {
    expected_us = 1000000L / currSpeed;

    motor.onestep((forward ? FORWARD : BACKWARD), MICROSTEP);
    ++stepCnt;
/*
if (DEBUG_SCANNER) {
  pcomm->print("remains_us = ");
  pcomm->print(remains_us);
  pcomm->print(", expected_us = ");
  pcomm->print(expected_us);
  pcomm->print(", cumul_us = ");
  pcomm->print(cumul_us, 0);
  pcomm->print(", currSpeed = ");
  pcomm->println(currSpeed);
}
*/
    prev_us = curr_us;
    curr_us = micros();
    diff_us = diffMicros(curr_us, prev_us);

    remains_us = expected_us - diff_us;  // last step may be:(diff_us / (STEPS_PER_ACCEL_ROUND - 1));
    if (remains_us > 0L)
      delayMicroseconds(remains_us);

    // calc new currSpeed
    cumul_us += expected_us;
    diffSpeed = round(cumul_us * accelStepsPerSqSec / 1000000.0F);

    if (speedUp) {
      currSpeed = diffSpeed;
      if (currSpeed > trgSpeed)
        currSpeed = trgSpeed;
    } else {
      currSpeed = startSpeed - diffSpeed;
      if (currSpeed < 0)
        break;
    }
  }

  // Don't stay in MICROSTEP mode after stop (speed-down):
  if ( !speedUp ) {
    motor.release();
    // or:
    //motor.onestep((forward ? FORWARD : BACKWARD), SINGLE);
    // or:
    //motor.setSpeed( round(stepsPerSecToRPM(MIN_STEPS_PER_SEC)) ); // set minimal speed
    //motor.step( (stepCnt % MICROSTEPS), (forward ? FORWARD : BACKWARD), MICROSTEP); // finish last full step
  }

  // If we didn't reach fullSpeed in allowed number of steps,
  //   return the speed we reached in fullSpeed var: 
  if (speedUp && currSpeed < trgSpeed) {
    inout_fullSpeed = currSpeed / MICROSTEPS;
    /*
    if (DEBUG_SCANNER) {
      pcomm->print(F("Reached only speed of "));
      pcomm->print(currSpeed);
      pcomm->println(F("(stepa/sec)"));
    }
    */
  }
/*
  if (DEBUG_SCANNER) {
      pcomm->print(F("slowStartStop did use "));
      pcomm->print(stepCnt);
      pcomm->println(F(" steps."));
  }
*/
  return stepCnt / MICROSTEPS;
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
 * @Author: Hagai Solodar (19/09/2016), Benny Godlin (27/9/2016)
 * 
 * @param:
 * @return: error type
 * 
 */
errType Scanner::doFullScan()
{
    if ( DEBUG_SCANNER )pcomm->println("Scanner::doFullScan");

    int nextCameraPos = fullScanMinDist;
     // move to start
    cameraMove(nextCameraPos);
    baseTurn(0);

    for ( nextCameraPos += SCAN_BAND_HEIGHT_MM;
          nextCameraPos <= fullScanMaxDist;
          nextCameraPos += SCAN_BAND_HEIGHT_MM)
    {
      if ( baseTurn( (mBaseAngle < (fullScanMaxDeg + fullScanMinDeg)/2) ? fullScanMaxDeg : fullScanMinDeg) != err_ok)
        return err_fullscan_base_turn; //error return value
      if ( cameraMove(nextCameraPos) != err_ok)
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
  if ( DEBUG_SCANNER ) pcomm->println(F("Scanner::releaseMotors"));
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
int Scanner::getBaseAngle(){
  return mBaseAngle;
}
 /**
 * Description: return the camera's position in mm (distance covered form end-stop)
 * 
 * @Author Hagai Solodar (21/9/2016)
 * 
 * @param: 
 * @return: cameraPosition 
 */
int Scanner::getCameraPosition(){
  return mCameraPosition;
}


 /**
 * Description: "Release" motors so the scanner mechanics doesn't apply any presure or resistance
 *              to moving by hand.
 *
 *              may be called by ISR - Interrupt Seervice Routine
 *              ("attahched" to certain interrupts pin (2 or 3 for UNO) and called with "button" to halt
 *              all execution)
 *              
 * @Author Hagai Solodar (21/9/2016)
 * 
 * @param:
 * @return:
 */
void Scanner::emergencyStop()
{
  releaseMotors();
  // Note: Maybe we need to totally disconnect
  //   the Adafruit board from electric power.
}

/**
 * Translate steps_per_sec to RPM of the steper shaft. 
 * 
 * @Author Benny Godlin (10/11/2016)
 * 
 * @param:
 * @return:
 */
static float Scanner::stepsPerSecToRPM(int steps_per_sec) 
{
  return (float)steps_per_sec * SEC_PER_MIN / STEP_PER_REV;
}

