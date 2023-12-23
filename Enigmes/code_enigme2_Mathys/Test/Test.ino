int num1=0;
int num2=0;
int num3=0;
int num4=0;
void setup() {
Serial.begin(9600);
}

void loop() {
num1=analogRead(A0);
num2=analogRead(A1);
num3=analogRead(A2);
num4=analogRead(A3);
//Serial.println(num1);
//Serial.println(num2);
//Serial.println(num3);
Serial.println(num4);
}
