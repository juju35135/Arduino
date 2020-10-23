// -----------------------------------------
// commande et contrôle pour ampli à tube
// juju35135, aucuns droits reservés 
// version beta 1.11
// -----------------------------------------



int R=1;
int V=1;
int B=1;


int ledRouge=5; // Constante pour la broche 5
int ledVert=6; // Constante pour la broche 6
int ledBleu=9; // Constante pour la broche 9

int pwmBleu;

int verif = 3;// verification filament PIN 3 
int etatfil=0;// etat du filament
int bouton = 2;// bouton PIN 2
int led = 19;// led sur A5 PIN 19
int etatbouton = 0; // variable etat bouton (appuie ou non appuie)
int dernieretatbouton = 0;// variable memoire derniere position du bouton
int etatled = 0;// varibale de la led, soit éteinte soit allumée
int pinR1, pinR2;
int etatR1, etatR2;
int allumage = 1 ; // état allumage
int tempoActive = 0; // État d'activation de la tempo
long prechauffe, postchauffe ;
unsigned long tempoDepart = 0; // Temps à l'activation de la tempo


void setup() {
  
prechauffe = 60000  ; //durée prechauffage
postchauffe = 45000 ;//durée postchauffage

pinMode (ledVert,OUTPUT); // Broche ledVert configurée en sortie
pinMode (ledRouge,OUTPUT); // Broche ledRouge configurée en sortie
pinMode (ledBleu,OUTPUT); // Broche ledBleu configurée en sortie


pinMode(verif, INPUT_PULLUP);
pinR1 = 14; //Relais sur A0 pin 14
pinR2 = 15; //Relais saur A1 pin 15

pinMode(bouton, INPUT_PULLUP);
pinMode(led, OUTPUT);
pinMode(pinR1, OUTPUT); //mode écriture pour relais1
pinMode(pinR2, OUTPUT); //mode écriture pour relais2 

digitalWrite(pinR1,HIGH); //forcage relais1 en position ouvert 
digitalWrite(pinR2,HIGH); //forcage relais2 en position ouvert
etatR1 = 1; //ecriture variable état relais1
etatR2 = 1; //ecriture variable état relais2

digitalWrite(ledVert,HIGH); // allume la couleur verte

}

void loop() {



etatbouton = digitalRead(bouton); //lecture etat BP

if (etatbouton != dernieretatbouton) {

if (!etatbouton) {//si etatbouton est différent de 1
    if (etatled) { //et que si etatled est à 1 {
        etatled = 0;//nous passons etatled à 0



       allumage = 0 ;
       etatR1 = 1;
       etatR2 = 1;
       tempoActive = 1;
       tempoDepart = millis();
      }
else
{
 
etatled = 1;//sinon nous le passons à 1
allumage = 1 ;
digitalWrite(ledVert,LOW); // eteint la couleur verte

etatR1 = 0;
etatR2 = 0;
tempoActive = 1;
tempoDepart = millis();


}}

dernieretatbouton = etatbouton;

}


digitalWrite(led, etatled);

if ( allumage ) { 



digitalWrite(pinR1,etatR1); // action sur relais filaments


 
 if ( tempoActive ) {

pwmBleu = ( millis() - tempoDepart )*255/prechauffe;
analogWrite(ledBleu,pwmBleu) ; // impulsion largeur voulue sur la broche 0 = 0% et 255 = 100% haut
digitalWrite(ledRouge,HIGH); // allumage led rouge
         
        // Et si il s'est écoulé la tempo de prechauffage
        if ( ( millis() - tempoDepart ) >= prechauffe ) {
             
                        
            digitalWrite(pinR2,etatR2); //action sur relais HT
            digitalWrite(ledRouge,LOW); //extinction led rouge
 
            // Et on désactive la temporisation
            tempoActive = 0;
        }
        }}

        else {
digitalWrite(pinR2,etatR2); //action sur relais HT
          
if ( tempoActive ) {

pwmBleu = postchauffe/( millis() - tempoDepart )*50;
analogWrite(ledBleu,pwmBleu) ; // impulsion largeur voulue sur la broche 0 = 0% et 255 = 100% haut
         
        // Et si il s'est écoulé la tempo de postchauffage
        if ( ( millis() - tempoDepart ) >= postchauffe ) {
       
                 
            digitalWrite(pinR1,etatR1); //action sur le relais filaments
 
            // Et on désactive la temporisation
            tempoActive = 0;
        }}

}

delay(200); //interruption de 200ms (anti rebond)

etatfil = digitalRead(verif);


if (etatled != 0){ //si on est en marche

while(etatfil != 0) // disparition du chauffage
{
digitalWrite(pinR2,HIGH); //coupure de la HT
etatfil = digitalRead(verif); // verification du chauffage

//Reset de la tempo
tempoActive = 1 ; 
tempoDepart = millis();
allumage = 1 ; 

// Clignotement de la LED
digitalWrite(led, LOW);
delay (500);
digitalWrite(led, HIGH);
delay (500);

}
}

else { //si on est eteint 
  
  if (etatfil !=0) { //si le filament est allumé, la LED rouge allumée 
         digitalWrite(ledRouge,LOW);
         digitalWrite(ledVert,HIGH); 
 }  else {
        digitalWrite(ledRouge,HIGH);
        digitalWrite(ledVert,LOW);
  }
}

}

        
