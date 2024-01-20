//énigme 6
#include <Keypad.h>
int valBouton;
const byte Ligne = 4; // 4 lignes du digicode
const byte Colonne = 4; // 4 colonnes du digicode
int fini = 0; // Sera le signal pour activer l'énigme suivante, passe à 1 si réussie
int bouton = 10;
//int led = 11;
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

static int8_t Send_buf[8] = {0} ;//The MP3 player undestands orders in a 8 int string
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
  pinMode(bouton,INPUT_PULLUP);
  //pinMode(led,OUTPUT);
  //digitalWrite(led,HIGH);
  Serial3.begin(9600);//Start our Serial coms for our serial monitor! 
  delay(500);//Wait chip initialization is complete 
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card   
  delay(200);//wait for 200ms 
}

void loop() {
  valBouton=digitalRead(bouton);
  if(valBouton==HIGH){
    sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00103); //son à changer
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
        sendCommand(CMD_PLAY_WITHFOLDER, 0X0F00107); //son de réussite
        Serial.println("réussi");
      }
      if (codeEssai[0] != code[0] || codeEssai[1] != code[1] || codeEssai[2] != code[2] || codeEssai[3] != code[3] || codeEssai[4] != code[4] || codeEssai[5] != code[5]){ // Si le code n'est pas bon
        Serial.println("code faux"); // Pour les tests
           }
      compteur=0; // Rétablit le compteur à 0 pour retenter sa chance
      }
    }
  }
