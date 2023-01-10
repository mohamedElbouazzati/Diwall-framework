#include <LoRaLib.h>

/// Default config
///                 Arduino UNO      RFM95/96/97/98
///                 GND--------------GND   (ground in)
///                 5V---------------VIN  (3V to 6V 
/// interrupt 1 pin D3---------------DIO0  (interrupt request out) is used by RFM95w module to trigger RxDone/TxDone status.
///          SS pin D10--------------NSS   (CS chip select in)
///         SCK pin D13--------------SCK   (SPI clock in)
///        MOSI pin D11--------------MOSI  (SPI Data in)
///        MISO pin D12--------------MISO  (SPI Data out)
///                    ------------- DIO1 is used by RFM95w module to trigger RxTimeout and other kind of errors status.

/// RFM95 pins translation to Adafruit RFM9x names
/// DIO0 = G0
/// DIO1 = G1 DIO0 
/// DI02 = G2
/// NSS = CS
/// RESET=RST

// My arduino uno wiring
#define RFM95_CS 10
#define RFM95_INT0 2 // verify that on arduino it is an hardware interrupt pin. On arduino UNO pin3 is INT1
#define RFM95_INT1 3
#define RFM95_RST 5

// LoRa parameters
#define RF95_FREQ 868.3
#define RF95_BW 125.0
#define RF95_SF 12
#define RF95_CR 5
#define SYNC_WORD 0x34

// Jammer Type
#define JAMTYPE 0 //0:trigger, 1: selective

// Instantiate lora module
// NSS pin: 4
// DIO0 pin: 3
// DIO1 pin: 5
SX1276 lora = new LoRa(RFM95_CS,RFM95_INT0,RFM95_INT1);
bool firstsend=true;
int transmissionState = ERR_NONE;
int state;

void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  // initialize SX1278 with default settings
  Serial.print(F("Initializing ... "));
  int state = lora.begin(RF95_FREQ, RF95_BW, RF95_SF, RF95_CR, SYNC_WORD,17, 200, 8,0);
  if (state == ERR_NONE) 
  {
    Serial.println(F("success!"));
  } 
  else 
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called
  // when new packet is received
  lora.setDio0Action(setFlag);

  // start listening for LoRa packets
  // NOTE: for spreading factor 6, the packet length
  //       must be known in advance, and provided to both
  //       startReceive() and readData() methods!
  Serial.print(F("Starting to listen ... "));
  //state = lora.startReceive();
  if (state == ERR_NONE) 
  {
    Serial.println(F("success!"));
  } 
  else 
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // NOTE: 'listen' mode will be disabled
  // automatically by calling any of the
  // following methods:
  //
  // lora.standby()
  // lora.sleep()
  // lora.transmit()
  // lora.receive()
  // lora.scanChannel()
  //
  // LoRa module will not receive any new
  // packets until 'listen' mode is re-enabled
  // by calling lora.startReceive()
  Serial.println("initalisation finish");
}

// flag to indicate that a packet was received
volatile bool receivedFlag = true;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

void loop() {
  // check if the flag is set

  if(firstsend)
  {
 
    transmissionState=lora.transmit("PING");
     while(transmissionState != ERR_NONE);
    firstsend=false;
    state = lora.startReceive();

    receivedFlag = false;
  }
  if(receivedFlag) 
  {
        // reset flag
        
        // you can read received data as an Arduino String
        String str;
         state = lora.readData(str);
        
    
        // you can also read received data as byte array
        /*
          byte byteArr[8];
          int state = lora.readData(byteArr, 8);
        */
    
        if (state == ERR_NONE) {
          // packet was successfully received
          Serial.println(F("Received packet!"));
    
          // print data of the packet
          Serial.print(F("Data:\t\t\t"));
          Serial.println(str);
    
          // print RSSI (Received Signal Strength Indicator)
          Serial.print(F("RSSI:\t\t\t"));
          Serial.print(lora.getRSSI());
          Serial.println(F(" dBm"));
    
          // print SNR (Signal-to-Noise Ratio)
          Serial.print(F("SNR:\t\t\t"));
          Serial.print(lora.getSNR());
          Serial.println(F(" dB"));
    
          // print frequency error
          Serial.print(F("Frequency error:\t"));
          Serial.print(lora.getFrequencyError());
          Serial.println(F(" Hz"));

          transmissionState=lora.transmit("PING");
          while(transmissionState != ERR_NONE);
          state = lora.startReceive();
      
          receivedFlag = false;
    
        } else if (state == ERR_CRC_MISMATCH) {
          // packet was received, but is malformed
          Serial.println(F("CRC error!"));
          
          
    
        }
    
        // we're ready to receive more packets,
        // enable interrupt service routine
        enableInterrupt = true;
  }

}
