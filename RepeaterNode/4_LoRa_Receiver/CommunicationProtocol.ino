#include "CommunicationProtocol.h"


bool CheckParity(uint8_t data)
{
  
}

//returns true if the the value has not been seen before and the message is free to use
bool CheckID(uint8_t[] data, uint8_t[] savedID)
{
  uint8_t id = 0;
  for (int i = 4; i > 0; i--)
  {
    id | data[i]; 
    id = id << 1;
  }
  for (int i = 0; i < sizeof(savedID); i ++)
  {
    if (id == savedID[i])
    {
      return false;
    }
  }
  return true;
}

uint8_t GetVolt(uint8_t data)
{
  
}

uint8_t GetAmp(uint8_t data)
{
  
}

uint16_t GetBatteryLevel(uint8_t data)
{
  
}

uint16_t GetSensorData(uint8_t data)
{
  
}

uint8_t GetChecksum(uint8_t data)
{
  
}
