/*******************************************************************************************************
  Programs for Arduino - Copyright of the author Stuart Robinson - 08/02/20

  This program is supplied as is, it is up to the user of the program to decide if the program is
  suitable for the intended purpose and free from errors.
*******************************************************************************************************/

/*******************************************************************************************************
  Program Operation - The program listens for incoming packets using the LoRa settings in the 'Settings.h'
  file. The pins to access the lora device need to be defined in the 'Settings.h' file also.

  There is a printout of the valid packets received, the packet is assumed to be in ASCII printable text,
  if its not ASCII text characters from 0x20 to 0x7F, expect weird things to happen on the Console Monitor.
  The LED will flash for each packet received and the buzzer will sound, if fitted.

  Sample Console monitor output;

  1109s  Hello World 1234567890*,CRC,DAAB,RSSI,-61dBm,SNR,9dB,Length,23,Packets,1026,Errors,0,IRQreg,50

  If there is a packet error it might look like this, which is showing a CRC error,

  1189s PacketError,RSSI,-111dBm,SNR,-12dB,Length,0,Packets,1126,Errors,1,IRQreg,70,IRQ_HEADER_VALID,IRQ_CRC_ERROR,IRQ_RX_DONE

  Console monitor baud rate is set at 9600.
*******************************************************************************************************/

#define Program_Version "V1.0"

#include <SPI.h>                                 //the lora device is SPI based so load the SPI library
#include "SX128XLT.h"                            //include the appropriate library   
#include "Settings.h"                            //include the setiings file, frequencies, LoRa settings etc   

//#include <arduino-timer.h>                       //Timer heartbeat

SX128XLT LT;                                     //create a library class instance called LT

uint32_t RXpacketCount;
uint32_t errors;

uint8_t RXBUFFER[RXBUFFER_SIZE];                 //create the buffer that received packets are copied into

uint8_t RXPacketL;                               //stores length of packet received
int8_t  PacketRSSI;                              //stores RSSI of received packet
int8_t  PacketSNR;                               //stores signal to noise ratio of received packet4

uint8_t TXPacketL;
uint32_t TXPacketCount, startmS, endmS;

uint8_t buff[] = "Packet has been echoed";
uint8_t passedBuff[32];
uint8_t passedBuffCounter = 0;



//auto heartbeat = timer_create_default();

void loop()
{
  rxReceiver();
  //heartbeat.tick(); // tick the timer

}


void rxReceiver() {
  RXPacketL = LT.receive(RXBUFFER, RXBUFFER_SIZE, 60000, WAIT_RX); //wait for a packet to arrive with 60seconds (60000mS) timeout

  digitalWrite(LED1, HIGH);                      //something has happened

  PacketRSSI = LT.readPacketRSSI();              //read the recived RSSI value
  PacketSNR = LT.readPacketSNR();                //read the received SNR value

  if (RXPacketL == 0)                            //if the LT.receive() function detects an error, RXpacketL == 0
  {

    packet_is_ErrorRX();
  }
  else
  {
    packet_is_OKRX();

    if (getHeartbeatID(RXBUFFER) > 0)
  {
    Console.print("Heartbeat from: ");
    Console.println(getHeartbeatID(RXBUFFER));
  } 
  else 
  {
    Console.print("MessageID: ");
    Console.println(GetID(RXBUFFER));
    Console.print("Sensordata: ");
    Console.println(GetSensorData(RXBUFFER));
    Console.print("Voltage :");
    Console.println(GetVolt(RXBUFFER));
  }

    
  }

  digitalWrite(LED1, LOW);                        //LED off

  // Console.println();

  
}

bool sendHeartBeat(void *) {
  txTransmitter();
  return true;
}



void txTransmitter() {
  Console.print(TXpower);                                       //print the transmit power defined
  Console.print(F("dBm "));
  Console.print(F("Packet> "));
  Console.flush();

  TXPacketL = sizeof(RXBUFFER);                                    //set TXPacketL to length of array
  buff[TXPacketL - 1] = '*';                                   //replace null character at buffer end so its visible on reciver

  LT.printASCIIPacket(RXBUFFER, TXPacketL);                        //print the buffer (the sent packet) as ASCII

  digitalWrite(LED1, HIGH);
  startmS =  millis();                                         //start transmit timer
  if (LT.transmit(RXBUFFER, TXPacketL, 10000, TXpower, WAIT_TX))   //will return packet length sent if OK, otherwise 0 if transmit, timeout 10 seconds
  {
    endmS = millis();                                          //packet sent, note end time
    TXPacketCount++;
    packet_is_OKTX();
  }
  else
  {
    packet_is_ErrorTX();                                 //transmit packet returned 0, there was an error
  }

  digitalWrite(LED1, LOW);
  Console.println();
  delay(packet_delay);                                 //have a delay between packets
}


void packet_is_OKRX()
{
  uint16_t IRQStatus, localCRC;

  IRQStatus = LT.readIrqStatus();                  //read the LoRa device IRQ status register

  RXpacketCount++;

  printElapsedTime();                              //print elapsed time to Console Monitor
  Console.print(F("  "));
  //LT.printASCIIPacket(RXBUFFER, RXPacketL);        //print the packet as ASCII characters

  localCRC = LT.CRCCCITT(RXBUFFER, RXPacketL, 0xFFFF);  //calculate the CRC, this is the external CRC calculation of the RXBUFFER
  /*Console.print(F(",CRC,"));                        //contents, not the LoRa device internal CRC
    Console.print(localCRC, HEX);
    Console.print(F(",RSSI,"));
    Console.print(PacketRSSI);
    Console.print(F("dBm,SNR,"));
    Console.print(PacketSNR);
    Console.print(F("dB,Length,"));
    Console.print(RXPacketL);
    Console.print(F(",Packets,"));
    Console.print(RXpacketCount);
    Console.print(F(",Errors,"));
    Console.print(errors);
    Console.print(F(",IRQreg,"));
    Console.print(IRQStatus, HEX);*/
}

void packet_is_OKTX()
{
  //if here packet has been sent OK
  uint16_t localCRC;

  Console.print(F("  BytesSent,"));
  Console.print(TXPacketL);                             //print transmitted packet length
  localCRC = LT.CRCCCITT(RXBUFFER, TXPacketL, 0xFFFF);
  Console.print(F("  CRC,"));
  Console.print(localCRC, HEX);                              //print CRC of sent packet
  Console.print(F("  TransmitTime,"));
  Console.print(endmS - startmS);                       //print transmit time of packet
  Console.print(F("mS"));
  Console.print(F("  PacketsSent,"));
  Console.print(TXPacketCount);                         //print total of packets sent OK
}


void packet_is_ErrorRX()
{
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                   //read the LoRa device IRQ status register

  printElapsedTime();                               //print elapsed time to Console Monitor

  if (IRQStatus & IRQ_RX_TIMEOUT)                   //check for an RX timeout
  {
    Console.print(F(" RXTimeout"));
  }
  else
  {
    errors++;
    Console.print(F(" PacketError"));
    Console.print(F(",RSSI,"));
    Console.print(PacketRSSI);
    Console.print(F("dBm,SNR,"));
    Console.print(PacketSNR);
    Console.print(F("dB,Length,"));
    Console.print(LT.readRXPacketL());               //get the real packet length
    Console.print(F(",Packets,"));
    Console.print(RXpacketCount);
    Console.print(F(",Errors,"));
    Console.print(errors);
    Console.print(F(",IRQreg,"));
    Console.print(IRQStatus, HEX);
    LT.printIrqStatus();                            //print the names of the IRQ registers set
  }

  delay(250);                                       //gives a longer buzzer and LED flash for error

}

void packet_is_ErrorTX()
{
  //if here there was an error transmitting packet
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                  //read the the interrupt register
  Console.print(F(" SendError,"));
  Console.print(F("Length,"));
  Console.print(TXPacketL);                         //print transmitted packet length
  Console.print(F(",IRQreg,"));
  Console.print(IRQStatus, HEX);                    //print IRQ status
  LT.printIrqStatus();                             //prints the text of which IRQs set
}



void printElapsedTime()
{
  float seconds;
  seconds = millis() / 1000;
  Console.print(seconds, 0);
  Console.print(F("s"));
}


void led_Flash(uint16_t flashes, uint16_t delaymS)
{
  uint16_t index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}


void setup()
{
  //BuildSendableData(RXBUFFER, 255, 0);
  //  heartbeat.every(10000000, sendHeartBeat);
  //  pinMode(LED1, OUTPUT);                        //setup pin as output for indicator LED
  ///  led_Flash(2, 125);                            //two quick LED flashes to indicate program start

  Console.println();
  Console.print(F(__TIME__));
  Console.print(F(" "));
  Console.println(F(__DATE__));
  Console.println(F(Program_Version));
  Console.println();
  Console.println(F("4_LoRa_Receiver Starting"));
  Console.println();

  if (BUZZER > 0)
  {
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(BUZZER, LOW);
  }

  SPI.begin();

  //SPI beginTranscation is normally part of library routines, but if it is disabled in the library
  //a single instance is needed here, so uncomment the program line below
  //SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  //setup hardware pins used by device, then check if device is found
  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, LORA_DEVICE))
  {
    Console.println(F("LoRa Device found"));
    led_Flash(2, 125);
    delay(1000);
  }
  else
  {
    Console.println(F("No device responding"));
    while (1)
    {
      led_Flash(50, 50);                                       //long fast speed LED flash indicates device error
    }
  }

  //The function call list below shows the complete setup for the LoRa device using the information defined in the
  //Settings.h file.
  //The 'Setup LoRa device' list below can be replaced with a single function call;
  //LT.setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate);

  //***************************************************************************************************
  //Setup LoRa device
  //***************************************************************************************************
  LT.setMode(MODE_STDBY_RC);
  LT.setRegulatorMode(USE_LDO);
  LT.setPacketType(PACKET_TYPE_LORA);
  LT.setRfFrequency(Frequency, Offset);
  LT.setBufferBaseAddress(0, 0);
  LT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate);
  LT.setPacketParams(12, LORA_PACKET_VARIABLE_LENGTH, 255, LORA_CRC_ON, LORA_IQ_NORMAL, 0, 0);
  LT.setDioIrqParams(IRQ_RADIO_ALL, (IRQ_TX_DONE + IRQ_RX_TX_TIMEOUT), 0, 0);
  LT.setHighSensitivity();
  //LT.setLowPowerRX();
  //***************************************************************************************************


  Console.println();
  LT.printModemSettings();                                     //reads and prints the configured LoRa settings, useful check
  Console.println();
  LT.printOperatingSettings();                                 //reads and prints the configured operting settings, useful check
  Console.println();
  Console.println();
  LT.printRegisters(0x900, 0x9FF);                             //print contents of device registers
  Console.println();
  Console.println();

  Console.print(F("Receiver ready - RXBUFFER_SIZE "));
  Console.println(RXBUFFER_SIZE);
  Console.println();
}
