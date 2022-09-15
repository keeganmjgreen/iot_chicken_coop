#define IR_BREAK_BEAM_PIN 25

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IR_BREAK_BEAM_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(IR_BREAK_BEAM_PIN));
  delay(100);
}
