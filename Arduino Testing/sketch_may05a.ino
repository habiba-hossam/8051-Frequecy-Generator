#define ip 21
long prev = 0;
double fq;
void ISR_A(){
long now = micros();
fq = 1000000/(2.0*(now-prev));
prev = now;
}
void setup() {
pinMode(ip, INPUT);
attachInterrupt(digitalPinToInterrupt(ip), ISR_A, CHANGE);
prev = micros();
Serial.begin (9600);
}
void loop() {
Serial.println(fq);
Serial.println();
delay(100);
}
