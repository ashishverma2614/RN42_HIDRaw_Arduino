/*
  RN42_HIDRaw_Arduino.cpp - Roving Netowrks RN42 Bluetooth HID raw library
  Developed by Chris Leitner - 2012

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  RN42 HID raw report format:
  |start(1 byte)|length(1 byte)|descriptor(1 byte)|data(length - 1 [for the descriptor])
  Keyboard:
  |0xFD|9|1|modifier|0x00|code 1|code 2|code 3|code 4|code 5|code 6
  Keyboard modifier bits (sent as one byte)
  bit 7		|bit 6 		|bit 5		|bit 4	|bit 3	|bit 2	|bit 1		|bit 0
  rt GUI	|rt alt		|rt shift	|rt ctrl|lt GUI	|lt alt	|lt shift	|lt ctrl
  
  You must initialize the Serial library in your code before you call any of these functions.
  The default speed for the RN42 is 115200 - add this in the setup function Serial.begin(115200);
  */

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "RN42_HIDRaw_Arduino.h"

RN42_HIDRaw_Arduino::RN42_HIDRaw_Arduino(){
}

//Sends a raw report for a keyboard HID
void RN42_HIDRaw_Arduino::SendKeyboardRaw(char raw_data[], uint8_t mod_bit){

	//write the RN42 Keyboard raw header
	Serial.write((uint8_t)0xFD); //start byte indicator for RN42
    Serial.write((uint8_t)0x09); //length of the raw report (how many bytes)
    Serial.write((uint8_t)0x01); //descritptor byte see the RN42 HID manual for details 0x01 is keyboard
    Serial.write((uint8_t)mod_bit); //modifier bit (indicates shift, control, and other keys (see comments above)
    Serial.write((uint8_t)0x00); //unused always send 0x00
	//write the key press data
	//each of these should be an ASCII keycode indicating a keypressed
	//example 0x04 is an 'a' see the RN42 HID manual for a list of codes
	Serial.write((uint8_t)raw_data[0]);
    Serial.write((uint8_t)raw_data[1]);
    Serial.write((uint8_t)raw_data[2]);
    Serial.write((uint8_t)raw_data[3]);
    Serial.write((uint8_t)raw_data[4]);
    Serial.write((uint8_t)raw_data[5]);
}

//This function just send a raw report with no keys or modifiers pressed
void RN42_HIDRaw_Arduino::SendKeyboardRawBlank(void){
	
	//write the RN42 Keyboard raw header
	Serial.write((uint8_t)0xFD);
    Serial.write((uint8_t)0x09);
    Serial.write((uint8_t)0x01);
    Serial.write((uint8_t)0x00);
    Serial.write((uint8_t)0x00);
	//write the key press data
	Serial.write((uint8_t)0x00);
    Serial.write((uint8_t)0x00);
    Serial.write((uint8_t)0x00);
    Serial.write((uint8_t)0x00);
    Serial.write((uint8_t)0x00);
    Serial.write((uint8_t)0x00);
}

//Sends a raw report for a gamepad
void RN42_HIDRaw_Arduino::SendGamepadRaw(char x_left, char y_left, char z_pos, char rot_pos, uint8_t buttons1, uint8_t buttons2)
{
	//write the header part for RN42
	Serial.write((uint8_t)0xFD); //start byte
	Serial.write((uint8_t)0x06); //length of the descriptor
	//gampad positions and buttons
	//on a gamepad there typically is two analog joysticks one is typically used to
	//indicate x/y position and one is for z/rotation. 
	Serial.write((uint8_t)x_left); //value between -127 to 127 indicating the x postition
	Serial.write((uint8_t)y_left); //value between -127 to 127 indicating the y postition
	Serial.write((uint8_t)z_pos); //value between -127 to 127 indicating the z postition
	Serial.write((uint8_t)rot_pos); //value between -127 to 127 indicating the rotation postition
	//one bit for each button pressed there can be a total of 16 buttons one byte in each
	//set the bit to show a button pressed and clear the bit to indicate not pressed
	Serial.write((uint8_t)buttons1); 
	Serial.write((uint8_t)buttons2);
}

void RN42_HIDRaw_Arduino::SendMouseRaw(uint8_t buttons, uint8_t x_stop, uint8_t y_stop, uint8_t wheel){
	//write the header part for RN42
	Serial.write((uint8_t)0xFD);
	Serial.write((uint8_t)0x05);
	Serial.write((uint8_t)0x02);
	//gampad positions and buttons
	Serial.write((uint8_t)buttons);
	Serial.write((uint8_t)x_stop);
	Serial.write((uint8_t)y_stop);
	Serial.write((uint8_t)wheel);
}