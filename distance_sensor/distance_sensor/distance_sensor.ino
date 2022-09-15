#include <M5Atom.h>

#define DISTANCE_SENSOR_TRIG_PIN 22
#define DISTANCE_SENSOR_ECHO_PIN 19

int distance_sensor_time_val;
float distance_sensor_val;

void setup()
{
    M5.begin(true, false, true);
    M5.dis.fillpix(0x00ff00);

    pinMode(DISTANCE_SENSOR_TRIG_PIN, OUTPUT);
    pinMode(DISTANCE_SENSOR_ECHO_PIN, INPUT);

    Serial.begin(9600);
}
void loop()
{
    digitalWrite(DISTANCE_SENSOR_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(DISTANCE_SENSOR_TRIG_PIN, LOW);
    distance_sensor_time_val = pulseIn(DISTANCE_SENSOR_ECHO_PIN, HIGH);
    distance_sensor_val = (float(distance_sensor_time_val) / 2) / 29.1;

    // /*Serial.print("distance_sensor_time_val:");*/ Serial.print(distance_sensor_time_val); Serial.print(",");
    /*Serial.print("distance_sensor_val:");*/ Serial.print(distance_sensor_val); Serial.print("\r\n");

    delay(500);
}
