//énigme 6
#include <Keypad.h>
int lastState = LOW;
int currentState;
const byte Ligne = 4; // 4 lignes du digicode
const byte Colonne = 4; // 4 colonnes du digicode
int fini = 0; // Sera le signal pour activer l'énigme suivante, passe à 1 si réussie
int bouton = 10;
int led = 11;
int compteur =0; // Permet de compter le nombre de touche appuyée
//Schéma à identifier
char code[6] ={'A','C','D','A','B','D'}; // Temporaire (trouver une jolie mélodie)
char codeEssai[6];
// Touches du keypad
char keys[Ligne][Colonne] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte Ligne_Pins[Ligne] = {9, 8, 7, 6}; // Correspond aux branchements sur la carte 
byte Colonne_Pins[Colonne] = {5, 4, 3, 2}; // Pareil 
Keypad mon_keypad = Keypad(makeKeymap(keys), Ligne_Pins, Colonne_Pins, Ligne, Colonne); 

void setup() {
  Serial.begin(9600);
  pinMode(bouton,INPUT_PULLUP);
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
}

void loop() {
  currentState = digitalRead(bouton);
  if(currentState == LOW) {
    //les notes de musiques sont jouées
    digitalWrite(led,LOW);
  }
  else {
    //le son s'arrête
    digitalWrite(led,HIGH);
  }
  char keypad_matrix = mon_keypad.getKey(); // Récupère la touche appuyé
  if (keypad_matrix){ // Si une touche est appuyée
    codeEssai[compteur] = keypad_matrix; // Met la valeur dans la liste
    compteur+=1; // Incrémente pour que l'on se déplace dans la liste des valeurs
    Serial.println(keypad_matrix); // Ne sert que pour les tests
    if (keypad_matrix =='*'){// Si on appuie sur *, on réintialise les valeurs
    	compteur=0;
    }
    if (compteur == 6){ // Si on a 6 valeurs, on vérifie si le code est bon
      if (codeEssai[0] == code[0] && codeEssai[1] == code[1] && codeEssai[2] == code[2] && codeEssai[3] == code[3] && codeEssai[4] == code[4] && codeEssai[5] == code[5]){ // Si le code est bon
        fini = 1;
        Serial.println("réussi");
      }
      if (codeEssai[0] != code[0] || codeEssai[1] != code[1] || codeEssai[2] != code[2] || codeEssai[3] != code[3] || codeEssai[4] != code[4] || codeEssai[5] != code[5]){ // Si le code n'est pas bon
        Serial.println("code faux"); // Pour les tests
           }
      compteur=0; // Rétablit le compteur à 0 pour retenter sa chance
      }
    }
  }