#include "CommunicationProtocol.h"

uint8_t IDcounter = 0;

bool CheckParity(uint8_t data[])
{
  uint8_t parity = 0;
  parity = data[7] & 0b00000001;
  int ones = 0;

  for(int ii = 0; ii < 6; ii++)
  {
    for(int i=0; i < 8; i++)
    {
        /* If LSB is set then increment ones otherwise zeros */
        if(data[ii] & 1)
            ones++;
        

        /* Right shift bits of num to one position */
        data[ii] >>= 1;
    }
  } 
  ones = ones % 2;
   
  return ones & parity;
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

uint8_t GetID(uint8_t data[])
{
 return data[0]; 
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
  return data[3] + data[4]; //todo
}

uint16_t GetSensorData(uint8_t data[])
{
  uint16_t sensor = 0;
  sensor = data[5];
  sensor = sensor << 8;
  sensor |= data[6];
  return sensor; 
}

bool GetChecksum(uint8_t data[])
{
  uint8_t check = 0;
  check = data[7] & 0b11111110;
  check >>= 1;

  return check == 8 * 7;
  
}


uint8_t getHeartbeatID(uint8_t data[])
{
  if(data[0] &  0x80)
  {
    return data[1];
  }
  return 0;
}


void BuildSendableData(uint8_t *data, uint8_t volt, uint8_t amp, uint16_t battlevel, uint16_t sensor)
{
  

  data[0] = IDcounter;
  data[1] = volt;
  data[2] = amp;
  
  uint16_t temp16 = 0;
  temp16 = battlevel & 0xff00;
  temp16 >>= 8;
  data[3] = temp16;
  temp16 = 0;
  temp16 == battlevel & 0x00ff;
  data[4] = temp16;
  temp16 = 0;

  temp16 = sensor & 0xff00;
  temp16 >>= 8;
  data[5] = temp16;
  temp16 = 0;
  temp16 == sensor & 0x00ff;
  data[6] = temp16;
  temp16 = 0;
  IDcounter++;
  IDcounter %= 127;
} 

void BuildHeartbeatData(uint8_t *data, uint8_t Heartbeat)
{
  
  
  data[0] = IDcounter | 0x80;
  data[0] = data[0] | Heartbeat;
  data[1] = Heartbeat;
  IDcounter++;
  IDcounter %= 127;
} 
