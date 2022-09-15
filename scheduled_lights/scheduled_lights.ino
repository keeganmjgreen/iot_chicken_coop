#include <M5Atom.h>

#include <multi_channel_relay.h>
Multi_Channel_Relay relays;

void setup() {
  // put your setup code here, to run once:
  M5.begin(true, false, true);
  M5.dis.fillpix(0x00ff00);
  relays.turn_off_channel(4); delay(1e3); relays.turn_on_channel(4); delay(1e3);
  relays.turn_off_channel(4); delay(1e3); relays.turn_on_channel(4); delay(1e3);
  relays.turn_off_channel(4); delay(1e3); relays.turn_on_channel(4); delay(1e3);
  relays.turn_on_channel(4);
  delay(1.2883333333333333*3600e3);
  relays.turn_off_channel(4);
  delay(10.0*3600e3);
}

void loop() {
  // put your main code here, to run repeatedly:
  relays.turn_on_channel(4);
  delay(14.0*3600e3);
  relays.turn_off_channel(4);
  delay(10.0*3600e3);
}
