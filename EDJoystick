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
// *  PROJECTID: (#UNDEFINED)    Revision: 00000002.01A    
// *  TEST CODE:                 QACODE: A565              CENSORCODE: P\6#!~c>efr7
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
// *  look busy during our inspection.  The Lieutenant is litery 9 meters away so I'm just 
// *  sitting here typing stuff.  La de da is a reference number.
// *  
// *  To be honest, I kinda like this code.  It took a few hours to put up and, although 
// *  its dirty, it runs really well.  It is also, cleaner, in respects to the AI generated 
// *  code.  Much more compact and even has a hint of personality that ships for CD had before.  
// *  Not to mention, smaller and can be redundant on multiple systems.
// *  
// ***************************************************************************************
// *  V 2.01  _191128
// *    - Added non delay loop - code then modified from Arudino Tutorial   http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
// *    - Added Blink Routine then removed.  It has stayed in comment form as an example.
// *    - Removed Delay from Timer. (Codeded as Delayless Loop)
// *    - Brought Back Deadzone Code.
// *    - LED on board turns on when computed input has changed.
// *  
// *  v 1.01  _19####
// *    - Created.
// ***************************************************************************************

#include "Joystick.h"
Joystick_ Joystick;
  
  // Vars -----

  // Enable Serial Monitor for testing
  const boolean booTest = false;
  //  Testing Data Collection
  //  These two variables are kinda stupid.  Why cant I conditionally scope local vaibles at compile time.
  //  Better to assign them here and not use them, than to create and destroy them with every cycle.
  int tstxAxis = 0;
  int tstyAxis = 0;

  // Inverse Axis
  const boolean booXInverse = false;
  const boolean booYInverse = true;

  // Tuning -- instead.
  // In futuer, auto tune this on controller start based on first
  // first read position and distance from 512.
  const int intXtune = 6;
  const int intYtune = 7;
  const int intDeadZone = 2 ;   // Reentroducing deadzones

  // Precision slope and Breakaway Point
  // Slope is rep by 1/#.  eg, Slope 2 = 1/2 = .5, 4 = .25, ...
  // Break point . 9 represents 90%
  const int intSlopeTune = 2;
  const float intBreakTune = .98;

  //  HARDWARE HACK
  // Voltage flow offset: 3.3V is 1.5151, 5V is 1
  // w 3.3 v range is 0 = 675.84 and mid is 337.92 -note
  // Calc XYMid =  338, XYMax 0 - 676              -note
  const float floVOffset = 1.5151; 

  // Adjust joystick end to end limits value.
  // doesnt work yet and never finished.
  //const float floMag = .3; 

  // constants won't change. Used here to set a pin number:
  const int ledPin =  LED_BUILTIN;// the number of the LED pin

  // Delayless Loop Variables
  unsigned long tmePrevMillis = 0;   
  int intRestTime = 46;        // Do not check for update until resttime is passed

  // // Blink Variables
  int ledState = LOW;                // ledState used to set the LED

  // // Generally, you should use "unsigned long" for variables that hold time
  // // The value will quickly become too large for an int to store
  // unsigned long previousMillis = 0;        // will store last time LED was updated

  // // Blink Timer Interval
  // const long interval = 1000;           // interval at which to blink (milliseconds)

  // Change State Variables
  int button0ValPrev = false;
  float joyxValPrev = 0;
  float joyyValPrev = 0;
  boolean booUpdate = false;

  void setup() {
  // put your setup code here, to run once:

  if (booTest == true)
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
  //  I like the idea of blinking lights, but it would require more coding to make it valuble.
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

  // Begin of Delayless Loop
  if (currentMillis - tmePrevMillis >= intRestTime) 
  {  
    tmePrevMillis = currentMillis;
    // Grabbing Data ---
    
    int button0Val =digitalRead(6);
    
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
      tstxAxis = xAxis;
      tstyAxis = yAxis;
    }
  
    // Calculations ---
    // should I float these?  Yeah, why not.
    float joyxVal = int(floVOffset * xAxis);
    float joyyVal = int(floVOffset * yAxis);
  
    // Inversion with Tuning
    if (booXInverse == true)
    {
      joyxVal = 1024 - joyxVal + intXtune;
    }
    else
    {
      joyxVal = joyxVal + intXtune;
    }
    
    if (booYInverse == true)
    {
      joyyVal = 1024 - joyyVal + intYtune;
    }
    else
    {
      joyyVal = joyyVal  + intYtune;
    }
  
    // Post Processing
    // Precision and Breakaway.   !!!Yes, I can use functions but im lazy.
  
    // X ***********
    if (joyxVal > 512)
    {
      if (joyxVal < (1024 * intBreakTune))
      {
        joyxVal = (((joyxVal - 512) / intSlopeTune) + 512);
      }
      else
      {
        joyxVal = 1024;
      }
    }
    else //if (joyxVal < 512)
    {
      if (joyxVal > (1024 - (1024 * intBreakTune)))
      {
        joyxVal = (((joyxVal - 512) / intSlopeTune) + 512);
      }
      else
      {
        joyxVal = 0;
      }
    }
    
    // Y *********
    if (joyyVal > 512)
    {
      if (joyyVal < (1024 * intBreakTune))
      {
        joyyVal = (((joyyVal - 512) / intSlopeTune) + 512);
      }
      else
      {
        joyyVal = 1024;
      }
    }
    else //if (joyyVal < 512)
    {
      if (joyyVal > (1024 - (1024 * intBreakTune)))
      {
        joyyVal = (((joyyVal - 512) / intSlopeTune) + 512);
      }
      else
      {
        joyyVal = 0;
      }
    }
    //  Really, I stole this code from Tom, down the hall.  I peeked at how he was
    //  handling the Scarab's, servy system thing, steering mechanism.  Makes sence.  
    //  If a pilot is pushing the stick to its limit, he wants full power.
    //  Also, this eliminates 2 problems.  I dont need to be check for values exceeding
    //  limits being passed.  And, I dont' have to worry about that dang x^3 curve.  Oh,
    //  the AI thinks its so smart by programming its cool curves and all.  I think
    //  <REDACTED>

    // Deadzone Check
    if ((joyxVal >= 512 - intDeadZone) && (joyxVal <= 512 + intDeadZone))
    {
      joyxVal = 512;
    }
    if ((joyyVal >= 512 - intDeadZone) && (joyyVal <= 512 + intDeadZone))
    {
      joyyVal = 512;
    }
    // Execution and Send Updates only when value is changed ---
    if (joyxVal != joyxValPrev)
    {
      Joystick.setXAxis(int(joyxVal));
      joyxValPrev = joyxVal;
      booUpdate = true;
    }
    
    if (joyyVal != joyyValPrev)
    {
      Joystick.setYAxis(int(joyyVal));
      joyyValPrev = joyyVal;
      booUpdate = true;
    }

    if (button0Val != button0ValPrev)
    {    
      Joystick.setButton(0, !button0Val);
      button0ValPrev = button0Val;
      booUpdate = true;
    }

    // Update LED to indicate data was transfered.  This will help preceive when unknow drift occures
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
      Serial.print("    X = ");
      Serial.print(tstxAxis);
      Serial.print("     Y = ");
      Serial.print(tstyAxis);
      Serial.println();
      Serial.print("Joy X = ");
      Serial.print(joyxVal);
      Serial.print(" Joy Y = ");
      Serial.print(joyyVal);
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.println();      
      // Serial.print(27,BYTE); // clear screen - Not sure how to print escape sequences.
    }
  
    // Slow delay if in testing mode.
    if (booTest == true)  
    {
      intRestTime = 250;
    }
  }   // End Delayless Loop
}

//  Closing Notes:  Codes a mess but it works.  I got another idea while writing it.  I
//  am going to issue a request to the AI Programer System to see if it can read data
//  from the ship global sensor array system and convert that data into environmental sound.
//  Its a win win.  If it can't, It will prove the AI system as a sham.  If it can, I will
//  impress that cute little test pilot I see from time to time.  She is the best.
