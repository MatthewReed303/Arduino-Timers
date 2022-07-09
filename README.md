# Simple Timer Library based on PLC Timers
<br/>

## Timer Options

TON      = Time On Delay  
TOF      = Time Off Delay  
TP       = Pulse Timer  
FLASHER  = Flashing Timer  
DEBOUNCE = Debounce Timer

---

### Timer Description

#### TON Timer
---
The TON Timer requires EN (Enable) to be True to begin timing. The DN (Done Bit) becomes True when the Elapsed Time = Preset Time.
When EN is False the timer resets and DN becomes False. If EN becomes false during timing the Timer is reset and waits for EN.
ET (Elapsed Time) is active during timing and ET value remains when timing is complete, until the Timer is reset or started.
TT (Timing Bit) This Timing bit is true only while the timer is timing, If timer is stopped or reset TT = False. 
If RES (RESET) is True during timing or when the timer is complete, the timer will be reset.  
<br/>     
  
#### TOF Timer
---
The TOF Timer requires EN (Enable) to be True to activate the Timer. The DN (Done Bit) becomes True when the Timer is Enabled.
When EN becomes False (Detects Falling Edge) the timer will begin timing, when Elapsed Time = Preset Time then DN = False.
ET (Elapsed Time) is active during timing and ET value remains when timing is complete, until the Timer is reset or started.
TT (Timing Bit) This Timing bit is true only while the timer is timing, If timer is stopped or reset TT = False.
If RES (RESET) is True during timing or when the timer is complete, the timer will be reset.  
<br/>  
    
#### TP Timer
---
The TP Timer requires EN (Enable) to be True to begin timing. The DN (Done Bit) becomes True when EN rising edge detected.
When EN becomes False (Detects Falling Edge) while timing the timer will continue until Elapsed Time = Preset Time then DN = False.
ET (Elapsed Time) is active during timing and ET value remains when timing is complete, until the Timer is reset or started.
TT (Timing Bit) This Timing bit is true only while the timer is timing, If timer is stopped or reset TT = False.
If RES (RESET) is True during timing or when the timer is complete, the timer will be reset.  
<br/>  
  
#### FLASHER Timer
---
The FLASHER Timer requires EN (Enable) to be True to begin timing, The DN (Done Bit) is True while Preset time on timing, When time on is done,
the time off will begin timing and DN set to false for the Preset off time, when the time off is done they cycle will continue until EN = False.
If RES (RESET) is True during timing or when the timer is complete, the timer will be reset.  
<br/>  
  
#### DEBOUNCE Timer
---
The debounce timer can detect the rising and falling edge of the IN (Input) If the signal is rising and the elapsed time = preset (default 50ms)
then DN (Done) Is true and input active. If the falling edge is detected the DN will remain True untill IN is false for the preset time.  
<br/>  
  
### Base Time Format
---
The Timers base timing format can be changed by changing the BASE parameter of the Function:  
    
BASE = "Microseconds"  
BASE = "Milliseconds"  
BASE = "Seconds"  
BASE = "Minutes"  
BASE = "Hours"  
  
For example if BASE = "Seconds" the PT (Preset Time) and ET (Elapsed Time) will be converted to Seconds  

### How to use the Timers
---
The Timers are using the Arduino micros() function. Do not use delay() in your code as you may miss your times and delay is blocking code.   
First the Class has to be called and assign an object for each timer you plan to use:  
`Time Timer1;`  
"Time" is the Class and Timer1 the Object. Call this before setup and loop  
The Function within the Class can now be called in the Loop and used based on you Logic  
```cpp
Timer1.TON("Parameters HERE")  
Timer1.TOF("Parameters HERE")  
Timer1.TP("Parameters HERE") 
```  
The parameters can be used outside the function by using the class object. Do not write to the read parameters   
```cpp
Timer1.EN = 1; //Enable Timer1  
Timer1.RES = 1; //Reset Timer1  
Timer1.PT = 100; //Set Timer1 Preset  
Timer1.BASE = "Milliseconds"; //Set Timer1 to Milliseconds 
bool result_DN = Timer1.DN; //Read Timer1 Done Status  
bool result_TT = Timer1.TT; //Read Timer1 Timing Status  
unsigned long result_ET = Timer1.ET; //Read Timer1 Elapsed Time  
```  
The Parameters for the TON / TOF / TP Timers are:  
1=Enable(EN)  
2=RESET(RES)  
3=PRESET TIME(PT)  
4=Time BASE   
5=TIMER DONE(DN)  
6=TIMING ACTIVE BIT(TT)  
7=TIMER ELAPSED TIME(ET)  
```cpp
Timer1.TON(Timer1.EN, Timer1.RES, Timer1.PT = 3, Timer1.BASE = "Seconds", Timer1.DN, Timer1.TT, Timer1.ET);  
```  
The Parameters for the FLASHER Timer are:  
1=Enable(EN)  
2=RESET(RES)  
3=PRESET TIME ON(PT_ON)  
4=PRESET TIME OFF(PT_OFF)  
5=Time BASE  
6=TIMER DONE(DN)  
```cpp
Timer1.FLASHER(Timer2.EN, Timer2.RES, Timer2.PT_ON = 350, Timer2.PT_OFF = 250, Timer2.BASE = "Milliseconds", Timer2.DN);  
 ``` 
Debounce has to have an IN (Input) can be a GPIO Pin etc and returns the Input once processed Default time is 50ms  
```cpp
Time DB1 //Calling Time Class assigning DB1 Object  
bool pin1 = DB1.DEBOUNCE(digitalRead(1));       //Default 50ms  
bool pin1 = DB1.DEBOUNCE(digitalRead(1), 100);  //Using 100ms   
```  

### Timer Examples 

````cpp
#include <Arduino.h>
#include <Timer.h>

//Call the Class and Assign Object for Timer
Time Timer1; 
Time Timer2;
Time Timer3; 
//Call the Class and Assign Object for Debounce
Time DB1; 
Time DB2; 
Time DB3; 
Time DB4; 

//Analog Parameters
const int AI_01_Pin = 14; //Analog Input 1 is connected to GPIO 14 (Analog CH0) 
int AI_01_Value = 0;      //Analog Reading

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
  //Function Parameters TON/TOF/TP: 1=Enable(EN), 2=RESET(RES), 3=PRESET TIME(PT), 4=Time BASE 5=TIMER DONE(DN), 6=TIMING ACTIVE BIT(TT), 7=TIMER ELAPSED TIME(ET)
  //Function Parameters Flasher: 1=Enable(EN), 2=RESET(RES), 3=PRESET TIME ON(PT_ON), 4=PRESET TIME OFF(PT_OFF), 5=Time BASE, 6=TIMER DONE(DN)

  Timer1.TON(Timer1.EN, Timer1.RES, Timer1.PT = 3, Timer1.BASE = "Seconds", Timer1.DN, Timer1.TT, Timer1.ET); //Timer Type can be Changed Timer1.TON or Timer1.TOF or Timer1.TP
  Timer2.FLASHER(Timer2.EN, Timer2.RES, Timer2.PT_ON = 350, Timer2.PT_OFF = 250, Timer2.BASE = "Milliseconds", Timer2.DN); //FLASHER Format
  Timer3.TON(Timer3.EN, Timer3.RES, Timer3.PT = 1000, Timer3.BASE = "Milliseconds", Timer3.DN, Timer3.TT, Timer3.ET); //Timer 3 for non blocking delay
  
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

  //Example of using non blocking delay ( Take analog reading every 1000ms )
  if (!Timer3.DN) {
    Timer3.EN = 1; //Enable Timer
  }
  else {
    //Read Analog value and Print to serial monitor
    AI_01_Value = analogRead(AI_01_Pin);
    Serial.print("Analog Input 1 Value:  ");
    Serial.println(AI_01_Value);
    Timer3.EN = 0; //Disable Timer to restart cycle 
  }


}

````


