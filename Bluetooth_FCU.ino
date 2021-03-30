//-----Bluetooth commands-----

//m[1-4] set fire mode
//a for stats
//d[0-inf] set dwell
//s to reset shots
//r[0-inf] set rof
//b[0-inf] set burst size
//f[0-inf] set burst delay

//-----------------------------

//-------Default values--------
int dwell = 8; // how long to open solenoid ms
int rate = 25; // rate of fire
int burstDelay=30; //delay between burst shots ms
int burstSize=3; // size of each burst shot
String password="000000";

#include <SoftwareSerial.h>



char solenoid = 7; // solenoid control pin
char trigger = 6; // semi auto switch pin [[Use as selector switch]]
char fireSelector =5;

int lastTriggerState = false; // last state of trigger switch
int triggerState = true; // current state of trigger switch
int lastFireSelectorState = false; // last state of fire selector switch
int fireSelectorState = false; // state of the fire selector switch

int startPressed = 0;    // the moment the trigger was pressed
int endPressed = 0;      // the moment the trigger was released
int holdTime = 0;        // how long the trigger was hold
int idleTime = 0;        // how long the trigger was idle
int fireMode = 1;        // If the gun is full auto or semi

bool loggedIn=false;
int burstCounter=0;

bool tempbool = true;

int shotsFired = 0;      // ShotCounter


String data;
SoftwareSerial bt(2,3);
void setup()
{
  bt.begin(9600);
  bt.println("Enter password: ");
  pinMode( solenoid, OUTPUT ); // Solenoid
  pinMode( trigger, INPUT_PULLUP ); // Semi Auto
  pinMode( fireSelector, INPUT_PULLUP); //Selector button
}


void shoot()
{
  if (fireMode == 1) {
    semiAuto();
    delay(5);
  }
  else if (fireMode == 2) {
    binary();
    delay(5);

  }
  else if (fireMode == 3) {
    fullAuto();
    delay(5);

  }
  else if(fireMode == 4){
    burst();
    delay(5);

  }
}
/////////////////////////////////////////////Fire Modes/////////////////////////////////////////////

void burst(){
  if ( triggerState == false ) {
    while(burstCounter<burstSize)
    {
      oneShot();
      burstCounter+=1;
      

      delay(burstDelay);
    }
    burstCounter=0;

  }
}


void binary()
{

  oneShot();

}
void fullAuto()
{
  if ( triggerState == false ) { // switch is closed
     oneShot();

    delay( 600 / rate ); // wait for next shot

    if ( digitalRead( trigger ) == false ) // if trigger is still pulled
    {

      fullAuto(); // call function again

    }
  }
}

void semiAuto()
{
  if ( triggerState == false ) { // switch is closed
    oneShot();
  }
}
void oneShot(){
    digitalWrite( solenoid, HIGH ); // open solenoid

    delay( dwell ); // wait to clear barrel
    shotsFired = shotsFired + 1;

    digitalWrite( solenoid, LOW ); // close solenoid
}
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
    triggerState = digitalRead( trigger ); // read current status of semi switch
    if ( triggerState != lastTriggerState ) // not the same as last / start status
    {
      shoot(); // fire
    }
    
    lastTriggerState = triggerState; // set current state to current trigger state
    lastFireSelectorState = fireSelectorState; // same as above
    
  if(bt.available()){
    String in = bt.readString();
    if(in!=NULL){
      in.trim();
    }

    while(loggedIn==false){
      if(in==password){
        loggedIn=true;
        bt.println("Type J for Menu");
        return;
      }
      else{
        bt.println("Invalid password");
        break;
      }
      
    }
    if(loggedIn==true){
      
      char startDigit = in[0];
      
      if(startDigit=='R'||startDigit=='r'){
        in.remove(0,1);
        if(in.toInt()!=NULL && in.toInt()>0){
         rate=in.toInt();
         bt.println("Rof: "+(String)rate);
        }
        else
         bt.println("Invalid arguement");
      }
      
      else if(startDigit=='D'||startDigit=='d'){
        in.remove(0,1);
        if(in.toInt()!=NULL && in.toInt()>0){
         dwell=in.toInt();
         bt.println("Dwell: "+(String)dwell);
         }
        else
         bt.println("Invalid arguement");
  
      }
      else if(startDigit=='M'||startDigit=='m'){
        in.remove(0,1);
        
        if(in.toInt()!=NULL && in.toInt()>0){
          fireMode=in.toInt();
          bt.println("Firemode: "+(String)fireMode);
        }
        else
        bt.println("Invalid arguement");
      }
      else if(startDigit=='A'||startDigit=='a'){
        bt.println("-------------Stats-------------\nDwell: "+(String)dwell+"\nRate of fire: "+(String)rate+"\nBurst size: "+(String)burstSize+"\nBurst delay: "+burstDelay+"\nFire mode: "+(String)fireMode+"\nShots Fired: "+(String)shotsFired+"\n----------------------------------");
      }
      else if(startDigit=='S'||startDigit=='s'){
        shotsFired=0;
        bt.println("Shots reset!");
      }
      else if(startDigit=='B'||startDigit=='b'){
        in.remove(0,1);
        if(in.toInt()!=NULL&& in.toInt()>0){
          burstSize=in.toInt();
          bt.println("Burst size: "+(String)burstSize);
        }
        else
        bt.println("Invalid arguement");
      }
      else if(startDigit=='F'||startDigit=='f'){
        in.remove(0,1);
        if(in.toInt()!=NULL&& in.toInt()>0){
          burstDelay=in.toInt();
          bt.println("Burst delay: "+(String)burstDelay);
        }else
        bt.println("Invalid arguement");
      }
      else if(startDigit=='L'||startDigit=='l'){
        loggedIn=false;
        bt.println("Logged out");
        bt.println("Enter password: ");
        return;
      }

      else if(startDigit=='j')
      {
        bt.println("------------------Menu-----------------\nM[1-4]   -set fire mode \nD[0-inf]  -set dwell \nR[0-inf]   -set rof \nS             -to reset shots \nB[0-inf]  -set burst size \nF[0-inf]   -set burst delay\nA             -for stats\nL             -to log out\n-------------------------------------------");
      }
      else{
        bt.println("Invalid command");
      }
    }
    
  }
  
    fireSelectorState = digitalRead(fireSelector);
    if ( fireSelectorState == false ) { // switch is closed
      if (fireSelectorState != lastFireSelectorState) {
        fireMode = fireMode + 1;
        if (fireMode >= 5) {
          fireMode = 1;
        }
      }
    }
}
