#include "arduino_secrets.h"

// M5Atom - Version: Latest
#include <M5Atom.h>

/* 
  Sketch generated by the Arduino IoT Cloud Thing "Arduino_Cloud_M5_SEN0503_test"
  https://create.arduino.cc/cloud/things/ef416d6b-86f9-4377-9f9f-325e8f0a8ba2

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  bool SEN0503_state;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  M5.begin(true, false, true);
  M5.dis.fillpix(0x00ff00);

  pinMode(22, INPUT);
}

void loop() {
  ArduinoCloud.update();
  // Your code here

  SEN0503_state = digitalRead(22);

//delay(1000);
}