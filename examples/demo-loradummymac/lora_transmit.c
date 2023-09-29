#include <stdio.h>
#include <stdlib.h>
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
#define BUFFER_SIZE                                255 // Define the payload size here




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
        int p=0;
        int i=0;
  for (int j = 0; j < sizeof(Packet); ++j + i) {

    if (Packet[j] == '!') {
      printf(" Transmitting packet ... ");  
      //uint8_t Payload[(int)(Packet[j+1]+6)];
      char* Payload;
      Payload = (char *)malloc((Packet[j+1]+7) * sizeof(char));
      int u = 0;
      for (int i = j ; i <Packet[j+1]+6+j; ++i) {
        if (Packet[i] == '#') {
          Payload[u] = Packet[i];
          Payload[Packet[j+1]+7] = '\0';
          Radio.Send(Payload,sizeof(Payload));
          printf("\nPacket Sent :: Payload : %s : size : %d\n",Payload, sizeof(Payload));
          DelayMs(1000);
          free(Payload);
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
       //leds_out_write(0b1000);
       DelayMs(20);
       //leds_out_write(0b0000);
}
void OnRadioTxTimeout( void )
{
    // Restarts continuous wave transmission when timeout expires
    printf("Tx timeout\n");
    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, 65535 );
}

/*
#include <stdlib.h>
#include <string.h>

int main() {
    int n = 5;
    char **table;

    table = (char **)malloc(n * sizeof(char *));

    for (int i = 0; i < n; i++) {
        table[i] = (char *)malloc(20 * sizeof(char)); //allocate memory for each string
    }

    strcpy(table[0], "Hello");
    strcpy(table[1], "world");
    strcpy(table[2], "this");
    strcpy(table[3], "is");
    strcpy(table[4], "a");
    strcpy(table[5], "example");
   

    for (int i = 0; i < n; i++) {
        printf("%s ", table[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        free(table[i]);
    }
    free(table);

    return 0;
}


*/


/*
#include <stdio.h>
#include <stdlib.h>

int main() {
   
    int length = 20;
    char *str;

    str = (char *)malloc((length + 1) * sizeof(char)); //allocate memory for the string, +1 for the null character

    for (int i = 0; i < length; i++) {
        str[i] = 'A' + i; //store a character in each element of the string
    }
    str[length] = '\0'; //add the null character at the end of the string

    printf("The string is: %s\n", str); //print the string
    printf("The length of the string is: %lu\n", strlen(str)); //print the length of the string

    free(str); //deallocate the memory to avoid memory leaks

    return 0;
}

*/