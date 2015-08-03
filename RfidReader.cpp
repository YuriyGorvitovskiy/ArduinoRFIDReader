/**
 ** This is Public Domain Software.
 ** 
 ** The author disclaims copyright to this source code.  
 ** In place of a legal notice, here is a blessing:
 **
 **    May you do good and not evil.
 **    May you find forgiveness for yourself and forgive others.
 **    May you share freely, never taking more than you give.
 **/
 
#include <Arduino.h>
#include <String.h>
#include "RfidReader.h"


void RfidReader::begin(Stream& anInput, byte aPresencePin) {
	input = &anInput;
	presencePin = aPresencePin;
	pos = 0xFF;
	state = RfidNothing;
	pinMode(presencePin, INPUT);
}

boolean RfidReader::validate() {
	return (data[0] ^ data[1] ^ data[2] ^ data[3] ^ data[4]) == data[5];
}

void RfidReader::print(Print& to) {
	to.print("RFID:");
	for (int i = 0; i < 6; ++i) {
		Serial.print(data[i] >> 4,  HEX);
        Serial.print(data[i] & 0XF, HEX);
    }
}
void RfidReader::println(Print& to) {
	print(to);
	to.println("");
}

void RfidReader::readNext() {
	byte ch = input->read();
	if (ch == 0x2) {
		pos = 0;
		return;
	}
	if (pos == 12) {
		if (ch == 0x3) {
			state = RfidArrived;
		}
		return;
	}
	byte val = ch - '0';
	if (val > 9)
		val = (ch - 'A') + 10;
		
	if (pos & 1)
		data[pos>>1] |= (val & 0xF);
	else
		data[pos>>1] = (val & 0xF) << 4;
	++pos;
}

RfidState RfidReader::check() {
	switch(state) {
		case RfidLeft:	
			state = RfidNothing;
			//No break intentionally
		case RfidNothing: 
			while(input->available() && state == RfidNothing) {
				if(input->read() == 0x2) {
					pos = 0;
					state = RfidReading;
				}
			}
			if (state == RfidNothing)
				break;
			//No break intentionally
		case RfidReading: 
			while (input->available() && state == RfidReading)
				readNext();
			break;
		case RfidArrived:
			state = RfidPresent;
			//No break intentionally
		case RfidPresent:
			if (input->available() || digitalRead(presencePin) == LOW) {
				state = RfidLeft;
			}
			break;
	}
	return state;
}
