// #include <arduino-timer.h>

int pinA = 22; // Pin for Encoder Channel A.
int pinB = 19; // Pin for Encoder Channel B.

bool oldA; // Old Encoder Channel A signal.
bool oldB; // Old Encoder Channel B signal.

bool newA; // New Encoder Channel A signal.
bool newB; // New Encoder Channel B signal.

float numDegrees = 0; // Current knee angle (relative - incremental encoder) (0.5-degree resolution).

// // strDegrees = "±XXX.X\n"
// char strDegrees[7]; // charsPerMessage = 7

int bitsPerSecond = 9600; // Default baud (data transfer) rate.
// Reliable over Bluetooth.

/*
 * int startBitsPerChar = 1;
 * int  dataBitsPerChar = 8;
 * int  stopBitsPerChar = 1;
 *
 * int bitsPerChar = startBitsPerChar + dataBitsPerChar + stopBitsPerChar;
 *
 * float secondsPerChar = bitsPerChar / bitsPerSecond;
 *
 * int charsPerMessage = 7;
 *
 * float secondsPerMessage = secondsPerChar * charsPerMessage;
 *
 * int millisPerMessage = ceil(secondsPerMessage * 1e3);
 *
 */

// int millisPerMessage = 10;
// 
// Timer<1, millis, char *> timer;

void setup()
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);

    newA = digitalRead(pinA);
    newB = digitalRead(pinB);

    Serial.begin(bitsPerSecond);

    // timer.every(millisPerMessage, [](char *strDegrees) -> bool { Serial.println(strDegrees); return true; }, strDegrees);
}
void loop()
{
    oldA = newA;
    oldB = newB;

    newA = digitalRead(pinA);
    newB = digitalRead(pinB);

    if ((!oldA && newB || oldA && !newB) &&
    (!oldB && !newA || oldB && newA))
    {
        numDegrees += 0.5;
    }
    if ((oldA && newB || !oldA && !newB) &&
    (!oldB && newA || oldB && !newA))
    {
        numDegrees -= 0.5;
    }

    // dtostrf(numDegrees, 3, 1, strDegrees);

    // timer.tick();
    if (Serial.availableForWrite() >= 7)
    {
        Serial.println(numDegrees);
    }
}
