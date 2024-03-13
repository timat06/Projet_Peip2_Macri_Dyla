#include <Keypad.h>
#include <Wire.h>
#include<LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servoMotor;
Servo servoMotor2;
Servo servoMotor3; //  Créez un objet servo

//Initialisation pour énigme 1 et 2
const int Bouton_rouge=22; // définis l'entrée de bouton rouge
const int Bouton_vert=26; // définis l'entrée de bouton vert
const int Bouton_jaune=28; // définis l'entrée de bouton jaune
const int Bouton_bleu=24; // définis l'entrée de bouton bleu
const int rouge=3; // nombre de fois que l'on doit appuyer sur le bouton rouge
const int vert=4; // nombre de fois que l'on doit appuyer sur le bouton vert
const int jaune=2; // nombre de fois que l'on doit appuyer sur le bouton jaune
const int bleu=5; // nombre de fois que l'on doit appuyer sur le bouton bleu
const int servo1=30;
const int servo2=32
const int servo3=34;
int compte=0; // variable permmettant de savoir quel bouton il faut appuyer
int valr=1; // valeur du bouton rouge (LOW ou HIGH), initialiser à 1 (HIGH)
int valv=1; // valeur du bouton vert (LOW ou HIGH), initialiser à 1 (HIGH)
int valj=1; // valeur du bouton jaune (LOW ou HIGH), initialiser à 1 (HIGH)
int valb=1; // valeur du bouton bleu (LOW ou HIGH), initialiser à 1 (HIGH)
int ancien_valr=1; // ancienne valeur du bouton rouge 
int ancien_valv=1; // ancienne valeur du bouton vert
int ancien_valj=1; // ancienne valeur du bouton jaune
int ancien_valb=1; // ancienne valeur du bouton bleu
int etape1=0;
int resultat=0;
int etape2=1;

//Initialisation pour énigme 3
int capteur1=10;
int capteur2=11;
int val1;
int val2;
int chemin1=0;
int chemin2=0;
int suivant=1;

//Initialisation pour énigme 5
int nombre=5; //Utiliser randomSeed() avec un analogRead() dedans pour changer
int boutonL=6; //Bouton pour activer les lumières (il faut rester appuyé)
int boutonR=7; //Bouton de réponse
int compteurBouton=0; //Compte le nombre de fois appuyé sur le bouton réponse
int valBoutonL=0; //Valeur boutonL
int valBoutonR=0; //Valeur boutonR
int prochaineEtape1; //Est-ce que l'énigme est réussie ?
int binaire[4]; //Stockage du nombre à trouver en binaire
int debut=1; //Sert pour lancer la 1ère énigme

//Initilisation pour énigme 4 et pad
int compteurPad1 =0; // Permet de compter le nombre de touche appuyée
const int boutonV1 = 8; // Entrée du bouton sur la carte
int valBoutonV1=1; // Bouton appuyé ?
const byte Ligne = 4; // 4 lignes du digicode
const byte Colonne = 4; // 4 colonnes du digicode
int prochaineEtape2 = 0; // Sera le signal pour activer l'énigme suivante, passe à 1 si réussie
char code1[4]={'1','2','3','4'}; // Code à trouver sous forme de liste, sera probablement random par la suite
// Touches du keypad
char keys[Ligne][Colonne] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
char codeEssai1[4]; // Contient les numéros choisis par le joueur
byte Ligne_Pins[Ligne] = {53, 51, 49, 47}; // Correspond aux branchements sur la carte 
byte Colonne_Pins[Colonne] = {45, 43, 41, 39}; // Pareil 
// Initialise la librairie avec nos paramètres (la taille de la matrice et symboles)
Keypad mon_keypad = Keypad( makeKeymap(keys), Ligne_Pins, Colonne_Pins, Ligne, Colonne);

// Initialisation énigme 6
int valBoutonV2;
int fini =0;
int boutonV2 = 9;
int compteurPad2 = 0;
char code2[6] = {'A','C','A','D','C','D'}; // Cri d'animaux
char codeEssai2[6];

//Initialisation MP3
static int8_t Send_buf[8] = {0} ;//The MP3 player undestands orders in a 8 int string 
                                //0X7E FF 06 command 00 00 00 EF;(if command =01 next song order)
#define NEXT_SONG 0X01  
#define PREV_SONG 0X02  
#define CMD_PLAY_W_INDEX 0X03 //DATA IS REQUIRED (number of song) 
#define VOLUME_UP_ONE 0X04 
#define VOLUME_DOWN_ONE 0X05 
#define CMD_SET_VOLUME 0X06//DATA IS REQUIRED (number of volume from 0 up to 30(0x1E)) 
#define SET_DAC 0X17 
#define CMD_PLAY_WITHVOLUME 0X22 //data is needed  0x7E 06 22 00 xx yy EF;(xx volume)(yy number of song) 
#define CMD_SEL_DEV 0X09 //SELECT STORAGE DEVICE, DATA IS REQUIRED 
#define DEV_TF 0X02 //HELLO,IM THE DATA REQUIRED 
#define SLEEP_MODE_START 0X0A 
#define SLEEP_MODE_WAKEUP 0X0B 
#define CMD_RESET 0X0C//CHIP RESET 
#define CMD_PLAY 0X0D //RESUME PLAYBACK 
#define CMD_PAUSE 0X0E //PLAYBACK IS PAUSED 
#define CMD_PLAY_WITHFOLDER 0X0F//DATA IS NEEDED, 0x7E 06 0F 00 01 02 EF;(play the song with the directory \01\002xxxxxx.mp3 
#define STOP_PLAY 0X16 
#define PLAY_FOLDER 0X17// data is needed 0x7E 06 17 00 01 XX EF;(play the 01 folder)(value xx we dont care) 
#define SET_CYCLEPLAY 0X19//data is needed 00 start; 01 close 
#define SET_DAC 0X17//data is needed 00 start DAC OUTPUT;01 DAC no output 
#define SINGLE_PLAY 0X08//Single play(without folder)

void sendCommand(int8_t command, int16_t dat) { 
delay(20); 
Send_buf[0] = 0x7e; //starting byte 
Send_buf[1] = 0xff; //version 
Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte 
Send_buf[3] = command; // 
Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback 
Send_buf[5] = (int8_t)(dat >> 8);//datah 
Send_buf[6] = (int8_t)(dat); //datal 
Send_buf[7] = 0xef; //ending byte 
for(uint8_t i=0; i<8; i++){ 
  Serial3.write(Send_buf[i]) ; 
} 
} 

//Chrono
LiquidCrystal_I2C LCD(0x27, 16, 2);
byte w;
int SEC;
int MIN;
unsigned long timer;
int timesUp;

void setup() {
  // chrono
  LCD.init();
  LCD.backlight(); 
  LCD.setCursor(2, 0);
  w = 1;
  SEC = 0;
  MIN = 15;
  timesUp = 0;
  //Enigme 1 boutons
  pinMode(Bouton_rouge,INPUT_PULLUP); // définis le bouton rouge comme une entrée (cela prend en plus la résistance)
  pinMode(Bouton_vert,INPUT_PULLUP); // définis le bouton vert comme une entrée (cela prend en plus la résistance)
  pinMode(Bouton_jaune,INPUT_PULLUP); // définis le bouton jaune comme une entrée (cela prend en plus la résistance)
  pinMode(Bouton_bleu,INPUT_PULLUP); // définis le bouton bleu comme une entrée (cela prend en plus la résistance)
  servoMotor.attach(servo1);
  servoMotor2.attach(servo2);
  servoMotor3.attach(servo3);
  servoMotor.write(0);
  servoMotor2.write(0);
  servoMotor3.write(0);
  // leds et boutons
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(boutonL,INPUT_PULLUP);
  pinMode(boutonR,INPUT_PULLUP);
  pinMode(boutonV1,INPUT_PULLUP);
  pinMode(boutonV2,INPUT_PULLUP);
  pinMode(capteur1,INPUT);
  pinMode(capteur2,INPUT);
  Serial.println(nombre);
  for(int i=0; i < 4; i++) { //Remplissage de la liste binaire avec le code binaire correspondant au nombre à trouver
    binaire[i]=bitRead(nombre,i);
  }
  for (int n=2; n < 6; n++) { //Initialisation de toutes les leds éteintes
    digitalWrite(n,HIGH);
  }
  Serial3.begin(9600);//Start our Serial coms for our serial monitor! 
  delay(500);//Wait chip initialization is complete 
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card   
  delay(200);//wait for 200ms
  sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00103); //Son de démarrage
  delay(100);
  sendCommand(CMD_PAUSE, 0X0E);
}

void loop() {
  // Enigme 1 A tester
while (etape1==0 && timesUp==0){
  valr=digitalRead(Bouton_rouge); // lis la valeur de bouton rouge
  valv=digitalRead(Bouton_vert); // lis la valeur de bouton vert
  valj=digitalRead(Bouton_jaune); // lis la valeur de bouton jaune
  valb=digitalRead(Bouton_bleu); // lis la valeur de bouton bleu
if ((valr==LOW)&&(ancien_valr==HIGH)){ // si le bouton rouge est appuyé et que qu'il ne l'était pas avant alors 
  if(resultat<rouge){ // si le resultat est inférieur au nombre de fois que l'on doit appuyer sur rouge alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
  }
}
if ((valv==LOW)&&(ancien_valv==HIGH)){ // si le bouton vert est appuyé et que qu'il ne l'était pas avant alors 
  if((rouge<=resultat)&&(resultat<vert+rouge)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge et inférieur à vert+rouge alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
  }
}
if ((valj==LOW)&&(ancien_valj==HIGH)){ // si le bouton jaune est appuyé et que qu'il ne l'était pas avant alors a
  if((vert+rouge<=resultat)&&(resultat<jaune+rouge+vert)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge+vert et inférieur à vert+rouge+jaune alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
  }
}
if ((valb==LOW)&&(ancien_valb==HIGH)){ // si le bouton bleu est appuyé et que qu'il ne l'était pas avant alors a
  if((jaune+rouge+vert<=resultat)&&(resultat<rouge+jaune+vert+bleu)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge+vert+jaune et inférieur à vert+rouge+jaune+bleu alors
    resultat= resultat+1; //resultat+=1
    Serial.println(resultat);
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
  }
  else{ //sinon
    resultat=0; //resultat =0
    Serial.println("FALSE");
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
  }
}
if(resultat==rouge+jaune+vert+bleu){ // si on a appuyé le bon nombre de fois et dans le bon ordre alors victoire
  Serial.println("Victoire");
  servoMotor.write(180);
  sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104);
  sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00102); //son de réussite
  etape1=1;
  resultat=1;
  etape2=1;
}
ancien_valr=valr; //la nouvelle val du bouton rouge de l'ancienne
ancien_valv=valv; //la nouvelle val du bouton vert de l'ancienne
ancien_valj=valj; //la nouvelle val du bouton jaune de l'ancienne
ancien_valb=valb; //la nouvelle val du bouton bleu de l'ancienne
delay(50);

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
        timesUp = 1;
        break;
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
  //Enigme 2
  while (etape2==1 && timesUp==0){
    valr=digitalRead(Bouton_rouge); // lis la valeur de bouton rouge
    valv=digitalRead(Bouton_vert); // lis la valeur de bouton vert
    valj=digitalRead(Bouton_jaune); // lis la valeur de bouton jaune
    valb=digitalRead(Bouton_bleu); // lis la valeur de bouton bleu
  if ((valr==LOW)&&(ancien_valr==HIGH)){ // si le bouton rouge est appuyé et que qu'il ne l'était pas avant alors 
    if((resultat==3)||(resultat==7)||(resultat==8)||(resultat==11)){ // si le resultat est inférieur au nombre de fois que l'on doit appuyer sur rouge alors
      resultat= resultat+1; //resultat+=1
      Serial.println(resultat);
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
    }
    else{ //sinon
      resultat=1; //resultat =0
      Serial.println("FALSE");
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
    }
  }
  if ((valv==LOW)&&(ancien_valv==HIGH)){ // si le bouton vert est appuyé et que qu'il ne l'était pas avant alors 
    if((resultat==1)||(resultat==2)||(resultat==13)||(resultat==15)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge et inférieur à vert+rouge alors
      resultat= resultat+1; //resultat+=1
      Serial.println(resultat);
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
    }
    else{ //sinon
      resultat=1; //resultat =0
      Serial.println("FALSE");
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
    }
  }
  if ((valj==LOW)&&(ancien_valj==HIGH)){ // si le bouton jaune est appuyé et que qu'il ne l'était pas avant alors a
    if((resultat==5)||(resultat==9)||(resultat==12)||(resultat==14)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge+vert et inférieur à vert+rouge+jaune alors
      resultat= resultat+1; //resultat+=1
      Serial.println(resultat);
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
    }
    else{ //sinon
      resultat=1; //resultat =0
      Serial.println("FALSE");
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
    }
  }
  if ((valb==LOW)&&(ancien_valb==HIGH)){ // si le bouton bleu est appuyé et que qu'il ne l'était pas avant alors a
    if((resultat==4)||(resultat==6)||(resultat==10)){ // si le resultat est supérieur au nombre de fois que l'on doit appuyer sur rouge+vert+jaune et inférieur à vert+rouge+jaune+bleu alors
      resultat= resultat+1; //resultat+=1
      Serial.println(resultat);
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
    }
    else{ //sinon
      resultat=1; //resultat =0
      Serial.println("FALSE");
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
    }
  }
  if(resultat==16){ // si on a appuyé le bon nombre de fois et dans le bon ordre alors victoire
    Serial.println("Victoire");
    servoMotor2.write(180);
    etape2=0;
    resultat=0;
    suivant=1;
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00102); //son de réussite

  }
  ancien_valr=valr; //la nouvelle val du bouton rouge de l'ancienne
  ancien_valv=valv; //la nouvelle val du bouton vert de l'ancienne
  ancien_valj=valj; //la nouvelle val du bouton jaune de l'ancienne
  ancien_valb=valb; //la nouvelle val du bouton bleu de l'ancienne
  delay(50);
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
            timesUp = 1;
            break;
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
  // Enigme 3
  while (suivant == 1 && timesUp==0) {
    val1=digitalRead(capteur1);
    val2=digitalRead(capteur2);
    if(val1==HIGH) {
      chemin1=1;
      Serial.println("chemin1 ok");
    }
    if(val2==HIGH) {
      chemin2=1;
      Serial.println("chemin2 ok");
    }
    if(chemin1==1 && chemin2==1) {
      prochaineEtape1=0; //Passage à l'autre énigme
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00102); //son de réussite
      suivant=0;
      Serial.println("Victoire");
    }
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
        timesUp = 1;
        break;
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
  // Enigme 5 qui finalement sera 4
  while ((prochaineEtape1 == 0) && (timesUp == 0)) {
    valBoutonL=digitalRead(boutonL); //Si on appuye sur le bouton des lumières, elles s'allument
    if(valBoutonL == LOW) {
      for(int j=0; j < 4; j++) {
        if(binaire[j] == 1) {
          digitalWrite(j+2,LOW);
        }
      }
      compteurBouton=0; //Pour recommencer si on se trompe de réponse
    }
    else { //Sinon elles s'éteignent
      for(int j=0; j < 4; j++) {
        if(binaire[j] == 1) {
          digitalWrite(j+2,HIGH);
        }
      }
    
    }

    valBoutonR=digitalRead(boutonR); 
    if (valBoutonR == LOW) {
      compteurBouton +=1;
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00104); //Son de prise en compte
      Serial.println(compteurBouton); //test
      delay(500);
      if (compteurBouton == nombre) {
        prochaineEtape1=1; //Passage à l'autre énigme
        sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00102); //son de réussite
        debut = 0;
        //Victoire
        Serial.println("Enigme1 ok");
        servoMotor3.write(180);
      }
    }
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
            timesUp = 1;
            break;
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

  //Enigme 4 qui finalement sera 5
  while ((prochaineEtape1 == 1) && (prochaineEtape2 == 0) && (timesUp == 0)) {
    valBoutonV1 = digitalRead(boutonV1);
    if (valBoutonV1 == LOW) {
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00105); //son à changer
    }
    char keypad_matrix1 = mon_keypad.getKey(); // Récupère la touche appuyé
    if (keypad_matrix1){ // Si une touche est appuyée
      codeEssai1[compteurPad1] = keypad_matrix1; // Met la valeur dans la liste
      compteurPad1+=1; // Incrémente pour que l'on se déplace dans la liste des valeurs
      Serial.println(keypad_matrix1); // Ne sert que pour les tests
      if (keypad_matrix1 =='C'){// Si on appuie sur C, on réintialise les valeurs
    	  compteurPad1=0;
      }
      if (compteurPad1 == 4){ // Si on a 4 valeurs, on vérifie si le code est bon
        if (codeEssai1[0] == code1[0] && codeEssai1[1] == code1[1] && codeEssai1[2] == code1[2] && codeEssai1[3] == code1[3]){ // Si le code est bon
      	  Serial.println("Etape 2 ok"); // Pour les tests
          prochaineEtape2 = 1; // Déverouille la prochaine éngime
          sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00102); //son de réussite

        }
      
        if (codeEssai1[0] != code1[0] || codeEssai1[1] != code1[1] || codeEssai1[2] != code1[2] || codeEssai1[3] != code1[3]){ // Si le code n'est pas bon
          Serial.println("Etape 2 erreur"); // Pour les tests
          sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
          }
        compteurPad1=0; // Rétablit le compteur à 0 pour retenter sa chance
        }
    }
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
            timesUp = 1;
            break;
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

  // Enigme 6
  while ((prochaineEtape2 == 1) && (fini == 0) && (timesUp == 0)) {
    valBoutonV2 = digitalRead(boutonV2);
    if(valBoutonV2==LOW){
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00106); //son à changer
    }
    char keypad_matrix2 = mon_keypad.getKey(); // Récupère la touche appuyé
    if (keypad_matrix2){ // Si une touche est appuyée
      codeEssai2[compteurPad2] = keypad_matrix2; // Met la valeur dans la liste
      compteurPad2+=1; // Incrémente pour que l'on se déplace dans la liste des valeurs
      Serial.println(keypad_matrix2); // Ne sert que pour les tests
      if (keypad_matrix2 =='*'){// Si on appuie sur *, on réintialise les valeurs
    	  compteurPad2=0;
      }
      if (compteurPad2 == 6){ // Si on a 6 valeurs, on vérifie si le code est bon
        if (codeEssai2[0] == code2[0] && codeEssai2[1] == code2[1] && codeEssai2[2] == code2[2] && codeEssai2[3] == code2[3] && codeEssai2[4] == code2[4] && codeEssai2[5] == code2[5]){ // Si le code est bon
          fini = 1;
          sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00102); //son de réussite
          Serial.println("Etape3 ok");
          Serial.println("jeu terminé");
        }
        if (codeEssai2[0] != code2[0] || codeEssai2[1] != code2[1] || codeEssai2[2] != code2[2] || codeEssai2[3] != code2[3] || codeEssai2[4] != code2[4] || codeEssai2[5] != code2[5]){ // Si le code n'est pas bon
          Serial.println("Etape 3 erreur"); // Pour les tests
          sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00101); //Son de mauvaise réponse
        }
        compteurPad2=0; // Rétablit le compteur à 0 pour retenter sa chance
      }
    }
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
            timesUp = 1;
            break;
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

if(w==1) {
  LCD.clear();
  LCD.setCursor(5,0);
  LCD.print("MISSION");
  LCD.setCursor(4,1);
  LCD.print("ACCOMPLIE");
  delay(100);
}

if(w==0) {
  LCD.clear();
  LCD.setCursor(2,0);
  LCD.print("DOMMAGE VOUS");
  LCD.setCursor(3,1);
  LCD.print("AVEZ PERDU");
  delay(100);
}
}