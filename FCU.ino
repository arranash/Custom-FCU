int dwell = 12; // how long to open solenoid ms
int rate = 55; // rate of fire

char solenoid = 6; // solenoid control pin
char trigger = 7; // semi auto switch pin [[Use as selector switch]]
char fireSelector = 5;

int lastTriggerState = false; // last state of trigger switch
int triggerState = true; // current state of trigger switch
int lastFireSelectorState = false; // last state of fire selector switch
int fireSelectorState = false; // state of the fire selector switch

int startPressed = 0;    // the moment the trigger was pressed
int endPressed = 0;      // the moment the trigger was released
int holdTime = 0;        // how long the trigger was hold
int idleTime = 0;        // how long the trigger was idle
int fireMode = 0;        // If the gun is full auto or semi

int shotsFired = 0;      // ShotCounter
String data;

void setup()
{
  pinMode( solenoid, OUTPUT ); // Solenoid
  pinMode( trigger, INPUT_PULLUP ); // Semi Auto
  pinMode( fireSelector, INPUT_PULLUP); //Selector button
}


void shoot()
{
  if (fireMode == 0) {
    semiAuto();
  }
  else if (fireMode == 1) {
    binary();
  }
  else if (fireMode == 2) {
    fullAuto();
  }
}
/////////////////////////////////////////////Fire Modes/////////////////////////////////////////////

void binary()
{

  digitalWrite( solenoid, true ); // open solenoid

  delay( dwell ); // wait to clear barrel
  shotsFired = shotsFired + 1;
  digitalWrite( solenoid, false ); // close solenoid

}
void fullAuto()
{
  if ( triggerState == false ) { // switch is closed
    digitalWrite( solenoid, true ); // open solenoid

    delay( dwell); // wait to clear barrel

    digitalWrite( solenoid, false ); // close soleniod
    shotsFired = shotsFired + 1;

    delay( 1000 / rate ); // wait for next shot

    if ( digitalRead( trigger ) == false ) // if trigger is still pulled
    {

      fullAuto(); // call function again

    }
  }
}

void semiAuto()
{
  if ( triggerState == false ) { // switch is closed
    digitalWrite( solenoid, HIGH ); // open solenoid

    delay( dwell ); // wait to clear barrel
    shotsFired = shotsFired + 1;

    digitalWrite( solenoid, LOW ); // close solenoid
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  }
  fireSelectorState = digitalRead(fireSelector);
  if ( fireSelectorState == false ) { // switch is closed
    if (fireSelectorState != lastFireSelectorState) {
      fireMode = fireMode + 1;
      if (fireMode >= 3) {
        fireMode = 0;
      }
    }
  }


  triggerState = digitalRead( trigger ); // read current status of semi switch
  if ( triggerState != lastTriggerState ) // not the same as last / start status
  {
    shoot(); // fire
  }


  lastTriggerState = triggerState; // set current state to current trigger state
  lastFireSelectorState = fireSelectorState; // same as above
}
