/*
  The MIT License (MIT)
  Copyright (c) 2022 Matthew Reed
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Please see the README and Examples on how to use
*/

#ifndef Timer
#define Timer

#include "Arduino.h"

class Time{
public:

//Function in the Class pass by Reference
//TON Timer
void TON(bool EN, bool RES, unsigned long PT, String BASE, bool &DN, bool &TT, unsigned long &ET);
//TOF Timer
void TOF(bool EN, bool RES, unsigned long PT, String BASE, bool &DN, bool &TT, unsigned long &ET);
//TP Timer
void TP(bool EN, bool RES, unsigned long PT, String BASE, bool &DN, bool &TT, unsigned long &ET);
//Flasher Timer
void FLASHER(bool EN, bool RES, unsigned long PT_ON, unsigned long PT_OFF, String BASE, bool &DN);
//Input Debounce Timer Default 50ms
bool DEBOUNCE(bool IN = 0, unsigned long PT = 50);

//Timer Parameters
bool EN;     //Enable Timer 
bool RES;    //Reset Timer 
bool DN;     //Timer DONE
bool TT;     //Timer Timing Bit
bool IN;     //Input Into Function
String BASE; //Timer Base Format
unsigned long ET; //Elapsed Time
unsigned long PT;  //Set Preset Time
unsigned long PT_ON;  //Set Preset Time
unsigned long PT_OFF;  //Set Preset OFF Time


private:
//Time Base Format Function
unsigned long TimerFormatting(String FORMAT);
//ACC Time Function
unsigned long AccTime(bool EN);

//Acc Variables
unsigned long accValue;
unsigned long previousMicros;
unsigned long currentMicros;

//TON Timer Function
unsigned long TONaccValue;
bool TONTimerState;

//TOF Timer Function
unsigned long TOFaccValue;
bool TOFTimerState = 0;
bool TOFLastTimerState = 0;
bool TOFTimerStart = 0;

//TP Timer Function
unsigned long TPaccValue;
bool TPTimerState = 0;
bool TPLastTimerState = 0;
bool TPTimerStart = 0;

//Flasher Timer Function
unsigned long FLASHaccValue;

//Debounce Timer Function
unsigned long DBaccValue;
bool DbState = 0;
bool LastDbState = 0;
bool RisingEdge = 0;
bool FallingEdge = 0;

};


#endif