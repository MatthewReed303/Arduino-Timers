#include "Arduino.h"
#include "Timer.h"

unsigned long Time::TimerFormatting(String Format){
unsigned long FormatValue;

    if (Format == "Microseconds"){
        FormatValue = 1;
    }
    else if (Format == "Milliseconds"){
        FormatValue = 1000;
    }
    else if (Format == "Seconds"){ 
        FormatValue = 1000000;
    }
    else if (Format == "Minutes"){ 
        FormatValue = 60000000;
    }
    else if (Format == "Hours"){
        FormatValue = 3600000000;
    }
return FormatValue;
}

//Function to Calculate Accumulated Time EN = Enable begin timing else Acc time = 0
unsigned long Time::AccTime(bool EN){
    currentMicros = micros();
    if (EN == 1){
        accValue = accValue + currentMicros - previousMicros;
    }
    else{
        accValue = 0;
    }
    previousMicros = currentMicros;
return accValue;
}

//Timer TON Function Returns Done Bit when Elapsed Time = Preset Time
void Time::TON(bool EN, bool RES, unsigned long PT, String BASE, bool &DN, bool &TT, unsigned long &ET){ //Returning Data using Reference

unsigned long BASE_Num = TimerFormatting(BASE); //Timer Base Settings Milis,Seconds etc
unsigned long PT_Calc = PT * BASE_Num;          //Convert Preset time to the base time offset
  
if (EN == 1) { //Timer Enabled
    if (EN != TONTimerState) { //Detect Enable Rising Edge Change and Reset ET
        TONaccValue = 0;
    }
    if (DN == 0) {
        TONaccValue = AccTime(1);
    } 
    if (TONaccValue >= PT_Calc) { //Time is done activate Done (DN) bit
        TONaccValue = PT_Calc;
        AccTime(0);
        DN = 1;
    }
    if ((TONaccValue > 0) && (TONaccValue <= PT_Calc)) { //Calculate Elapsed Time
        ET = TONaccValue / BASE_Num;
    }
    else {
        ET = 0;
    }

    if ((TONaccValue > 0) && (TONaccValue < PT_Calc)) { //Set Timing Bit while timing active
        TT = 1;
    }
    else {
        TT = 0;
    }  
}
else {
    AccTime(0);
    DN = 0;
}
  
if (RES == 1) { //Reset Timer
    AccTime(0);
    TONaccValue = 0;
    DN = 0;
    TT = 0;
    ET = 0;
}
TONTimerState = EN;
}


//Timer TOF Function Returns Done Bit when Enable goes Low and disables Done bit when time is done
void Time::TOF(bool EN, bool RES, unsigned long PT, String BASE, bool &DN, bool &TT, unsigned long &ET){ //Returning Data using Reference

unsigned long BASE_Num = TimerFormatting(BASE); //Timer Base Settings Milis,Seconds etc
unsigned long PT_Calc = PT * BASE_Num;          //Convert Preset time to the base time offset
  
if (EN == 1) { 
    TOFTimerState = 1;
    DN = 1;
}
else {
    TOFTimerState = 0; 
}

if (TOFTimerState != TOFLastTimerState) { //Check For Change in timer Enable
    if (TOFTimerState == 0) { //Detect Falling Edge to start Timer
        TOFTimerStart = 1; //Start The Timer
    }
    else {
        TOFTimerStart = 0; //Stop The Timer
        TOFaccValue = 0;
        AccTime(0);
    }
}

if (TOFTimerStart == 1) {
    TOFaccValue = AccTime(1);
}
else {
    AccTime(0);    
}

if (TOFaccValue >= PT_Calc) { //Time is done Switch Done Bit Off
    TOFaccValue = PT_Calc;
    TOFTimerStart = 0;
    AccTime(0); //Reset Acc Time
    DN = 0;
}

if ((TOFaccValue > 0) && (TOFaccValue <= PT_Calc)) { //Calculate Elapsed Time
    ET = TOFaccValue / BASE_Num;
}
else {
    ET = 0;
}

if ((TOFaccValue > 0) && (TOFaccValue < PT_Calc)) { //Set Timing Bit while timing active
    TT = 1;
}
else {
    TT = 0;
}
  
if (RES == 1) { //Reset Timer
    AccTime(0);
    TOFTimerStart = 0;
    TOFaccValue = 0;
    DN = 0;
    TT = 0;
    ET = 0;
}
TOFLastTimerState = TOFTimerState; 
}


//Timer TP Function Returns Done Bit when Enable Rising Edge Detected and stays enabled when EN goes Low, when time is done, DN bit turns off
void Time::TP(bool EN, bool RES, unsigned long PT, String BASE, bool &DN, bool &TT, unsigned long &ET){ //Returning Data using Reference

unsigned long BASE_Num = TimerFormatting(BASE); //Timer Base Settings Milis,Seconds etc
unsigned long PT_Calc = PT * BASE_Num;          //Convert Preset time to the base time offset
  
if (EN == 1) { 
    TPTimerState = 1;
}
else {
    TPTimerState = 0; 
}

if (TPTimerState != TPLastTimerState) { //Check For Change in timer Enable
    if (TPTimerState == 1) { //Detect Rising Edge to start Timer
        TPTimerStart = 1; //Start The Timer
        AccTime(0); //Make sure we start timer from 0
    }
    else if (DN == 0) { //Detect Falling edge and if Timer Running do not switch off
        TPTimerStart = 0;
        AccTime(0);
    }
    else if (TPTimerStart == 0) {
        AccTime(0);
    }
}

if (TPTimerStart == 1) {
    TPaccValue = AccTime(1); //Return Acc Value
    DN = 1;
}
else {
    TPTimerStart = 0;
}

if (TPaccValue >= PT_Calc) { //Time is done Switch Done Bit Off
    TPaccValue = PT_Calc;
    AccTime(0); //Reset Acc Time
    TPTimerStart = 0;
    DN = 0;
}

if ((TPaccValue > 0) && (TPaccValue <= PT_Calc)) { //Calculate Elapsed Time
    ET = TPaccValue / BASE_Num;
}
else {
    ET = 0;
}

if ((TPaccValue > 0) && (TPaccValue < PT_Calc)) { //Set Timing Bit while timing active
    TT = 1;
}
else {
    TT = 0;
}
  
if (RES == 1) { //Reset Timer
    AccTime(0);
    TPaccValue = 0;
    TPTimerStart = 0;
    DN = 0;
    TT = 0;
    ET = 0;
}
TPLastTimerState = TPTimerState;
}


//Timer FLASHER Function Returns Done Bit when Elapsed Time = Preset Time ON Time and Done bit off for Preset Off time
void Time::FLASHER(bool EN, bool RES, unsigned long PT_ON, unsigned long PT_OFF, String BASE, bool &DN){ //Returning Data using Reference

unsigned long BASE_Num = TimerFormatting(BASE); //Timer Base Settings Milis,Seconds etc
unsigned long PT_Calc_ON = PT_ON * BASE_Num;    //Convert Preset time to the base time offset
unsigned long PT_Calc_OFF = PT_OFF * BASE_Num;  //Convert Preset time to the base time offset
  
if (EN == 1) { //Timer Enabled
    FLASHaccValue = AccTime(1); 
}
else {
    FLASHaccValue = 0; //Set Timer Off
    AccTime(0);
    DN = 0;
}

if ((FLASHaccValue > 0) && (FLASHaccValue < PT_Calc_ON)) { //Set Timing Bit while timing active
    DN = 1;
}
else if ((FLASHaccValue > PT_Calc_ON) && (FLASHaccValue < (PT_Calc_ON + PT_Calc_OFF))) { //Offset On time and Off time and continue timing
    DN = 0;
}
else if (DN == 0) { //Reset the cycle and start again
    AccTime(0);
    FLASHaccValue = 0;
}
  
if (RES == 1) { //Reset Timer
    AccTime(0);
    FLASHaccValue = 0;
    DN = 0;
}
}

//Simple Debounce function Time Delay Input has to be true for preset time
bool Time::DEBOUNCE(bool IN, unsigned long PT){ 

if (IN == 1){
    DbState = 1;
}
else {
    DbState = 0;
}

if (DbState != LastDbState) { //Check For Change on the input
    if (DbState == 1) { //Detect Rising Edge
        FallingEdge = 0;
        RisingEdge = 1;
        DBaccValue = 0;
        AccTime(0);
    }
    else if (DbState == 0) { //Detect Falling Edge
        FallingEdge = 1;
        RisingEdge = 0;
        DBaccValue = 0;
        AccTime(0);
    }
}

if (RisingEdge == 1) {
    if (DN == 0) {
        DBaccValue = AccTime(1);
        FallingEdge = 0; 

        if (DBaccValue >= (PT * 1000)) { //Time is done activate Done (DN) bit
            DBaccValue = (PT * 1000);
            AccTime(0);
            DN = 1;
            RisingEdge = 0;
            FallingEdge = 0;
        }  
    }
    
}
if (FallingEdge == 1) {
    if (DN == 1) {
        DBaccValue = AccTime(1);
        RisingEdge = 0;

        if (DBaccValue >= (PT * 1000)) { //Time is done disable Done (DN) bit
            DBaccValue = (PT * 1000);
            AccTime(0);
            DN = 0;
            FallingEdge = 0;
            RisingEdge = 0;
        }
    }  
}
LastDbState = DbState;
return DN;
}