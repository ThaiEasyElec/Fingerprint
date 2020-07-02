/*
* Definde by Support ThaiEasyElec
* EFDV743 Rectangle-shaped All-in-one UART Capacitive Fingerprint Sensor
* Vertion 0.9 :	2020/06/30
* Visit us 	  : http://thaieasyelec.com
*/

#ifndef FINGERPRINT_h
#define FINGERPRINT_h

#include <Arduino.h>
#include <Stream.h>

#define ACK_SUCCESS 0x00
#define ACK_FAIL 0x01
#define ACK_FULL 0x04
#define ACK_NOUSER 0x05
#define ACK_USER_OCCUPIED 0x06
#define ACK_FINGER_OCCUPIED 0x07
#define ACK_TIMEOUT 0x08

#define HEADER 0xF5
#define TAIL 0xF5
#define MODIFY_SN_NUMBER 0x08
#define QUERY_SN_NUMBER 0x2A
#define SLEEP_MODE 0x2C
#define ADD_01 0x01
#define ADD_02 0x02
#define ADD_03 0x03
#define DELETE_USER 0x04
#define DELETE_ALL_USER 0x05
#define QUERY_COUNT_OF_USER 0x09
#define COMPARISON 0x0C

class FINGERPRINT {
	public:
		FINGERPRINT();
		void init(Stream &serial);
		bool sendCommand(uint16_t timeout);
		int16_t Compare();
		uint32_t ModifySNnumber(byte sn3, byte sn2, byte sn1);
		uint32_t QueryModelSN();
		void clearACK();
		uint16_t countUser();
		uint16_t countAllUser();
		bool DeleteUser(uint16_t address);
		bool DeleteAllUser();
		bool AddFingerprint(uint16_t address, uint8_t permission);
		uint8_t data2HighBit(uint16_t intData);
		uint8_t data2LowBit(uint16_t intData);
		uint8_t checkSum(uint8_t index2, uint8_t index3, uint8_t index4, uint8_t index5, uint8_t index6);
		void checkResponse(uint8_t index);
		
	protected:
		Stream *_Serial;
};
 
#endif