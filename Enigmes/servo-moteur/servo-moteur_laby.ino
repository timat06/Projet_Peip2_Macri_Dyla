#include <Servo.h>

Servo servoMotor; //  Créez un objet servo

void setup() {
  servoMotor.attach(9); // Indiquez la broche à laquelle le servo est connecté (ici : broche 9)
}

void loop() {
  servoMotor.write(-180); // Envoyez le servo à la position de 0 degré
  delay(1000); // Attendez 1 seconde
  servoMotor.write(0); // Envoyez le servo à la position de 90 degrés
  delay(1000); // Attendez 1 seconde
  servoMotor.write(90); // Envoyez le servo à la position de 180 degrés
  delay(1000); // Attendez 1 seconde
  servoMotor.write(180); // Envoyez le servo à la position de 180 degrés
  delay(1000); // Attendez 1 seconde
  servoMotor.write(360); // Envoyez le servo à la position de 180 degrés
  delay(1000); // Attendez 1 seconde
}