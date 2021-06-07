#include "CommunicationProtocol.h"


bool CheckParity(uint8_t data)
{
  
}

//returns true if the the value has not been seen before and the message is free to use
bool CheckID(uint8_t data[], uint8_t savedID[])
{
  uint8_t id = data[0]; 
  for (int i = 0; i < sizeof(savedID); i ++)
  {
    if (id == savedID[i])
    {
      return false;
    }
  }
  return true;
}

uint8_t GetVolt(uint8_t data[])
{
  return data[1]; 
}

uint8_t GetAmp(uint8_t data[])
{
   return data[2]; 
}

uint16_t GetBatteryLevel(uint8_t data[])
{
  return data[3] + data[4]; 
}

uint16_t GetSensorData(uint8_t data[])
{
  uint16_t sensor = 0;
  sensor = data[5];
  sensor = sensor << 8;
  sensor |= data[6];
  return sensor; 
}

uint8_t GetChecksum(uint8_t data[])
{
  
}
