#define ESC_PIN 25
#define MIN_PULSE 1000 // microseconds
#define MAX_PULSE 2000 // microseconds
#define ARM_PULSE  500 // microseconds
#define RUNDOWN_TIME_MS 2000

#include "ESC.h"
ESC my_esc(ESC_PIN, MIN_PULSE, MAX_PULSE, ARM_PULSE);

#include <multi_channel_relay.h>
Multi_Channel_Relay relays;

float currently_set_speed = 0;
// Note: The *currently set* speed is not necessarily the *current* speed.

bool still_setting_speed = false;
unsigned long rundown_start_time_ms = 0;
float newly_set_speed;
const float LO_SPEED = 0.6;
const float HI_SPEED = 1.0;
void set_speed(float _newly_set_speed);
void continue_setting_speed();

#define close_button_PIN 23
#define  open_button_PIN 33

bool close_button_old_state;
bool  open_button_old_state;

bool close_button_new_state;
bool  open_button_new_state;

bool close_button_rising_edge;
bool  open_button_rising_edge;

bool close_button_falling_edge;
bool  open_button_falling_edge;

#define ENC_PIN_A 22 // Pin for Encoder Channel A.
#define ENC_PIN_B 19 // Pin for Encoder Channel B.

bool enc_calibrated = false;
const float ANGLE_THRES_DEG = 5;

bool old_enc_state_A; // Old Encoder Channel A signal.
bool old_enc_state_B; // Old Encoder Channel B signal.

bool new_enc_state_A; // New Encoder Channel A signal.
bool new_enc_state_B; // New Encoder Channel B signal.

const float ANGLE_MIN_DEG =  0;
const float ANGLE_MAX_DEG = 79;

float angle_deg; // Current door angle (relative - incremental encoder) (0.5-degree resolution).

float start_angle_deg;

void setup()
{
    relays.channelCtrl(0); // Turns off all channels.

    Serial.begin(9600);

    my_esc.arm();

    pinMode(close_button_PIN, INPUT_PULLUP);
    pinMode( open_button_PIN, INPUT_PULLUP);

    close_button_new_state = !digitalRead(close_button_PIN);
     open_button_new_state = !digitalRead( open_button_PIN);

    pinMode(ENC_PIN_A, INPUT_PULLUP);
    pinMode(ENC_PIN_B, INPUT_PULLUP);
}
void loop()
{
    if (Serial.available() > 0)
    {
        set_speed(Serial.parseFloat());
    }

    close_button_old_state = close_button_new_state;
     open_button_old_state =  open_button_new_state;

    close_button_new_state = !digitalRead(close_button_PIN);
     open_button_new_state = !digitalRead( open_button_PIN);

    close_button_rising_edge = !close_button_old_state & close_button_new_state;
     open_button_rising_edge = ! open_button_old_state &  open_button_new_state;

    close_button_falling_edge = close_button_old_state & !close_button_new_state;
     open_button_falling_edge =  open_button_old_state & ! open_button_new_state;

    if ((currently_set_speed > 0) & (close_button_rising_edge | open_button_falling_edge))
    {
        if (close_button_rising_edge)
        {
            if (!enc_calibrated | (enc_calibrated & (ANGLE_MAX_DEG - angle_deg < ANGLE_THRES_DEG)))
            {
                new_enc_state_A = digitalRead(ENC_PIN_A);
                new_enc_state_B = digitalRead(ENC_PIN_B);
                angle_deg = ANGLE_MAX_DEG;
                enc_calibrated = true;
            }
        }
        set_speed(0);
    }
    else if ((currently_set_speed < 0) & (open_button_rising_edge | close_button_falling_edge))
    {
        if (open_button_rising_edge)
        {
            if (!enc_calibrated | (enc_calibrated & (angle_deg - ANGLE_MIN_DEG < ANGLE_THRES_DEG)))
            {
                new_enc_state_A = digitalRead(ENC_PIN_A);
                new_enc_state_B = digitalRead(ENC_PIN_B);
                angle_deg = ANGLE_MIN_DEG;
                enc_calibrated = true;
            }
        }
        set_speed(0);
    }
    else if (currently_set_speed == 0)
    {
        if (!open_button_new_state & close_button_falling_edge)
        {
            set_speed(-LO_SPEED);
            start_angle_deg = angle_deg;
        }
        else if (!close_button_new_state & open_button_falling_edge)
        {
            set_speed(+LO_SPEED);
            start_angle_deg = angle_deg;
        }
    }
    if (still_setting_speed & ((millis() - rundown_start_time_ms) > RUNDOWN_TIME_MS))
    {
        continue_setting_speed();
        still_setting_speed = false;
    }
    if (enc_calibrated)
    {
        old_enc_state_A = new_enc_state_A;
        old_enc_state_B = new_enc_state_B;
    
        new_enc_state_A = digitalRead(ENC_PIN_A);
        new_enc_state_B = digitalRead(ENC_PIN_B);
    
        if ((!old_enc_state_A && new_enc_state_B || old_enc_state_A && !new_enc_state_B) &&
            (!old_enc_state_B && !new_enc_state_A || old_enc_state_B && new_enc_state_A))
        {
            angle_deg += 0.5;
        }
        if ((old_enc_state_A && new_enc_state_B || !old_enc_state_A && !new_enc_state_B) &&
            (!old_enc_state_B && new_enc_state_A || old_enc_state_B && !new_enc_state_A))
        {
            angle_deg -= 0.5;
        }
             if (angle_deg < ANGLE_MIN_DEG) {angle_deg = ANGLE_MIN_DEG;}
        else if (angle_deg > ANGLE_MAX_DEG) {angle_deg = ANGLE_MAX_DEG;}

        if (Serial.availableForWrite() >= 6)
        {
            Serial.println(angle_deg);
        }
        if (newly_set_speed > 0)
        {
            // set_speed(+0.5 + 0.6 * (ANGLE_MAX_DEG - angle_deg) / (ANGLE_MAX_DEG - start_angle_deg));
            if (angle_deg > ANGLE_MAX_DEG - 15)
            {
                set_speed(+LO_SPEED);
            }
            else
            {
                set_speed(+HI_SPEED);
            }
        }
        else if (newly_set_speed < 0)
        {
            // set_speed(-0.5 - 0.6 * (angle_deg - ANGLE_MIN_DEG) / (start_angle_deg - ANGLE_MIN_DEG));
            if (angle_deg < ANGLE_MIN_DEG + 15)
            {
                set_speed(-LO_SPEED);
            }
            else
            {
                set_speed(-HI_SPEED);
            }
        }
    }
    delay(10);
}

void set_speed(float _newly_set_speed)
{
    if (!still_setting_speed)
    {
        // Change the speed of the motor, knowing its currently set speed...
    
        // The unsigned speed of the motor is controlled by the electronic speed
        // controller (ESC), while its direction is controlled by the relay module.

        newly_set_speed = _newly_set_speed;

        if (currently_set_speed * newly_set_speed <= 0)
        {
            // If the currently and newly set speeds have opposite signs,
            // or if either are zero...
    
            // First, set the speed to zero using the ESC
            // (if the currently set speed is not already zero):
            my_esc.speed(MIN_PULSE);
            delay(10);
            my_esc.speed(MIN_PULSE);
            delay(10);
            my_esc.speed(MIN_PULSE);
            delay(10);
            my_esc.speed(MIN_PULSE);
            delay(10);
            my_esc.speed(MIN_PULSE);
    
            // Next, wait for the motor to physically stop:
            if (currently_set_speed != 0)
            {
                rundown_start_time_ms = millis();
                still_setting_speed = true;
            }
            // It is assumed that no channels of the relay module
            // should be turned off while the motor is running.
            else
            {
                continue_setting_speed();
            }
        }
        else
        {
            continue_setting_speed();
        }
    }
}
void continue_setting_speed()
{
    if (currently_set_speed * newly_set_speed <= 0)
    {
        if (newly_set_speed > 0)
        {
            // If the newly set speed is positive and nonzero,
            // set the direction accordingly using the relay module:
            relays.turn_off_channel(2);
            relays.turn_off_channel(3);
            relays.turn_on_channel (1);
            relays.turn_on_channel (4);
        }
        else if (newly_set_speed < 0)
        {
            // Else, if the newly set speed is negative and nonzero,
            // set the direction accordingly using the relay module:
            relays.turn_off_channel(1);
            relays.turn_off_channel(4);
            relays.turn_on_channel (2);
            relays.turn_on_channel (3);
        }
        else
        {
            // Else (if the newly set speed is zero), turn off direction
            // control by turning off all four channels of the relay module:
            relays.channelCtrl(0); // Turns off all channels.
            // This does not turn off the relay module itself.
        }
    }
    // Apply the newly set speed using the ESC:
    my_esc.speed(MIN_PULSE + (MAX_PULSE - MIN_PULSE) * abs(newly_set_speed));

    currently_set_speed = newly_set_speed;
    // The currently set speed is the newly set speed.
}
