/* 2_2_Robot_Receive_ATmega328P.ino
   Robot Receive ATmega328P Example
   Written by: Ho Yun Bobby Chan
   Date: 2/15/19
   SparkFun Electronics

   license: Creative Commons Attribution-ShareAlike 4.0 (CC BY-SA 4.0)
   Do whatever you'd like with this code, use it for any purpose.
   Please attribute and keep this license.

   The first step to controlling the RedBot remotely is to first drive it
   from the Serial Monitor in a tethered setup. This is example code
   for the RedBot Mainboard with ATmega328P. After uploading this sketch,
   keep the RedBot tethered to your computer with the USB cable. Flip the
   switches to the respective sides: MOTOR => RUN and POWER => ON. You
   will also need to have UART flipped to the XBee_SW_Serial side.

   Assuming that you have a pair of XBees 1s (or 3 configured with
   802.15.4 protocol) on the same channel, a character will be
   transmitted wirelessly between the XBees. Any charactered received
   from the XBee connected to the software serial defined pins will
   be passed to the Serial Monitor. For troubleshooting, any character
   sent through the Serial Monitor will be echoed back. Try testing the
   controller to see if the robot will move forward or sending the following
   character through the Serial Monitor.

       A = forward

  If your motors are not moving forward when you send the forward command,
  simply flip the wiring. You can adjust the code but that would require
  adjusting more than one line of code. This does not account for motor
  intensity like the example that is used with the Wireless Joystick Example
  and RedBot Experiment 9.

  WARNING: Make sure to flip the switch to the XBEE_SW_SERIAL when
  you are uploading to the RedBot Mainboard. You will have issues uploading
  code and possibly brick your XBee.
*/

#include <RedBot.h>  //include RedBot library
RedBotMotors motors; //make instance of RedBot

char c_data;  // variable for holding incoming data from XBee to Arduino

// We'll use RedBot SoftwareSerial to communicate with the XBee:
// For Atmega328P's
// XBee's DOUT (TX) is connected to pin 14 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 15 (Arduino's Software TX)
#include <RedBotSoftwareSerial.h>
RedBotSoftwareSerial RedBotXBee; //make instance of Software Serial, pins     defined already in modified Software Serial Library

//LED to check if the LED is initialized.
const int status_LED = 13;

void setup() {
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  RedBotXBee.begin(9600);// Initialize SW for XBee for receiving serial
  Serial.begin(9600);// Initialize HW for Serial Monitor for DEBUGGING

  //Status LED to see if the RedBot is initializing
  pinMode(status_LED, OUTPUT);
  for (int i = 0; i < 3; i++) {
    digitalWrite(status_LED, HIGH);//set Status LED on
    delay(50);
    digitalWrite(status_LED, LOW); //set Status LED off
    delay(50);
  }

  Serial.println("RedBot Initialized!");
}//end setup

void loop() {

  if (RedBotXBee.available() || Serial.available()) {
    if (RedBotXBee.available()) {
      c_data = RedBotXBee.read();//store received value from XBee into variable

    }

    else if (Serial.available()) {
      c_data = Serial.read();//store received value from Serial Monitor into variable

    }

    Serial.println("Character Received, ");
    Serial.write(c_data);//send it out to serial monitor
    Serial.println();
    digitalWrite(status_LED, HIGH); //turn ON Status LED
    //delayMicroseconds(500);//add short delay for LED for feedback, this can be commented out if it is affecting performance

    if (c_data == 'A') {
      Serial.println("Drive Forward");
      RedBotXBee.write('A');
      digitalWrite(status_LED, HIGH); //turn ON Status LED

      motors.drive(255); //forward
    }
    else {
      //nothing was received, do nothing
    }


  }

  delay(100); // short pause so we are not constantly receiving characters
  motors.stop();//turn off motors
  digitalWrite(status_LED, LOW); //turn OFF Status LED
}//end loop
