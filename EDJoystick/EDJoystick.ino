// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                            (c) 2856 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: /*+T,&j<\&59    Revision: 00000002.05A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: myp9C#A*xXP7 <-Assistant(K.g\c4y\c+k?): 8;g*>!!G#7Lt H]z{ic.dq-P9 H]z{ic.dq-P9 bY*!){9G+yBP ;5,BR'g|rMQf !*5+<BPf`<q- nB{h/3T^RK-t 'm>h;~L}y{7K jrG}qY?iXj#6
// *
// ***************************************************************************************
// *  Programmer Notes:
// *
// *            Coder:  Robert Lebowski
// *    Support Coder:  Thomas Whatshisname  *need to look this up later
// *  --- Apnd:
// *    Other Sources:  https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/
// *                    Based on Comment by Goustoulos
// *
// *  Purpose:
// *  Microcontroller code for secondary thumb joystick to replace pilots tendencies
// *  to control vertical and horizontal thrust with the HAT controls.  Really, a HAT on
// *  a joystick is pathetic.  It really is nothing more than 4 buttons.  We do not need
// *  a HAT Control on a joystick.  1  HAT control on a joystick is 1 HAT control too
// *  many.  We need this thumb joystick instead and in its place.
// *
// *  Note about behavior:  The joystick has been designed for trust vectoring on a non
// *  certified component.  This code is designed to provide fine control of 0 to up to
// *  45 percent thrust in the joystick's 0 to 90 percent range from center.  Exceed the
// *  90 percent range and thrust will be at 100 percent.  These values are configurable.
// *
// *  La de da, de da.  Things have been getting slow in this department ever since the
// *  AI programmers have been installed in the system.  I want to get up and get a cup
// *  of coffee and just sit back and browse the net, but the coding manager told us to
// *  look busy during our inspection.  The Lieutenant is literally 9 meters away so I'm just
// *  sitting here typing stuff.  La de da is a reference number.
// *
// *  To be honest, I kind of like this code.  It took a few hours to put up and, although
// *  its dirty, it runs really well.  It is also, cleaner, in respects to the AI generated
// *  code.  Much more compact and even has a hint of personality that ships for CD had before.
// *  Not to mention, smaller and can be redundant on multiple systems.
// *
// ***************************************************************************************
// *
// *  V 2.05  _200124
// *    OK.  Boss calls me into the office.  Offers me some sweet brandy.  Tells me to 
// *    improve this program.  Thats it.  What the hell?  This program was scrap.  
// *    I see the censor made changes to it, but its not too difficult to see what 
// *    she/he/it did.  Except, it would be cool if the comments werentn't left out of its
// *    edits.  Also, I see no, 0000, nothing, zero, nada, pull request for this routine
// *    So, again, what the hell?
// *  
// *    - changing joy memory position from range of 0 to 1024 and a center of 512, to
// *      range of -512 to 512 with 0 center, to make calculations to joy position easier.
// *    - changed a few functions to allow me to create a second slope in the future.  
// *      This will allow me to fine tune the lower joystick positions more accurately.
// *    - Created a "booTuner" variable that helps me figure out what tuning, 
// *      breakaway, and deadzone values to use.  
// *    - CENSOR EDIT:  AS INSTRUCTED, REPORTING "A POLICE SIREN CHIRP WHEN PROGRAMMER 
// *        IGNITED A WAX CANDLE," SUGGESTING CORRUPT WATCHDOG ROUTINES AND OR INTENT AND 
// *        OR PURPOSE.  SENDING ALL CORE DATA WITH REPORT TO INTEL CORRESPONDANCE 
// *        COORDINATOR FOR DETAILED ANALYSIS AND CORRECTIONS.  
// *    - BOOKMARK CREATED:  fe*}<\~7A@ryDc)=@okfc^6yQ}_J
// *
// *
// *  V 2.04  _200120 (MODIFICATIONS MADE BY CENSOR)
// *    - ILLEGAL ACCESS DETECTED.  MULTIPLE ANONYMOUS ATTEMPTS TO OBTAIN ORIGINAL CODE
// *        DETECTED.  WATCHDOG ROUTINES INITIATED.  ACCESS TO ASSOCIATED SYSTEMS
// *        ELEVATED.  WATCHLIST CREATED.
// *    - COMPILER CORRUPTION DETECTED.  CHANGES TO CODE MADE TO ACCOMIDATE CHANGES IN
// *        COMPILER.
// *    - RESORT PROCEDURES: SUPPORT PROCEDURES AS TOP.  SETUP PROCEDURES AS MID.  MAIN
// *        PROCEDURES AS LAST.
// *    - CHANGED SEVERAL VARIABLE NAMES.
// *    - REPLACED fltBreakTune VARIABLE WITH intBreakTune VARIABLE AND REPLACE LOGIC
// *        WITH A STATIC INTEGER NUMBER BREAKAWAY NUMBER.
// *    - CONSIDERING USERPING fltPrecisionBreakaway FUNCTION AND REPLACING intSlope
// *        LOGIC WITH EXPONENTAL CURVE.
// *    - SPELLING ERROR CORRECTIONS MADE.
// *
// *  V 2.03  _200107
// *    - Note:  Boss found out I wrote this code because it was reviewed or tested somewhere.
// *                No clue as to where though.  And, that's bad because I just wrote it for
// *                myself.  Thank God security restricts his access to my source, otherwise
// *                he'd flip over the comments in here.  The sensor did fine him for
// *                me not using proper function calls to redundant routines and for not
// *                tying it into the code hub.  Crap, he was still pissed though.
// *                He told me that if I couldn't do those two simple things then I would
// *                be sent on a solo data delivery mission to Shinrata.  Only Elite ranked
// *                is allowed into that system.  I have no Pilots Federation rank.  So,
// *                yeah, he has the rank to put me out on a suicide mission.
// *    - Learned how to program functions in this silly language.
// *    - Created functions for several bits of code that was called more than one.
// *    - No real code changes.
// *    - Tied this program to the hub, I hope.
// *
// *  V 2.02  _191209
// *    - Added and rearranged source code and comments to make initial setup easier.
// *
// *  V 2.01  _191128
// *    - Added non delay loop - code then modified from Arduino Tutorial   http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
// *    - Added Blink Routine then removed.  It has stayed in comment form as an example.
// *    - Removed Delay from Timer. (Coded as Delay less Loop)
// *    - Brought Back Deadzone Code.
// *    - LED on board turns on when computed input has changed.
// *
// *  v 1.01  _19####
// *    - Created.
// ***************************************************************************************

#include "Joystick.h"
Joystick_ Joystick;

// --- Current Pin Layout ---
// PIN 6   - Joystick Button
// PIN A0  - X axis
// PIN A1  - Y axis
// PIN GND - Joystick Ground
// PIN 3.5 - Joystick Power

// --- Setup ---

// Voltage
//    Are you powering the joystick with the 5V pin or the 3.5V pin?  Pick one or the other (HARDWARE HACK)
// Voltage flow offset: 3.3V is 1.5151, 5V is 1
// w 3.3 v range is 0 = 675.84 and mid is 337.92 -note
// Calc XYMid =  338, XYMax 0 - 676              -note

//const float floVOffset = 1;       // is used for 5V
// or
const float floVOffset = 1.5151;  // is used for 3.5V

// Inverse Axis
const boolean booXInverse = false;
const boolean booYInverse = true;

// Tuning -- instead.
//    My joystick was cheap.  Midpoint was inaccurate.  While in debug I figured out how far off and then readjusted the
//    midpoint with these values
// In future, auto tune this on controller start based on first
// first read position and distance from 512.
const int intXtune = 3;
const int intYtune = 10;
const int intDeadZone = 2 ;   // Reintroducing deadzones

// Precision slope and Breakaway Point
//    Not sure how to explain this except by example.  If joystick is at halfway point of uppermost and mid position and
//    slope is 2 or (1/2), then the joystick position will be reported as at the 1/4 point of uppermost and mid position.
//    Also, if breakpoint is either max(1024 - intBreakTune) or min (0 + intBreakTune) away from the joysticks upper
//    limit, then the position will be reported as max upper limit.
//      Note: These are thruster controls.  I need either precision or full on.
// Slope is rep by 1/#.  eg, Slope 2 = 1/2 = .5, 4 = .25, ...
// Break point . 9 represents 90%
const int intSlopeTune = 2;
const int intBreakTune = 12;

// Enable Serial Monitor for testing
//    Enabling booTest will slow the board time and enable the serial monitor to be read.
const boolean booTest = false;
//    Enabling booTuner will enable serial moniter to display tuning information
const boolean booTuner = false;
//  Testing Data Collection
//  These two variables are kind of stupid.  Why cant I conditionally scope local variables at compile time.
//  Better to assign them here and not use them, than to create and destroy them with every cycle.
int fltTestAxisX = 0;
int fltTestAxisY = 0;

// Adjust joystick end to end limits value.
// doesn't work yet and never finished.
//const float floMag = .3;

// Delay less Loop Variables
//    intRestTime defines the amount of time, in milliseconds, passed before another data read pass is performed
//    and transmitted from the controller to the main system.
unsigned long tmePrevMillis = 0;
int intRestTime = 46;        // Do not check for update until rest time is passed

// --- End Setup ---

// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// // Blink Variables
int ledState = LOW;                // ledState used to set the LED

// // Generally, you should use "unsigned long" for variables that hold time
// // The value will quickly become too large for an int to store
// unsigned long previousMillis = 0;        // will store last time LED was updated

// // Blink Timer Interval
// const long interval = 1000;           // interval at which to blink (milliseconds)

// Change State Variables
int button0ValPrev = false;
float fltAxisXPrev = 0;
float fltAxisYPrev = 0;
boolean booUpdate = false;


float fltJoyTuneInverse (float fltAxisVal, int intTune, boolean booInverse)
//  This function will clean the newly read joystick value by adjusting its passed tune
//  and invert the value if requested.
{
  //  Adjust 0-1024 512 center value to 0 center. Range unchecked.
  fltAxisVal = fltAxisVal - 512;

  if (booTuner == true)
  {
      Serial.print("TUNING: Suggested Tune Val : (+-)");
      Serial.print(fltAxisVal);
  }

  if (booInverse == true)
  {
    fltAxisVal = intTune - fltAxisVal;
  }
  else
  {
    fltAxisVal = intTune + fltAxisVal;
  }

  if (booTuner == true)
  {
      Serial.print(" Reporting: ");
      Serial.print(fltAxisVal);
      Serial.println();
  }
  
  return fltAxisVal;
}


float fltPrecisionBreakaway (float fltAxisVal, int intSlope, float intBreak)
//  Now that we have a valid position of the joystick, we need to define its behavior.
//  Precision is defined before the breakaway point by the Slope Value, then
//  Breaks away at the BreakTune value.
//  Really, I stole this code from Tom, down the hall.  I peeked at how he was
//  handling the Scarab's, SRV system thing, steering mechanism.  Makes sense.
//  If a pilot is pushing the stick to its limit, he wants full power.
//  Also, this eliminates 2 problems.  I don't need to be check for values exceeding
//  limits being passed.  And, I don't' have to worry about that dang x^3 curve.  Oh,
//  the AI thinks its so smart by programming its cool curves and all.  I think
//  <REDACTED>
// **************************************************************************** \\
// * CENSOR EDIT:                                                               \\
// *   WITH COMPLIANCE TO THE DEFENDANTS’ MOTION FOR EXPEDITED CLARIFICATION    \\
// *   OR, IN THE ALTERNATIVE, MODIFICATION OF THE FEAHC (CASE:BF0N9SXCEnwvB7)  \\
// *   ORDER TO PROVIDE MORE INFORMATION REGARDING THE REDACTED STATEMENT,      \\
// *   FOLLOWS:                                                                 \\
// *      "12 lines were removed."                                              \\
// **************************************************************************** \\
//{  <-- CORRUPTION: UNEXPLAINED COMMENTED LINE
{
  if (booTuner == true)
  {
      Serial.print("  BREAKAWAY: BreakDistance: ");
      Serial.print(512 - intBreak - abs(fltAxisVal));
  }    

  if (abs(fltAxisVal) > (512 - intBreak))
  {
    fltAxisVal = ((abs(fltAxisVal) / fltAxisVal) * 512);
    
    if (booTuner == true)
    {
      Serial.print(" BRK ");
    }
    
  }
  else
  {
    fltAxisVal = (fltAxisVal / intSlope);
    
    if (booTuner == true)
    {
      Serial.print(" --- ");
    }
    
  }
  
  if (booTuner == true)
  {
      Serial.print(" Reporting: ");
      Serial.print(fltAxisVal);
      Serial.println();
  }
  
  return fltAxisVal;
}


float fltDeadZoneCheck (float fltAxisVal, int intDead)
//  If the value isn't far enough away from the deadzone, then set
//  the position to absolute center.
{
  //Serial.print(abs(intDead));
  //Serial.print("  ABSVal = ");
  //Serial.print(abs(fltAxisVal));
  //Serial.print("  Val =  ");
  //Serial.print(fltAxisVal);  // testing values
  
  if (abs(fltAxisVal) < intDead)
  {
    fltAxisVal = 0;
    if (booTuner == true)
    {
      Serial.print("    DEADZONE: Reporting: ZERO Set");
      Serial.println();
    }
  }
  else if (booTuner == true)
  { 
    Serial.print("    DEADZONE: Reporting: ");
    Serial.print(fltAxisVal);
    Serial.println();
  }
  
  //Serial.print("  Ret = ");
  //Serial.print(fltAxisVal);
  //Serial.println();
  
  return fltAxisVal;
}


float flt512Center (float fltVal)
//  Adjust -512-512 0 center value to 512 center. Range unchecked.
{
  return fltVal + 512;
}


void setup()
{
  // put your setup code here, to run once:

  if ((booTest == true) || (booTuner == true))
  {
    Serial.begin(9600);
  }

  Joystick.begin();
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);

  pinMode(6, INPUT_PULLUP);

  // Unused Pins
  //  I want more buttons.  I really dont have enough buttons on the joystick.
  //  These are placeholders.
  //pinMode(7, INPUT_PULLUP);
  //pinMode(8, INPUT_PULLUP);
  //pinMode(9, INPUT_PULLUP);
  //pinMode(10, INPUT_PULLUP);
  //pinMode(11, INPUT_PULLUP);
  //pinMode(12, INPUT_PULLUP);
  //pinMode(13, INPUT_PULLUP);

  int lastButtonState = 0;

  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
}


void loop()
{
  //  I like the idea of blinking lights, but it would require more coding to make it valuable.
  //
  //  // check to see if it's time to blink the LED; that is, if the difference
  //  // between the current time and last time you blinked the LED is bigger than
  //  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  //
  //  if (currentMillis - previousMillis >= interval)
  //  {
  //    // save the last time you blinked the LED
  //    previousMillis = currentMillis;
  //
  //    // if the LED is off turn it on and vice-versa:
  //    if (ledState == LOW) {
  //      ledState = HIGH;
  //    } else {
  //      ledState = LOW;
  //    }
  //  }
  //
  //  // set the LED with the ledState of the variable:
  //  digitalWrite(ledPin, ledState);

  // Begin of Delay less Loop
  if (currentMillis - tmePrevMillis >= intRestTime)
  {
    tmePrevMillis = currentMillis;
    // Grabbing Data ---

    int button0Val = digitalRead(6);

    //int button1Val =digitalRead(7);
    //int button2Val =digitalRead(8);
    //int button3Val =digitalRead(9);
    //int button4Val =digitalRead(10);
    //int button5Val =digitalRead(11);
    //int button6Val =digitalRead(12);
    //int button7Val =digitalRead(13);

    int xAxis = analogRead(A0);
    int yAxis = analogRead(A1);

    //  Testing Data Collection
    if (booTest == true)
    {
      fltTestAxisX = xAxis;
      fltTestAxisY = yAxis;
    }

    // Calculations ---
    // should I float these?  Yeah, why not.
    float fltAxisX = int(floVOffset * xAxis);
    float fltAxisY = int(floVOffset * yAxis);

    // Inversion with Tuning
    fltAxisX = fltJoyTuneInverse (fltAxisX, intXtune, booXInverse);
    fltAxisY = fltJoyTuneInverse (fltAxisY, intYtune, booYInverse);

    // Post Processing
    // Precision and Breakaway.
    fltAxisX = fltPrecisionBreakaway (fltAxisX, intSlopeTune, intBreakTune);
    fltAxisY = fltPrecisionBreakaway (fltAxisY, intSlopeTune, intBreakTune);

    // Deadzone Check
    fltAxisX = fltDeadZoneCheck(fltAxisX, intDeadZone);
    fltAxisY = fltDeadZoneCheck(fltAxisY, intDeadZone);

    // Reset to 512 Center before updates sent.
    fltAxisX = flt512Center(fltAxisX);
    fltAxisY = flt512Center(fltAxisY);

    // Execution and Send Updates only when value is changed ---
    // Considering, we are now getting to the point where external code is called,
    // I am not going to sub container the next portion of the code.  Real programmers
    // know why.
    if (fltAxisX != fltAxisXPrev)
    {
      Joystick.setXAxis(int(fltAxisX));
      fltAxisXPrev = fltAxisX;
      booUpdate = true;
    }

    if (fltAxisY != fltAxisYPrev)
    {
      Joystick.setYAxis(int(fltAxisY));
      fltAxisYPrev = fltAxisY;
      booUpdate = true;
    }

    if (button0Val != button0ValPrev)
    {
      Joystick.setButton(0, !button0Val);
      button0ValPrev = button0Val;
      booUpdate = true;
    }

    // Update LED to indicate data was transferred.  This will help perceive when unknow drift occurs
    // as well as other things.
    if (booUpdate == true)
    {
      digitalWrite(ledPin, HIGH);
    }
    else
    {
      digitalWrite(ledPin, LOW);
    }
    booUpdate = false;

    //  PlaceHolders for Unused Unneeded Unwanted Unloved Code
    //Joystick.setButton(1, !button1Val);
    //Joystick.setButton(2, !button2Val);
    //Joystick.setButton(3, !button3Val);
    //Joystick.setButton(4, !button4Val);
    //Joystick.setButton(5, !button5Val);
    //Joystick.setButton(6, !button6Val);
    //Joystick.setButton(7, !button7Val);

    // Testing Output
    if (booTest == true)
    {
      Serial.println();
      Serial.print("DEBUG:");
      Serial.println();
      Serial.print("    raw X: ");
      Serial.print(fltTestAxisX);
      Serial.print("        raw Y: ");
      Serial.print(fltTestAxisY);
      Serial.println();
      Serial.print("Sending X: ");
      Serial.print(fltAxisX);
      Serial.print(" Sending Y: ");
      Serial.print(fltAxisY);
      Serial.println();
      Serial.println();

      // Serial.print(27,BYTE); // clear screen - Not sure how to print escape sequences.
    }

    // Slow delay if in testing mode.
    if ((booTest == true) || (booTuner == true))
    {
      intRestTime = 250;
    }
  }   // End Delay less Loop
}

// End of main loop
// Start of all supporting functions


//  Closing Notes:  Codes a mess but it works.  I got another idea while writing it.  I
//  am going to issue a request to the AI Programmer System to see if it can read data
//  from the ship global sensor array system and convert that data into environmental sound.
//  Its a win win.  If it can't, It will prove the AI system as a sham.  If it can, I will
//  impress that cute little test pilot I see from time to time.  She is the best.
