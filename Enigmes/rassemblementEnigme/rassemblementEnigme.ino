#include <Keypad.h>

//Initialisation pour énigme 5
int nombre=5; //Utiliser randomSeed() avec un analogRead() dedans pour changer
int boutonL=6; //Bouton pour activer les lumières (il faut rester appuyé)
int boutonR=7; //Bouton de réponse
int compteurBouton=0; //Compte le nombre de fois appuyé sur le bouton réponse
int valBoutonL=0; //Valeur boutonL
int valBoutonR=0; //Valeur boutonR
int prochaineEtape1=0; //Est-ce que l'énigme est réussie ?
int binaire[4]; //Stockage du nombre à trouver en binaire

//Initilisation pour énigme 4 et pad
int compteurPad1 =0; // Permet de compter le nombre de touche appuyée
const int boutonV1 = 10; // Entrée du bouton sur la carte
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
byte Ligne_Pins[Ligne] = {9, 8, 7, 6}; // Correspond aux branchements sur la carte 
byte Colonne_Pins[Colonne] = {5, 4, 3, 2}; // Pareil 
// Initialise la librairie avec nos paramètres (la taille de la matrice et symboles)
Keypad mon_keypad = Keypad( makeKeymap(keys), Ligne_Pins, Colonne_Pins, Ligne, Colonne);

// Initialisation énigme 6
int valBoutonV2;
int fini =0;
int boutonV2 = 11;
int compteurPad2 = 0;
char code2[6] = {'A','C','D','A','B','D'}; // Temporaire (trouver une jolie mélodie)
char codeEssai2[6];

// Initialisation MP3
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


void setup() {
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(boutonL,INPUT_PULLUP);
  pinMode(boutonR,INPUT_PULLUP);
  pinMode(boutonV1,INPUT_PULLUP);
  pinMode(boutonV2,INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println(nombre);
  for(int i=0; i < 4; i++) { //Remplissage de la liste binaire avec le code binaire correspondant au nombre à trouver
    binaire[i]=bitRead(nombre,i);
    //Serial.println(bitRead(nombre,i));
  }
  for (int n=2; n < 6; n++) { //Initialisation de toutes les leds éteintes
    digitalWrite(n,HIGH);
  }
  Serial3.begin(9600);//Start our Serial coms for our serial monitor! 
  delay(500);//Wait chip initialization is complete 
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card   
  delay(200);//wait for 200ms
  sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00106); //Il faudra ajouter un son de démarrage de la boîte
  delay(100);
  sendCommand(CMD_PAUSE, 0X0E);
}

void loop() {

  // Enigme 5 qui finalement sera 4
  if (prochaineEtape1 == 0) {
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
      // Bruit sonore pour confirmer la prise en compte à ajouter
      Serial.println(compteurBouton); //test
      delay(200);
      if (compteurBouton == nombre) {
        prochaineEtape1=1; //Passage à l'autre énigme
        //Victoire
        Serial.println("reussi");
      }
    }
  }

  //Enigme 4 qui finalement sera 5
  if ((prochaineEtape1 == 1) && (prochaineEtape2 == 0)) {
    valBoutonV1 = digitalRead(boutonV1);
    if (valBoutonV1 == HIGH) {
      sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00106); //son à changer
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
      	  Serial.println("True"); // Pour les tests
          prochaineEtape2 = 1; // Déverouille la prochaine éngime
          sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00107); //son de réussite

        }
      
        if (codeEssai1[0] != code1[0] || codeEssai1[1] != code1[1] || codeEssai1[2] != code1[2] || codeEssai1[3] != code1[3]){ // Si le code n'est pas bon
          Serial.println("False"); // Pour les tests
          //Ajouter un son d'erreur ?
          }
        compteurPad1=0; // Rétablit le compteur à 0 pour retenter sa chance
        }
    }
  }

  // Enigme 6
  if ((prochaineEtape2 == 1) && (fini == 0)) {
    valBoutonV2 = digitalRead(boutonV2);
    if(valBoutonV2==HIGH){
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00103); //son à changer
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
          sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00107); //son de réussite
          Serial.println("réussi");
        }
        if (codeEssai2[0] != code2[0] || codeEssai2[1] != code2[1] || codeEssai2[2] != code2[2] || codeEssai2[3] != code2[3] || codeEssai2[4] != code2[4] || codeEssai2[5] != code2[5]){ // Si le code n'est pas bon
          Serial.println("code faux"); // Pour les tests
          //Ajouter un son d'erreur ?
        }
        compteurPad2=0; // Rétablit le compteur à 0 pour retenter sa chance
      }
    }
  }
}