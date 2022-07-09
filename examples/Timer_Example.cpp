#include <Arduino.h>
#include <Timer.h>

//Call the Class and Assign Object for Timer
Time Timer1; 
Time Timer2; 
//Call the Class and Assign Object for Debounce
Time DB1; 
Time DB2; 
Time DB3; 
Time DB4; 

void setup()
  
{
  Serial.begin(9600);
  Serial.print("Serial Started");
  //Setup Output Pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  //Setup Input Pins
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
}

void loop()
{
  
  //Call the Timer Type Function in the Class and Return Timer Results 
  //Function Parameters: 1=Enable(EN), 2=RESET(RES), 3=PRESET TIME(PT), 4=Time BASE 5=TIMER DONE(DN), 6=TIMING ACTIVE BIT(TT), 7=TIMER ELAPSED TIME(ET)
  Timer1.TON(Timer1.EN, Timer1.RES, Timer1.PT = 3, Timer1.BASE = "Seconds", Timer1.DN, Timer1.TT, Timer1.ET); //Timer Type can be Changed Timer1.TON or Timer1.TOF or Timer1.TP
  //Function Parameters: 1=Enable(EN), 2=RESET(RES), 3=PRESET TIME ON(PT_ON), 4=PRESET TIME OFF(PT_OFF), 5=Time BASE, 6=TIMER DONE(DN)
  Timer2.FLASHER(Timer2.EN, Timer2.RES, Timer2.PT_ON = 350, Timer2.PT_OFF = 250, Timer2.BASE = "Milliseconds", Timer2.DN); //FLASHER Format
  
  //Read Inputs with debounce to prevent false trigger
  bool IO0 = DB1.DEBOUNCE(digitalRead(8));
  bool IO1 = DB2.DEBOUNCE(digitalRead(9));
  bool IO2 = DB3.DEBOUNCE(digitalRead(10));

  //Debounce Example (Debounce time can be adjusted, 100ms as example, default 50ms)
  bool pin7 = DB4.DEBOUNCE(digitalRead(7), 100);
  if ( pin7 == 1 ){
    digitalWrite(11, HIGH);
  }
  else {
    digitalWrite(11, LOW);
  }

  //TIMER Example TON / TOF / TP
  if (IO0 == 1) { //Enable Timer
    Timer1.EN = 1; 
  }
  else {
    Timer1.EN = 0;
  }

  if ( Timer1.DN == 1 ){ //Timer Done
    digitalWrite(12, HIGH);
  }
  else {
    digitalWrite(12, LOW);
  }
  
  if (IO2 == 1){ //Reset Timer
    Timer1.RES = 1;
  }
  else {
    Timer1.RES = 0;
  }

  //FLASHER Example (ON Time and OFF Time)
  IO1 = 1; //Enable Built in LED Flasher comment this out to use other logic
  if (IO1 == 1) { //Enable Timer
    Timer2.EN = 1;
  }
  else {
    Timer2.EN = 0;
  }
  
  if ( Timer2.DN == 1 ){ //Timer Done
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (IO2 == 1){ //Reset Timer
    Timer2.RES = 1;
  }
  else {
    Timer2.RES = 0;
  }


}
