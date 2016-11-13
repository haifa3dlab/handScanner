#include <SoftwareSerial.h>  
#include "ScannerCtrl.h"
#include "Parser.h"

int bluetoothTx =  9;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 10;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

Scanner scanner;
Parser parser;
int cmdDelay = 20;
int init_flag = 1;

void setupBT() {
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
}

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hand Scanner Start!");

  setupBT();
}

void loop()
{
  uint32_t value = NO_VALUE;  // NO_VALUE defined in Parser.h
  //Serial.println("New Loop: ");
  if ( (Serial.available() && parser.Listen(Serial)) ||
       (bluetooth.available() && parser.Listen(bluetooth)) )
  {
    value = parser.callCommand(scanner);
    
    if ( value != NO_VALUE ) {
      Serial.print("Command return value: ");
      Serial.println(value);
    }
  }
  
  delay(cmdDelay);
}


/*
void basicTest()
{
  if (init_flag){
    Serial.println("Reseting Motor First");
    scanner.init();
    init_flag = 0;
  }

  // set motor's speed
  scanner.setHeightSpeed(10);
  scanner.setAngularSpeed(10);
  
  // turn base
  delay(cmdDelay);
  scanner.baseTurn(180);
  delay(cmdDelay);
  if ( scanner.getBaseAngle() > 200 ){
    scanner.baseTurn(-180);
  }

  // release motor to reduce heating (note that if you release the camera motor's the camera might "fall")
  scanner.releaseMotors(); // They are getting hot otherwise!!!

  // move camera motor
  delay(cmdDelay);
  if (  scanner.cameraMove(60) == err_exceeds_limits){
    Serial.println("Error - exceeds motor limits!");
  }
  delay(cmdDelay*3);

  // set different speed 
  scanner.setHeightSpeed(30);
  scanner.setAngularSpeed(30);

  // do another base turn
  delay(cmdDelay);
  scanner.baseTurn(90);
  delay(cmdDelay);
  if ( scanner.getBaseAngle() > 200 ){
    scanner.baseTurn(-90);
  }

  // release motors
  scanner.releaseMotors(); // They are getting hot otherwise!!!

  // move camera motor
  delay(cmdDelay);
  if (  scanner.cameraMove(60) == err_exceeds_limits){
    Serial.println("Error - exceeds motor limits!");
  }
  delay(cmdDelay*3);
}
*/
