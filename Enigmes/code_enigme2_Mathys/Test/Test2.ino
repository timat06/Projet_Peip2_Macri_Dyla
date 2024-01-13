char result[2][2] = {
  {'3','0'},
  {'2','1'},
};


int num1=0; //valeur atribuer à la photodiote ligne 1 et colonne 1.
int num2=0; //valeur atribuer à la photodiote ligne 1 et colonne 2.
int num3=0; //valeur atribuer à la photodiote ligne 2 et colonne 1.
int num4=0; //valeur atribuer à la photodiote ligne 2 et colonne 2.
int jeu=0;
void setup() {
Serial.begin(9600);
char actual[2][2] = {
  {'1','2'},
  {'3','0'},
};
num4=analogRead(A3);
if (num4>100){jeu=1;}
else{Serial.println("Replace bien les pièces avant de lancer le jeu");}
Serial.println(actual);
}

void loop() {
while (jeu==1){
num1=analogRead(A0); //lecture de la photodiote ligne 1 et colonne 1.
num2=analogRead(A1); //lecture de la photodiote ligne 1 et colonne 2.
num3=analogRead(A2); //lecture de la photodiote ligne 2 et colonne 1.
num4=analogRead(A3); //lecture de la photodiote ligne 2 et colonne 2.
//Serial.println(num1);
//Serial.println(num2);
//Serial.println(num3);
Serial.println(num4);
}
}
