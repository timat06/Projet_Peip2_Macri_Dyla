#include <Keypad.h>
int compteur =0; // Permet de compter le nombre de touche appuyée
const int bouton = 10; // Entrée du bouton sur la carte
int valBouton=1; // Bouton appuyé ?
int etat=1; // Allumé ou éteint
int ancien_val=1; // Ancienne valeur bouton
const byte Ligne = 4; // 4 lignes du digicode
const byte Colonne = 4; // 4 colonnes du digicode
int prochaineEtape = 0; // Sera le signal pour activer l'énigme suivante, passe à 1 si réussie
char code[4]={'1','2','3','4'}; // Code à trouver sous forme de liste, sera probablement random par la suite

// Touches du keypad
char keys[Ligne][Colonne] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

char codeEssai[4]; // Contient les numéros choisis par le joueur
byte Ligne_Pins[Ligne] = {9, 8, 7, 6}; // Correspond aux branchements sur la carte 
byte Colonne_Pins[Colonne] = {5, 4, 3, 2}; // Pareil 

// Initialise la librairie avec nos paramètres (la taille de la matrice et symboles)
Keypad mon_keypad = Keypad( makeKeymap(keys), Ligne_Pins, Colonne_Pins, Ligne, Colonne); 

void setup(){
  Serial.begin(9600);
  pinMode(bouton,INPUT);
}
  
void loop(){
  valBouton=digitalRead(bouton);
  if((valBouton==LOW)&&(ancien_val==HIGH)){etat=1-etat;}
  ancien_val=valBouton;
  if(etat==HIGH){ // La voix ne parle pas
    Serial.println("Voix"); //test
  }
  else { // La voix donne les indications
  Serial.println("Pas de voix"); //test
  }
  char keypad_matrix = mon_keypad.getKey(); // Récupère la touche appuyé
  if (keypad_matrix){ // Si une touche est appuyée
    codeEssai[compteur] = keypad_matrix; // Met la valeur dans la liste
    compteur+=1; // Incrémente pour que l'on se déplace dans la liste des valeurs
    Serial.println(keypad_matrix); // Ne sert que pour les tests
    if (keypad_matrix =='C'){// Si on appuie sur C, on réintialise les valeurs
    	compteur=0;
    }
    if (compteur == 4){ // Si on a 4 valeurs, on vérifie si le code est bon
      if (codeEssai[0] == code[0] && codeEssai[1] == code[1] && codeEssai[2] == code[2] && codeEssai[3] == code[3]){ // Si le code est bon
      	Serial.println("True"); // Pour les tests
        prochaineEtape = 1; // Déverouille la prochaine éngime

      }
      
      if (codeEssai[0] != code[0] || codeEssai[1] != code[1] || codeEssai[2] != code[2] || codeEssai[3] != code[3]){ // Si le code n'est pas bon
        Serial.println("False"); // Pour les tests
           }
      compteur=0; // Rétablit le compteur à 0 pour retenter sa chance
      }
  }
  
}