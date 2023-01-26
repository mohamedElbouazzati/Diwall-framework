#include <string.h>
#include <stdio.h>
#include <generated/csr.h>
#include "lora-litex/boards/board.h"
#include "lora-litex/system/gpio.h"
#include "lora-litex/system/delay.h"
#include "lora-litex/system/timer.h"
#include "lora-litex/radio/radio.h"
#include "lora-litex/radio/sx1276.h"
#include "libtimer.h"
#include "Dummy-Mac/DummyMac.h"
#include "lora-litex/boards/sx1276-board.h"
#include "lora-litex/radio/sx1276Regs-LoRa.h"
#include "lora-litex/boards/LitexLib/libspi.h"

#define REGION_EU868 
#define USE_MODEM_LORA  1
// carrier frequency:           868.3 MHz
#define RF_FREQUENCY                                868300000 // Hz 

#define TX_OUTPUT_POWER                             20      // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                    12      // [SF7..SF12]
#define LORA_CODINGRATE                             1        // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#define RX_TIMEOUT_VALUE                            3500
#define BUFFER_SIZE                                 255 // Define the payload size here


uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];


static RadioEvents_t RadioEvents;

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
void OnRxTimeout(void);

int8_t RssiValue = 0;
int8_t SnrValue = 0;

int lora_receive( void )
{  
    BoardInitMcu();
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.RxTimeout = OnRxTimeout;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    //sync word: 0x34 reserved for lorawan network
    Radio.SetPublicNetwork(true);
    Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE );
    Radio.Rx(3000);
  //DummyMacParser("payloadaaaaaaaaaaa",10);


    
    
}
 
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{  
    
    //leds_out_write(0b0100);
    DelayMs(20);
    //leds_out_write(0b0000);
    DummyMacParser(payload,size);
    BufferSize = size;
    uint8_t Buf[BufferSize];
    memcpy( Buf, payload, size );
    RssiValue = rssi;
    SnrValue = snr;
    printf("%s,%d ", Buf, sizeof(Buf));
    printf(",%d",  RssiValue);
    printf(",%d\n", SnrValue);
    strcpy(Buf, "\0");
    strcpy(payload, "\0");
}
void OnRxTimeout(void){
    printf("Rx timeout\n");
    //leds_out_write(0b0001);
}