#include <stdio.h>
#include <string.h>
#include <generated/csr.h>
#include "lora-litex/boards/board.h"
#include "lora-litex/system/gpio.h"
#include "lora-litex/system/delay.h"
#include "lora-litex/system/timer.h"
#include "lora-litex/radio/radio.h"
#include "lora-litex/radio/sx1276.h"
#include "libtimer.h"
#include "lora-litex/boards/sx1276-board.h"
#include "lora-litex/radio/sx1276Regs-LoRa.h"
#include "input_packet.h"

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
      while (1)
      {
        int i=0;
  for (int j = 0; j < sizeof(Packet); ++j + i) {

    if (Packet[j] == 2) {
      printf(" Transmitting packet ... ");  
      uint8_t Payload[Packet[j+1]];
      int u = 0;
      for (int i = j + 2; i < Packet[j + 1] + 4 + j; ++i) {
        if (Packet[i] == 4) {
          Radio.Send(Payload,sizeof(Payload));
          printf("\nPacket Sent :: Payload : %s : size : %d\n",Payload, sizeof(Payload));
          DelayMs(1000);
          break;
        } else {
          Payload[u] = Packet[i];
          u++;
        }
      }
    }
  }  
    break;
      }
      
}

 
void OnTxDone( void )
{
       leds_out_write(0b1000);
       DelayMs(20);
       leds_out_write(0b0000);
}
void OnRadioTxTimeout( void )
{
    // Restarts continuous wave transmission when timeout expires
    printf("Tx timeout\n");
    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, 65535 );
}
