void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int av = Serial.available();
  if (av) {
    String s = Serial.readString();
    Serial.print("Got: ");
    Serial.println(s);
  }
  delay(500);
}
