#include <Keypad.h>


int speakerPin = 12; // speaker connected to digital pin 12

const byte ROWS = 4; //four rows (four fingers)
const byte COLS = 2; //four columns (two hands)

char keys[ROWS][COLS] = {
{'f','j'},//left hand 1st finger and right hand 1st finger
{'d','k'},//left hand 2nd finger and right hand 2nd finger
{'s','l'},//left hand 3rd finger and right hand 3rd finger
{'a',';'}//left hand 4th finger and right hand 4th finger
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){

Serial.begin (9600);

pinMode(speakerPin, OUTPUT); // sets the speakerPin to be an output

}

void loop(){
char key = keypad.getKey();

int duration = 100;
if (key != NO_KEY){
Serial.println(key);

if (key=='a'){ tone(speakerPin,2093,duration);}//tone(speakerPin,frequency,duration)
if (key=='s'){ tone(speakerPin,2349,duration);}
if (key=='d'){ tone(speakerPin,2637,duration);}
if (key=='f'){ tone(speakerPin,2793,duration);}
if (key=='j'){ tone(speakerPin,3136,duration);}
if (key=='k'){ tone(speakerPin,3520,duration);}
if (key=='l'){ tone(speakerPin,3951,duration);}
if (key==';'){ tone(speakerPin,4186,duration);}

//alternative code--just uncomment this and beep function below it
/*if (key=='a'){ beep(speakerPin,2093,100);}
if (key=='s'){ beep(speakerPin,2349,100);}
if (key=='d'){ beep(speakerPin,2637,100);}
if (key=='f'){ beep(speakerPin,2793,100);}
if (key=='j'){ beep(speakerPin,3136,100);}
if (key=='k'){ beep(speakerPin,3520,100);}
if (key=='l'){ beep(speakerPin,3951,100);}
if (key==';'){ beep(speakerPin,4186,100);}
if (key==’i’){ beep(speakerPin,2093,100);}
if (key==’j’){ beep(speakerPin,2349,100);}
if (key==’k’){ beep(speakerPin,2637,100);}
if (key==’l’){ beep(speakerPin,2793,100);}
if (key==’m’){ beep(speakerPin,3136,100);}
if (key==’n’){ beep(speakerPin,3520,100);}
if (key==’o’){ beep(speakerPin,3951,100);}
if (key==’p’){ beep(speakerPin,4186,100);}*/
}
}

/*void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds){ // the sound producing function
int x;
long delayAmount = (long)(1000000/frequencyInHertz);
long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
for (x=0;x<loopTime;x++)
{
digitalWrite(speakerPin,HIGH);
delayMicroseconds(delayAmount);
digitalWrite(speakerPin,LOW);
delayMicroseconds(delayAmount);
}
}*/
