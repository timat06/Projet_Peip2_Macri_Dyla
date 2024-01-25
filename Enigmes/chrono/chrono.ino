#include <Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C LCD(0x27, 16, 2);

byte w = 0;

int SEC;
int MIN;
unsigned long timer;

void setup() {
  LCD.init();
  LCD.backlight(); 
  LCD.setCursor(2, 0);
  w = 1;
  SEC = 30;
  MIN = 0;
}


void loop() {
  while (w == 1 ) {
      if (millis() - timer > 995) {
         timer = millis();
         if (SEC == 0) {
          SEC = 59; 
          MIN = MIN - 1;
         }
         else { SEC = SEC - 1; }

         if (SEC == 0 && MIN == 0) {
            LCD.clear();
            LCD.setCursor(2, 0);
            LCD.print("TEMPS ECOULE");
            LCD.setCursor(5, 1);
            LCD.print(MIN);
            LCD.print(" : ");
            LCD.print(SEC);
            w = 0;
         }

         if (SEC == 0) {
            SEC = 59; MIN = MIN - 1;
            if (MIN < 0 ) { MIN = 0; }
         }

         if (w == 1) {
            LCD.clear();
            LCD.setCursor(2, 0);
            LCD.print("TEMPS RESTANT");
            LCD.setCursor(5, 1);
            LCD.print(MIN);
            LCD.print(" : ");
            LCD.print(SEC);
         }
     }
  }
}
