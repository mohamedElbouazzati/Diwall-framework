#include <stdio.h>
#include <string.h>
#include <generated/csr.h>
#include "loramacnode/boards/board.h"
#include "loramacnode/system/gpio.h"
#include "loramacnode/system/delay.h"
#include "loramacnode/system/timer.h"
#include "loramacnode/radio/radio.h"
#include "loramacnode/radio/sx1276.h"
#include "libtimer.h"
#include "loramacnode/boards/sx1276-board.h"
#include "loramacnode/radio/sx1276Regs-LoRa.h"

#define REGION_EU868 
#define USE_MODEM_LORA  1
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
#define BUFFER_SIZE                                 4 // Define the payload size here

uint8_t Payload[BUFFER_SIZE];


void OnTxDone( void );
void OnRadioTxTimeout( void );
static RadioEvents_t RadioEvents;
int lora_transmit( void )
{    
      BoardInitMcu();
      RadioEvents.TxDone = OnTxDone;
      RadioEvents.TxTimeout = OnRadioTxTimeout; 
      Radio.Init( &RadioEvents );
      Radio.SetChannel( RF_FREQUENCY );
      Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, LORA_CODINGRATE, LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
      // sync word:  0x34 reserved for lorawan network
      Radio.SetPublicNetwork(true);
      Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE );
      Payload[0] = 'P';
      Payload[1] = 'I';
      Payload[2] = 'N';
      Payload[3] = 'G';
      for( int i = 4; i < BUFFER_SIZE; i++ )
            {
               Payload[i] = i - 4;
             }
      while (1)
      {
         Radio.Send(Payload,BUFFER_SIZE);
        DelayMs(100);
      }
      
}

 
void OnTxDone( void )
{
    printf("Packet Sent!!! \n");
    printf("Tx done and LED 4 on\n");
    leds_out_write(0b1000);
}
void OnRadioTxTimeout( void )
{
    // Restarts continuous wave transmission when timeout expires
    printf("Tx timeout\n");
    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, 65535 );
}
