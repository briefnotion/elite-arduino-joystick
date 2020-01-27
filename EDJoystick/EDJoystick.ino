// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2857 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: /*+T,&j<\&59    Revision: 00000002.05A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: myp9C#A*xXP7
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
// *  V 2.06  _200127
// *    - A division by zero error was allowed in the newly created sign(float fltX)
// *      function.  It never was triggered but patched it regardless.
// *    - Created selftuner to run on start.
// *
// *
// *  V 2.06 _200126
// *    "Manipulate him to gain his trust."  Oxymoron, moronic, and moron is what this
// *    says.  But, we will get back to this, later.  Right now there is more brass
// *    floating around the office than I am comfortable with.  From both the Federation
// *    and Empire.
// *    I used this opportunity to talk to the test pilot.  Couldn't understand why
// *    someone who is in the void more than I am in a cabin, could be so shy, so I
// *    looked into it.  Personel Officer outranks me by a mile, but, I know he has debt.
// *    I pressured him to disclose her file.  He was pissed but did so by standard outing
// *    her file to voice.  At first I was shocked then later bored.  I sat and listened
// *    for nearly an hour as it constantly rambled off every single detail of every
// *    mission she has ever flown.  She is military with histories of combat,
// *    negotiations, rescue, relief, diplomacy, development, and so much more that I
// *    both understood and couln't.  Too much.  She isn't shy, she is guarded.  I letft.
// *    The bastard SO didn't set up a way to stop the playback.  I didn't even get
// *    through a decade of her history.
// *    Back to the brass, writing this is what they wanted.  "Nobody can see comments,"
// *    I said.  Death stares is all I got.  Will comply.
// *
// *    - Second Slope added.
// *    - Ploperly defining joystick construct.
// *    - Moved the tmeCurrentMillis variable into the main scope.
// *
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
// *    - BOOKMARK CREATED:  fe*}<\~7A@ryDc)=@okfc^6yQ}_J
// *
// *
// *  V 2.04  _200120 (MODIFICATIONS MADE BY CENSOR)
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
Joystick_ Joystick
(
  JOYSTICK_DEFAULT_REPORT_ID,   // default uint8_t hidReportId. Do not use 0x01 or 0x02
  JOYSTICK_TYPE_JOYSTICK,   // options: JOYSTICK_TYPE_JOYSTICK, JOYSTICK_TYPE_GAMEPAD, JOYSTICK_TYPE_MULTI_AXIS
  1,    // uint8_t buttonCount - button count
  0, // uint8_t hatSwitchCount
  true, // bool includeXAxis
  true, // bool includeYAxis
  false, // bool includeZAxis
  false, // bool includeRxAxis
  false, // bool includeRyAxis
  false,    // bool includeRzAxis
  false, // bool includeRudder
  false, // bool includeThrottle
  false, // bool includeAccelerator
  false, // bool includeBrake
  false // bool includeSteering
);

// --- Current Pin Layout ---
// PIN 6   - Joystick Button
// PIN A0  - X axis
// PIN A1  - Y axis
// PIN GND - Joystick Ground
// PIN 3.5 - Joystick Power

// --- Setup ---

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

//  Initialize with self tuner.
boolean booSelfTunerRun = true;

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
int intXtune = 0;
int intYtune = 0;
float fltXtuneMinError = 0;
float fltYtuneMinError = 0;
float fltXtuneMaxError = 0;
float fltYtuneMaxError = 0;
int intDeadZone = 0 ;   // Reintroducing deadzones

// Precision slope, controlled slope, and Breakaway Point
//    Not sure how to explain this except by example.  If joystick is at halfway point of uppermost and mid position and
//    slope is 2 or (1/2), then the joystick position will be reported as at the 1/4 point of uppermost and mid position.
//    Also, if breakpoint is either max(1024 - intBreakTune) or min (0 + intBreakTune) away from the joysticks upper
//    limit, then the position will be reported as max upper limit.
//      Note: These are thruster controls.  I need either precision or full on.
// Slope is rep by 1/#.  eg, Slope 2 = 1/2 = .5, 4 = .25, ...
// Break point . 9 represents 90%
const int intSlopeTune1 = 4;
const int intSlopeBreak1 = int(512 / 2);
const int intSlopeTune2 = 2;
const int intBreakTune = 12;


// Adjust joystick end to end limits value.
// doesn't work yet and never finished.
//const float floMag = .3;

// Delay less Loop Variables
//    intRestTime defines the amount of time, in milliseconds, passed before another data read pass is performed
//    and transmitted from the controller to the main system.
unsigned long tmeCurrentMillis = millis();  // 1 Second = 1000 millis
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


// Not going to use.
//boolean booDebug(boolean booOn, char strOutput[], boolean booPrintLn)
////  Debug output routin.
//  if (booOn == true)
//  {
//    Serial.print(strOutput);
//  }
//  if (booPrintLn == true)
//  {
//    Serial.println();
//  }
//}


float sign(float fltX)
{
  if (fltX == 0)
  {
    return 1;
  }
  else
  {
    return (fltX / abs(fltX));
  }
}

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
    fltAxisVal = 0 - intTune - fltAxisVal;
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


float fltPrecisionBreakaway (float fltAxisVal, int intSlope1, int intBreak1, int intSlope2, float intBreakA)
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

{
  if (booTuner == true)
  {
    Serial.print("  BREAKAWAY: BreakDistance: ");
    Serial.print(512 - intBreakA - abs(fltAxisVal));
  }

  if ((abs(fltAxisVal) >= 0) && (abs(fltAxisVal)) < intBreak1)
  {
    // Slope 1
    fltAxisVal = (fltAxisVal / intSlope1);


    if (booTuner == true)
    {
      Serial.print(" SL1 ");
      Serial.println();
    }
  }
  else if ((abs(fltAxisVal) >= intBreak1) && (abs(fltAxisVal) < 512 - intBreakA))
  {
    // Slope 2
    fltAxisVal = (((fltAxisVal) / intSlope2) - (sign(fltAxisVal) * (intBreak1 / intSlope1)));

    if (booTuner == true)
    {
      Serial.print(" SL2 ");
      Serial.println();
    }
  }
  else
  {
    // Break Away
    fltAxisVal = (sign(fltAxisVal) * 512);

    if (booTuner == true)
    {
      Serial.print(" BRK ");
      Serial.println();
    }
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
      Serial.print("    DEADZONE: Size: ");
      Serial.print(intDeadZone);
      Serial.print("  Reporting: ZERO Set");
      Serial.println();
    }
  }
  else if (booTuner == true)
  {
    Serial.print("    DEADZONE: Size: ");
    Serial.print(intDeadZone);
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


boolean booSelfTuner (float fltX, float fltY)
{
  //  I discovered that changes in atmospheric pressures, gravity, humidity,
  //  and temperature could and would change the tuning values, causing the
  //  zero center to drift.  This routine will adjust the automaticlly on
  //  start.

  float fltXError = 0;
  float fltYError = 0;

  if (tmeCurrentMillis <= 3000)
  {

    fltXError = 512 - fltX;
    fltYError = 512 - fltY;

    if (fltXtuneMaxError == 0)
    {
      fltXtuneMaxError = fltXError;
      fltXtuneMinError = fltXError;
      fltYtuneMaxError = fltYError;
      fltYtuneMinError = fltYError;
    }

    // X Test
    if (fltXError > fltXtuneMaxError)
    {
      fltXtuneMaxError = fltXError;
    }

    if (fltXError < fltXtuneMinError)
    {
      fltXtuneMinError = fltXError;
    }

    // Y Test
    if (fltYError > fltYtuneMaxError)
    {
      fltYtuneMaxError = fltYError;
    }

    if (fltYError < fltYtuneMinError)
    {
      fltYtuneMinError = fltYError;
    }

    // Set Tunes
    intXtune = int(fltXtuneMinError + ((fltXtuneMaxError - fltXtuneMinError) / 2));
    intYtune = int(fltYtuneMinError + ((fltYtuneMaxError - fltYtuneMinError) / 2));

    // Set DeadZone
    if ((fltXtuneMaxError - fltXtuneMinError) > intDeadZone)
    {
      intDeadZone = fltXtuneMaxError - fltXtuneMinError + 1;
    }
    if ((fltYtuneMaxError - fltYtuneMinError) > intDeadZone)
    {
      intDeadZone = fltYtuneMaxError - fltYtuneMinError + 1;
    }

    // Output
    if (booTuner == true)
    {
      Serial.println();
      Serial.print("Self Tuning:  ");
      Serial.println();

      Serial.print("  X: ");
      Serial.print(fltX);
      Serial.print("   Error: ");
      Serial.print(fltXError);
      Serial.print("   Min: ");
      Serial.print(fltXtuneMinError);
      Serial.print("   Max: ");
      Serial.print(fltXtuneMaxError);
      Serial.print("   XTune: ");
      Serial.print(intXtune);
      Serial.println();

      Serial.print("  Y: ");
      Serial.print(fltY);
      Serial.print("   Error: ");
      Serial.print(fltYError);
      Serial.print("   Min: ");
      Serial.print(fltYtuneMinError);
      Serial.print("   Max: ");
      Serial.print(fltYtuneMaxError);
      Serial.print("   YTune: ");
      Serial.print(intYtune);
      Serial.println();

      Serial.println();
    }

    return true;
  }
  else
  {
    if (booTuner == true)
    {
      Serial.print("Exiting Self Tuning");
      Serial.println();
      Serial.println();
    }
    return false;
  }
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
  tmeCurrentMillis = millis();

  // Begin of Delay less Loop
  if (tmeCurrentMillis - tmePrevMillis >= intRestTime)
  {
    tmePrevMillis = tmeCurrentMillis;

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

    //  Check Tuner to see if it needs to run.
    if (booSelfTunerRun == true)
    {
      booSelfTunerRun = booSelfTuner(fltAxisX, fltAxisY);
    }

    // Inversion with Tuning
    fltAxisX = fltJoyTuneInverse (fltAxisX, intXtune, booXInverse);
    fltAxisY = fltJoyTuneInverse (fltAxisY, intYtune, booYInverse);

    // Post Processing
    // Precision and Breakaway.
    fltAxisX = fltPrecisionBreakaway (fltAxisX, intSlopeTune1, intSlopeBreak1, intSlopeTune2, intBreakTune);
    fltAxisY = fltPrecisionBreakaway (fltAxisY, intSlopeTune1, intSlopeBreak1, intSlopeTune2, intBreakTune);

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
