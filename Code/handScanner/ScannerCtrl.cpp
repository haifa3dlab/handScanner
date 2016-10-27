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
Scanner::Scanner() : CameraMotor(200, CAMERA_CHANNEL), BaseMotor(200, BASE_CHANNEL), CameraStop(CAMERA_LIMIT_SWITCH_PIN), BaseStop(BASE_LIMIT_SWITCH_PIN)
{
    Serial.println("Enter Scanner C'tor");
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
    Serial.println("Enters Scanner::init()");
    BaseMotor.setSpeed(20);
    CameraMotor.setSpeed(40);
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
    if ( DEBUG_SCANNER ) Serial.println("Scanner::ResetCameraMotor");
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
     if ( DEBUG_SCANNER ) Serial.println("Scanner::ResetBaseMotor");
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
  if ( DEBUG_SCANNER ) Serial.println("Scanner::baseTurn");
  if ( toDegree > BASE_MAX_ANGLE || toDegree < 0 )
  { 
    if ( DEBUG_SCANNER ) Serial.println("err_exceeds_limits"); 
    return err_exceeds_limits;
  }
  int turn_degree = toDegree - mBaseAngle;
  bool forward = ( turn_degree >= 0 );
  int steps = round((forward ? turn_degree : -turn_degree) * BASE_STEP_PER_DEGREE);
  if ( DEBUG_SCANNER ) {
    Serial.print(forward ? "Steps Forward: " : "Steps Backwarrd: ");
    Serial.println(steps, DEC);
  }

  int speedUpSteps = slowStartStop(BaseMotor, angularSpeed, maxAccelBaseSteps, forward, /*speedUp=*/true);
  int fullSpeedSteps = steps - 2*speedUpSteps;
  if (fullSpeedSteps > 0) {
    BaseMotor.step(fullSpeedSteps, (forward ? FORWARD : BACKWARD), SINGLE);
  } else {
    // BUG: accel and (de)accel may use more then steps needed -
    //   and the base ends at different position then ordered !!!                <<=== FIX
    Serial.print("Acceleration and de-acceleration takes to much steps: ");
    Serial.println(speedUpSteps, DEC);
    Serial.println("Base position may be wrong !");
  }
  int slowDownSteps = slowStartStop(BaseMotor, angularSpeed, maxAccelBaseSteps, forward, /*speedUp=*/false);

  mBaseAngle += turn_degree;
  if ( DEBUG_SCANNER ) { 
    Serial.print("Base Angle: "); 
    Serial.println(mBaseAngle, DEC);
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
      Serial.println("Scanner::setAngularSpeed");
      Serial.print("Angular Speed in degress per second: ");
      Serial.println(degrees_per_sec, DEC);
    }
    angularSpeed = round(degrees_per_sec * BASE_STEP_PER_DEGREE);
    BaseMotor.setSpeed( angularSpeed );
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
  if ( DEBUG_SCANNER ) Serial.println("Scanner::cameraMove");
  if ( toPos > CAMERA_MAX_DIST || toPos < 0 )
  { 
    if ( DEBUG_SCANNER ) Serial.println("*ERROR: err_exceeds_limits*");
    return err_exceeds_limits;
  }
  int distance_mm = toPos - mCameraPosition;
  if ( distance_mm >= 0 ){
    int steps = round(distance_mm * CAMERA_STEPS_PER_MM);
    if ( DEBUG_SCANNER ) {Serial.print("Steps Forward: "); Serial.println(steps, DEC);}
    CameraMotor.step(steps, FORWARD, SINGLE);
  }
  else {
    int steps = -round(distance_mm * CAMERA_STEPS_PER_MM);
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
    if ( DEBUG_SCANNER ) Serial.println("Scanner::setHeightSpeed");
    if ( DEBUG_SCANNER ) { Serial.print("Camera Travel Speed in mm per second: "); Serial.println(mm_per_sec, DEC);}
    heightSpeed = round(mm_per_sec * CAMERA_STEPS_PER_MM);
    CameraMotor.setSpeed( heightSpeed );
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
 *         fullSpeed (the full speed to which accelerate or deccelerate in  steps/sec)
 *         accel (max acceleration in steps/(sec^2), to bound force applied to mechanics)
 *         speedUp (if true we speedup to fullSpeed, otherwise we slow-down from fullSpeed to stop)
 *         forward (if true the motor should step forward, otherwise step backword)
 *
 * @return: number of steps performed during (de)acceleration
 *
 * Constants: STEPS_PER_ACCEL_ROUND
 */
int Scanner::slowStartStop(AF_Stepper& motor, int fullSpeed, float accel, bool forward/*=true*/, bool speedUp/*=true*/)
{
  long startMicros, currMicros, prevMicros, diff, lastStepMicros;
  int startSpeed, trgSpeed, currSpeed, diffSpeed;
  int stepCnt = 0;

  if (speedUp) {
    startSpeed = STEPS_PER_ACCEL_ROUND / 0.2; // first faze - very slow for 200ms
    trgSpeed = fullSpeed;
  } else {
    startSpeed = fullSpeed;
    trgSpeed = 0;
  }

  for (currSpeed = startSpeed, startMicros = micros();
       (speedUp ? currSpeed <= trgSpeed : currSpeed > trgSpeed);
       /*in body*/)
  {
    motor.setSpeed( currSpeed );
    motor.step(STEPS_PER_ACCEL_ROUND, (forward ? FORWARD : BACKWARD), SINGLE);
    prevMicros = currMicros;
    currMicros = micros();
    diff = diffMicros(currMicros, prevMicros);
    stepCnt += STEPS_PER_ACCEL_ROUND;

    // Compensate for the last step time -
    //   motor.step() returns immediatelly after
    //   the motor reaches the position after STEPS_PER_ACCEL_ROUND
    //   but we should wait the step time according to current speed.
    lastStepMicros = (diff / (STEPS_PER_ACCEL_ROUND - 1));
    delayMicroseconds(lastStepMicros);

    // calc new currSpeed
    diff = diffMicros(currMicros, startMicros);
    diffSpeed = round((diff + lastStepMicros) * accel / 1000000.0F);

    if (speedUp) {
      currSpeed = diffSpeed;
      if (diffSpeed > fullSpeed)
        diffSpeed > fullSpeed;
    } else {
      currSpeed = fullSpeed - diffSpeed;
      if (currSpeed < 0)
        break;
    }
  }

  return stepCnt;
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
 * @param: motor
 * @return: error type
 * 
 */
errType Scanner::doFullScan()
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
      if ( baseTurn( (mBaseAngle < (fullScanMaxDeg + fullScanMinDeg)/2) ? fullScanMaxDeg : fullScanMinDeg) != 0)
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

