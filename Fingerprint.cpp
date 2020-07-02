#include "Fingerprint.h"

FINGERPRINT::FINGERPRINT()
{
}

void FINGERPRINT::init(Stream &serial)
{	
 _Serial = &serial;
}

uint16_t timeout = 5000;
uint8_t CMD[6] = {0x09, 0x00, 0x00, 0x00, 0x00, 0x09};
uint8_t ACK[8];

bool FINGERPRINT::sendCommand(uint16_t timeout)
{
  while (_Serial->available()>0)
  {
    _Serial->read();
  }
  _Serial->write(HEADER);
  _Serial->write(CMD, sizeof(CMD));
  _Serial->write(TAIL);

  while (!_Serial->available() && timeout > 0)
  {
    delay(1);
    timeout--;
  }

  byte ack[8];
  _Serial->readBytes(ack, 8);

  for (int i = 0; i < sizeof(ack); i++)
  {
    //Serial.print(" ");
    //Serial.print(ack[i], HEX);
    ACK[i] = ack[i];
  }

  if (ACK[6] == checkSum(ACK[1], ACK[2], ACK[3], ACK[4], ACK[5]))
  {
    //Serial.println("checkSum ok");
    return ACK_SUCCESS;
  }
  else
  {
    //Serial.println("checkSum fail");
    return ACK_FAIL;
  }
}

int16_t FINGERPRINT::Compare()
{
  CMD[0] = COMPARISON;
  CMD[1] = 0;
  CMD[2] = 0;
  CMD[3] = 0;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  if (sendCommand(timeout) == ACK_SUCCESS)
  {
    if ((ACK[4] == 0x01))
    {
      return (((ACK[2] << 8) & 0xFF00) + ACK[3]);
    }
    else
    {
      checkResponse(ACK[4]);
      return -1;
    }
  }
  else
  {
    return -1;
  }
  clearACK();
}

uint32_t FINGERPRINT::ModifySNnumber(byte sn3, byte sn2, byte sn1)
{
  CMD[0] = MODIFY_SN_NUMBER;
  CMD[1] = sn3;
  CMD[2] = sn2;
  CMD[3] = sn1;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  return (ACK[2] << 16) + (ACK[3] << 8) + ACK[4];
  clearACK();
}

uint32_t FINGERPRINT::QueryModelSN()
{
  CMD[0] = QUERY_SN_NUMBER;
  CMD[1] = 0;
  CMD[2] = 0;
  CMD[3] = 0;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  return (ACK[2] << 16) + (ACK[3] << 8) + ACK[4];
  clearACK();
}

void FINGERPRINT::clearACK()
{
  for (int i = 0; i < 8; i++)
  {
    ACK[i] = 0;
  }
}

uint16_t FINGERPRINT::countUser()
{
  CMD[0] = QUERY_COUNT_OF_USER;
  CMD[1] = 0;
  CMD[2] = 0;
  CMD[3] = 0;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  return ((ACK[2] << 8) + ACK[3]);
  clearACK();
}

uint16_t FINGERPRINT::countAllUser()
{
  CMD[0] = QUERY_COUNT_OF_USER;
  CMD[1] = 0;
  CMD[2] = 0;
  CMD[3] = 0xFF;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  return ((ACK[2] << 8) + ACK[3]);
  clearACK();
}

bool FINGERPRINT::DeleteUser(uint16_t address)
{
  CMD[0] = DELETE_USER;
  CMD[1] = data2HighBit(address);
  CMD[2] = data2LowBit(address);
  CMD[3] = 0;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  return ACK[4];
  clearACK();
}

bool FINGERPRINT::DeleteAllUser()
{
  CMD[0] = DELETE_ALL_USER;
  CMD[1] = 0;
  CMD[2] = 0;
  CMD[3] = 0;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  return ACK[4];
  clearACK();
}

bool FINGERPRINT::AddFingerprint(uint16_t address, uint8_t permission)
{
  Serial.print("Add Fingerprint Step1 ");
  CMD[0] = ADD_01;
  CMD[1] = data2HighBit(address);
  CMD[2] = data2LowBit(address);
  CMD[3] = permission;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  checkResponse(ACK[4]);
  clearACK();

  Serial.print("Add Fingerprint Step2 ");
  CMD[0] = ADD_02;
  CMD[1] = data2HighBit(address);
  CMD[2] = data2LowBit(address);
  CMD[3] = permission;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  checkResponse(ACK[4]);
  clearACK();

  Serial.print("Add Fingerprint Step3 ");
  CMD[0] = ADD_03;
  CMD[1] = data2HighBit(address);
  CMD[2] = data2LowBit(address);
  CMD[3] = permission;
  CMD[4] = 0;
  CMD[5] = checkSum(CMD[0], CMD[1], CMD[2], CMD[3], CMD[4]);
  sendCommand(timeout);
  checkResponse(ACK[4]);
  if(ACK[4]==ACK_SUCCESS)
  {
    return ACK_SUCCESS;
  }
  else
  {
    return ACK_FAIL;
  }
  clearACK();
}

uint8_t FINGERPRINT::data2HighBit(uint16_t intData)
{
  return (intData >> 8) & 0x00FF;
}

uint8_t FINGERPRINT::data2LowBit(uint16_t intData)
{
  return (intData) & 0x00FF;
}

uint8_t FINGERPRINT::checkSum(uint8_t index2, uint8_t index3, uint8_t index4, uint8_t index5, uint8_t index6)
{
  uint8_t checkSum = index2 ^ index3 ^ index4 ^ index5 ^ index6;
  return checkSum;
}

void FINGERPRINT::checkResponse(uint8_t index)
{
  switch (index) {
    case ACK_SUCCESS:
      Serial.println("Success");
      break;
    case ACK_FAIL:
      Serial.println("Fail");
      break;
    case ACK_FULL:
      Serial.println("Full");
      break;
    case ACK_NOUSER:
      Serial.println("No User");
      break;
    case ACK_USER_OCCUPIED:
      Serial.println("User Occupied");
      break;
    case ACK_FINGER_OCCUPIED:
      Serial.println("Finger Occupied");
      break;
    case ACK_TIMEOUT:
      Serial.println("Time Out");
      break;
    default:
      Serial.println("ERROR checkResponse");
      break;
  }
}