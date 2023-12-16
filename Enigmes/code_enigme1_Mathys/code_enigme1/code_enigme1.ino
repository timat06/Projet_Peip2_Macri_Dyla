const int Bouton_rouge=2; // définie l'entrée de bouton rouge
const int Bouton_vert=4; // définie l'entrée de bouton vert
const int Bouton_jaune=8; // définie l'entrée de bouton jaune
const int Bouton_bleu=12; // définie l'entrée de bouton bleu
const int rouge=3; // nombre de fois que l'on doit appuyer sur le bouton rouge
const int vert=4; // nombre de fois que l'on doit appuyer sur le bouton vert
const int jaune=2; // nombre de fois que l'on doit appuyer sur le bouton jaune
const int bleu=5; // nombre de fois que l'on doit appuyer sur le bouton bleu
int resultat=0; // variable permmettant de savoir quel bouton il faut appuyer
int valr=1; // valeur du bouton rouge (LOW ou HIGH), initialiser à 1 (HIGH)
int valv=1; // valeur du bouton vert (LOW ou HIGH), initialiser à 1 (HIGH)
int valj=1; // valeur du bouton jaune (LOW ou HIGH), initialiser à 1 (HIGH)
int valb=1; // valeur du bouton bleu (LOW ou HIGH), initialiser à 1 (HIGH)
int ancien_valr=1; // ancienne valeur du bouton rouge 
int ancien_valv=1; // ancienne valeur du bouton vert
int ancien_valj=1; // ancienne valeur du bouton jaune
int ancien_valb=1; // ancienne valeur du bouton bleu 






void setup() {
Serial.begin(9600);
pinMode(Bouton_rouge,INPUT_PULLUP); // définie le bouton rouge comme une entrée (cela prend en plus la résistance)
pinMode(Bouton_vert,INPUT_PULLUP); // définie le bouton vert comme une entrée (cela prend en plus la résistance)
pinMode(Bouton_jaune,INPUT_PULLUP); // définie le bouton jaune comme une entrée (cela prend en plus la résistance)
pinMode(Bouton_bleu,INPUT_PULLUP); // définie le bouton bleu comme une entrée (cela prend en plus la résistance)
}

void loop() {
valr=digitalRead(Bouton_rouge); // lis la valeur de bouton rouge
valv=digitalRead(Bouton_vert); // lis la valeur de bouton vert
valj=digitalRead(Bouton_jaune); // lis la valeur de bouton jaune
valb=digitalRead(Bouton_bleu); // lis la valeur de bouton bleu
if ((valr==LOW)&&(ancien_valr==HIGH)){ // si le bouton rouge est appuyé et que qu'il ne l'était pas avant alors 
  if(resultat<rouge){ // si le resultat est inférieur au nombre de fois que l'on doit appuyer sur rouge alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
  }
}
if ((valv==LOW)&&(ancien_valv==HIGH)){ // si le bouton vert est appuyé et que qu'il ne l'était pas avant alors 
  if((rouge<=resultat)&&(resultat<vert+rouge)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge et inférieur à vert+rouge alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
  }
}
if ((valj==LOW)&&(ancien_valj==HIGH)){ // si le bouton jaune est appuyé et que qu'il ne l'était pas avant alors a
  if((vert+rouge<=resultat)&&(resultat<jaune+rouge+vert)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge+vert et inférieur à vert+rouge+jaune alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
  }
}
if ((valb==LOW)&&(ancien_valb==HIGH)){ // si le bouton bleu est appuyé et que qu'il ne l'était pas avant alors a
  if((jaune+rouge+vert<=resultat)&&(resultat<rouge+jaune+vert+bleu)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge+vert+jaune et inférieur à vert+rouge+jaune+bleu alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
  }
}
if(resultat==rouge+jaune+vert+bleu){ // si on a appuyé le bon nombre de fois et dans le bon ordre alors victoire
  Serial.println("Victoire");
}
ancien_valr=valr; //la nouvelle val du bouton rouge de l'ancienne
ancien_valv=valv; //la nouvelle val du bouton vert de l'ancienne
ancien_valj=valj; //la nouvelle val du bouton jaune de l'ancienne
ancien_valb=valb; //la nouvelle val du bouton bleu de l'ancienne
}
