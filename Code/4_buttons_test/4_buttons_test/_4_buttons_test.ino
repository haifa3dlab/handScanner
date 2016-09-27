int b0 = 10;
int bN = 13;
int ledPin = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = b0; i <= bN; ++i) {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int sum = 0;
  for (int i = b0; i <= bN; ++i) {
    int bOn = digitalRead(i);
    //Serial.println(i);
    //Serial.println(bOn);
    sum += (1 << (i - b0)) * bOn;
  }
  Serial.println(sum, BIN);  // print as an ASCII-encoded binary
  delay(1500);
}
