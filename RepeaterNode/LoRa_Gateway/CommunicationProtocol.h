bool CheckParity(uint8_t data);
bool CheckID(uint8_t data[], uint8_t savedID[]);
uint8_t GetVolt(uint8_t data);
uint8_t GetAmp(uint8_t data);
uint16_t GetBatteryLevel(uint8_t data);
uint16_t GetSensorData(uint8_t data);
bool GetChecksum(uint8_t data);
void BuildSendableData(uint8_t *data[], uint8_t ID, uint8_t volt, uint8_t amp, uint16_t battlevel, uint16_t sensor);
uint8_t GetID(uint8_t data[]);
