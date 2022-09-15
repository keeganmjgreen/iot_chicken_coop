#define ESC_PIN 25
#define MIN_PULSE 1000 // microseconds
#define MAX_PULSE 2000 // microseconds
#define ARM_PULSE  500 // microseconds
#define RUNDOWN_TIME_MS 0.5e3

#include "ESC.h"
ESC my_esc(ESC_PIN, MIN_PULSE, MAX_PULSE, ARM_PULSE);

#include <multi_channel_relay.h>
Multi_Channel_Relay relays;

float currently_set_speed = 0;
// Note: The *currently set* speed is not necessarily the *current* speed.

void set_speed(float newly_set_speed);

#define close_button_PIN 23
#define open_button_PIN 33
bool close_button_old_state;
bool close_button_new_state;
bool open_button_old_state;
bool open_button_new_state;
bool close_button_rising_edge;
bool close_button_falling_edge;
bool open_button_rising_edge;
bool open_button_falling_edge;

void setup()
{
    relays.channelCtrl(0); // Turns off all channels.

    Serial.begin(9600);

    my_esc.arm();

    pinMode(close_button_PIN, INPUT_PULLUP);
    pinMode(open_button_PIN, INPUT_PULLUP);
    close_button_new_state = !digitalRead(close_button_PIN);
    open_button_new_state = !digitalRead(open_button_PIN);
}

void loop()
{
    if (Serial.available() > 0)
    {
        set_speed(Serial.parseFloat());
    }

    close_button_old_state = close_button_new_state;
    close_button_new_state = !digitalRead(close_button_PIN);
    open_button_old_state = open_button_new_state;
    open_button_new_state = !digitalRead(open_button_PIN);
    close_button_rising_edge = !close_button_old_state & close_button_new_state;
    close_button_falling_edge = close_button_old_state & !close_button_new_state;
    open_button_rising_edge = !open_button_old_state & open_button_new_state;
    open_button_falling_edge = open_button_old_state & !open_button_new_state;

    if (((currently_set_speed > 0) & (close_button_rising_edge | open_button_falling_edge)) |
        ((currently_set_speed < 0) & (open_button_rising_edge | close_button_falling_edge)))
    {
        set_speed(0);
    }
    else if (currently_set_speed == 0)
    {
        if (!open_button_new_state & close_button_falling_edge)
        {
            set_speed(-0.4);
        }
        else if (!close_button_new_state & open_button_falling_edge)
        {
            set_speed(+0.4);
        }
    }
    delay(100);
}

void set_speed(float newly_set_speed)
{
    // Change the speed of the motor, knowing its currently set speed...

    // The unsigned speed of the motor is controlled by the electronic speed
    // controller (ESC), while its direction is controlled by the relay module.

    if (currently_set_speed * newly_set_speed <= 0)
    {
        // If the currently and newly set speeds have opposite signs,
        // or if either are zero...

        // First, set the speed to zero using the ESC
        // (if the currently set speed is not already zero):
        my_esc.speed(MIN_PULSE);

        // Next, wait for the motor to physically stop:
        if (currently_set_speed != 0)
        {
            delay(RUNDOWN_TIME_MS);
        }
        // It is assumed that no channels of the relay module
        // should be turned off while the motor is running.

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
