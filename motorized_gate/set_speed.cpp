
// The speed of the motor may be positive or negative,
// depending on the direction.

signed int currently_set_speed = 0;
// Note: The *currently set* speed is not necessarily the *current* speed.

void set_speed(signed int newly_set_speed)
{
    // Change the speed of the motor, knowing its currently set speed...

    // The unsigned speed of the motor is controlled by the electronic speed
    // controller (ESC), while its direction is controlled by the relay module.

    if (currently_set_speed * newly_set_speed > 0)
    {
        // If *both* of the currently and newly set speeds are positive
        // or negative, apply the newly set speed using the ESC:
        esc.speed(abs(newly_set_speed));
    }
    else
    {
        // Else (if the currently and newly set speeds have opposite signs,
        //       or if either are zero)...

        // First, set the speed to zero using the ESC
        // (if the currently set speed is not already zero):
        esc.speed(0);

        // Next, wait for the motor to physically stop:
        delay(get_rundown_time(currently_set_speed));
        // It is assumed that no channels of the relay module
        // should be turned off while the motor is running.

        if (newly_set_speed > 0)
        {
            // If the newly set speed is positive and nonzero,
            // set the direction accordingly using the relay module:
            relays.turn_on_channel (1);
            relays.turn_off_channel(2);
            relays.turn_off_channel(3);
            relays.turn_on_channel (4);
        }
        else if (newly_set_speed < 0)
        {
            // Else, if the newly set speed is negative and nonzero,
            // set the direction accordingly using the relay module:
            relays.turn_off_channel(1);
            relays.turn_on_channel (2);
            relays.turn_on_channel (3);
            relays.turn_off_channel(4);
        }
        else
        {
            // Else (if the newly set speed is zero), turn off direction
            // control by turning off all four channels of the relay module:
            relays.channelCtrl(0); // Turns off all channels.
            // This does not turn off the relay module itself.
        }
        // Finally, apply the newly set speed:
        esc.speed(abs(newly_set_speed));
    }
    currently_set_speed = newly_set_speed;
    // The currently set speed is the newly set speed.
}
