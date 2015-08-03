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
 
#ifndef __RFID_READER_H__
#define __RFID_READER_H__

enum RfidState {
	RfidNothing,
	RfidReading,
	RfidArrived,
	RfidPresent,
	RfidLeft
};

class RfidReader {

protected:
	Stream*  	input;
	byte		presencePin;
	byte 		data[6];
	byte 		pos;
	RfidState	state;
	
	void 	    readNext();
public:
	void	  begin(Stream& anInput, byte aPresencePin);
	
	RfidState check();
	byte*	  lastRfid();
	boolean   validate();
	
	void 	  print(Print& to);
	void	  println(Print& to);
	
};

inline byte* RfidReader::lastRfid() {
	return data;
}

#endif //__RFID_READER_H__
