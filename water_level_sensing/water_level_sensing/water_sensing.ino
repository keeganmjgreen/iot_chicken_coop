#include <M5Atom.h>

#define DISTANCE_SENSOR_TRIG_PIN 19
#define DISTANCE_SENSOR_ECHO_PIN 23
#define PRESSURE_SENSOR_ANALOG_PIN 33
#define LOWER_FILL_SENSOR_DIGITAL_PIN 25
#define UPPER_FILL_SENSOR_ANALOG_PIN 32
#define UPPER_FILL_SENSOR_DIGITAL_PIN 21

int pressure_sensor_analog_val;
bool lower_fill_sensor_digital_val;
int upper_fill_sensor_analog_val;
bool upper_fill_sensor_digital_val;
int distance_sensor_time_val;
// float distance_sensor_val;

void setup()
{
    M5.begin(true, false, true);
    M5.dis.fillpix(0x00ff00);

    pinMode(DISTANCE_SENSOR_TRIG_PIN, OUTPUT);
    pinMode(DISTANCE_SENSOR_ECHO_PIN, INPUT);
    pinMode(PRESSURE_SENSOR_ANALOG_PIN, INPUT);
    pinMode(LOWER_FILL_SENSOR_DIGITAL_PIN, INPUT);
    pinMode(UPPER_FILL_SENSOR_ANALOG_PIN, INPUT);
    pinMode(UPPER_FILL_SENSOR_DIGITAL_PIN, INPUT);

    Serial.begin(9600);

    Serial.print("distance_sensor_time_val,pressure_sensor_analog_val,lower_fill_sensor_digital_val,upper_fill_sensor_analog_val,upper_fill_sensor_digital_val\r\n");
    // Serial.print("distance_sensor_val,pressure_sensor_analog_val,lower_fill_sensor_digital_val,upper_fill_sensor_analog_val,upper_fill_sensor_digital_val\r\n");
}
void loop()
{
    digitalWrite(DISTANCE_SENSOR_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(DISTANCE_SENSOR_TRIG_PIN, LOW);
    distance_sensor_time_val = pulseIn(DISTANCE_SENSOR_ECHO_PIN, HIGH);
    // distance_sensor_val = (float(distance_sensor_time_val) / 2) / 29.1;
    pressure_sensor_analog_val = analogRead(PRESSURE_SENSOR_ANALOG_PIN);
    lower_fill_sensor_digital_val = !digitalRead(LOWER_FILL_SENSOR_DIGITAL_PIN);
    upper_fill_sensor_analog_val = analogRead(UPPER_FILL_SENSOR_ANALOG_PIN);
    upper_fill_sensor_digital_val = digitalRead(UPPER_FILL_SENSOR_DIGITAL_PIN);

    /*Serial.print("distance_sensor_time_val:");*/ Serial.print(distance_sensor_time_val); Serial.print(",");
    // /* Serial.print("distance_sensor_val:");*/ Serial.print(distance_sensor_val); Serial.print(",");
    /*Serial.print("pressure_sensor_analog_val:");*/ Serial.print(pressure_sensor_analog_val); Serial.print(",");
    /*Serial.print("lower_fill_sensor_digital_val:");*/ Serial.print(lower_fill_sensor_digital_val); Serial.print(",");
    /*Serial.print("upper_fill_sensor_analog_val:");*/ Serial.print(upper_fill_sensor_analog_val); Serial.print(",");
    /*Serial.print("upper_fill_sensor_digital_val:");*/ Serial.print(upper_fill_sensor_digital_val); Serial.print("\r\n");

    delay(500);
}
