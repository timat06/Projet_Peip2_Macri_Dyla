int nombre=random(1,16); //Nombre à trouver entre 1 et 15 compris (correspond à la valeur max sur 4 bits)
int boutonL=6; //Bouton pour activer les lumières (il faut rester appuyé)
int boutonR=7; //Bouton de réponse
int compteur=0; //Compte le nombre de fois appuyé sur le bouton réponse
int valBouton=0; //Valeur boutonL
int valBoutonR=0; //Valeur boutonR
int reussi=0; //Est-ce que l'énigme est réussie ?
int binaire[4]; //Stockage du nombre à trouver en binaire

void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  Serial.begin(9600);
  for(int i=0; i < 4; i++) { //Remplissage de la liste binaire avec le code binaire correspondant au nombre à trouver
    binaire[i]=bitRead(nombre,i);
    Serial.println(bitRead(nombre,i));
  }
  for (int n=2; n < 6; n++) { //Initialisation de toutes les leds éteintes
    digitalWrite(n,HIGH);
  }
}

void loop() {
  valBouton=digitalRead(boutonL); //Si on appuye sur le bouton des lumières, elles s'allument
  if(valBouton == LOW) {
    for(int j=0; j < 4; j++) {
      if(binaire[j] == 1) {
        digitalWrite(j+2,LOW);
      }
    }
  }
  else { //Sinon elles s'éteignent
    for(int j=0; j < 4; j++) {
      if(binaire[j] == 1) {
        digitalWrite(j+2,HIGH);
      }
    }
    compteur=0; //Pour recommencer si on se trompe de réponse
  }
  valBoutonR=digitalRead(boutonR); 
  if (valBoutonR == LOW) {
      compteur +=1;
      // Bruit sonore pour confirmer la prise en compte à ajouter
      Serial.println(compteur); //test
      delay(1000);
  }
  if (compteur == nombre) {
      reussi=1; //Passage à l'autre énigme
      //Victoire
      Serial.println("reussi");
  }
}