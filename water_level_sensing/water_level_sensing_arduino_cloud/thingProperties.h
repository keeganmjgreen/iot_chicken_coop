// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "82481f03-3c71-49b4-98bc-a0424ef3dc80";

const char SSID[]        = SECRET_SSID;          // Network SSID (name)
const char PASS[]        = SECRET_PASS;          // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onM5LedStateChange();

bool m5_button_state;
bool m5_led_state;
CloudTime distance_sensor_time_val;
CloudLength distance_sensor_val;
CloudElectricPotential pressure_sensor_analog_val;
int lower_fill_sensor_digital_val;
CloudElectricPotential upper_fill_sensor_analog_val;
int upper_fill_sensor_digital_val;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(m5_button_state, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(m5_led_state, READWRITE, ON_CHANGE, onM5LedStateChange);
  // ArduinoCloud.addProperty(distance_sensor_time_val, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(distance_sensor_val, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(pressure_sensor_analog_val, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(lower_fill_sensor_digital_val, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(upper_fill_sensor_analog_val, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(upper_fill_sensor_digital_val, READ, 1 * SECONDS, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
