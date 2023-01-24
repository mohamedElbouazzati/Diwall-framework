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
#define USE_MODEM_LORA 1
// carrier frequency:           868.3 MHz
#define RF_FREQUENCY 868800000 // Hz

#define TX_OUTPUT_POWER 20      // dBm
#define LORA_BANDWIDTH 0        // [0: 125 kHz,
                                //  1: 250 kHz,
                                //  2: 500 kHz,
                                //  3: Reserved]
#define LORA_SPREADING_FACTOR 12 // [SF7..SF12]
#define LORA_CODINGRATE 1       // [1: 4/5,
                                //  2: 4/6,
                                //  3: 4/7,
                                //  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8  // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 5   // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON  false
#define LORA_IQ_INVERSION_ON    false
bool active_send_dbg=false;
typedef enum
{
    LOWPOWER,
    RX,
    RX_TIMEOUT,
    RX_ERROR,
    TX,
    TX_TIMEOUT,
} States_t;

#define RX_TIMEOUT_VALUE 3000
#define BUFFER_SIZE 255 // Define the payload size here

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

States_t State = LOWPOWER;

uint16_t BufferSize1 = BUFFER_SIZE;
uint8_t Buffer1[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

void OnRxError1( void );
void OnTxDone1( void );

void OnRxDone1(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);


int8_t RssiValue1 = 0;
int8_t SnrValue1 = 0;
void pingpong(States_t *memState,bool *isMaster);

int lora_pingpong( void )
{  
     States_t memState=255;
     bool isMaster = false;
     BoardInitMcu();

    RadioEvents.TxDone = OnTxDone1;
    RadioEvents.RxDone = OnRxDone1;

    RadioEvents.RxError = OnRxError1;
    printf(" init done ");
    Radio.Init( &RadioEvents );
        
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,LORA_SPREADING_FACTOR, LORA_CODINGRATE, LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON, true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH, LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON, 0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    SX1276Write( REG_LR_SYNCWORD, 0x34 );
    Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE ); 

    Radio.Rx( RX_TIMEOUT_VALUE );

   printf("initialisation ok \n");

   while( 1 )
    {
       
        pingpong(&memState,&isMaster);
    }
}
 

void OnTxDone1( void )
{
    printf("Transmit ok \n");
    State = active_send_dbg?LOWPOWER:TX;
   
}




void OnRadioTxTimeout1( void )
{
    // Restarts continuous wave transmission when timeout expires
    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, 65535 );
}


void OnRxDone1( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    BufferSize1 = size;
    memcpy( Buffer1, payload, BufferSize1 );
    RssiValue1 = rssi;
    SnrValue1 = snr;
    State = RX;
    printf("\nPacket received: ");
    printf("\t  %s ", Buffer1);
    printf("\t RSSI : %d",  RssiValue1);
    printf("\t SNR : %d \n", SnrValue1);
    State = RX;
    Radio.Sleep( );
}

void OnTxTimeout1( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
}

void OnRxTimeout1( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
}

void OnRxError1( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
}

void pingpong(States_t *memState,bool *isMaster)
{
   
    
   
    uint8_t i;
   
    if(*memState!=State)
        {
            printf("State = %d\n",State);
            *memState=State;
        }
        
        switch( State )
        {
            
        case RX:            printf("RX ");
                            if( *isMaster == true )
                            {
                                if( BufferSize1 > 0 )
                                {
                                    if( strncmp( ( const char* )Buffer1, ( const char* )PongMsg, 4 ) == 0 )
                                    {
                                        // Indicates on a LED that the received frame is a PONG
                                    
                                        printf("Master send PING \n");
                                        // Send the next PING frame
                                        Buffer1[0] = 'P';
                                        Buffer1[1] = 'I';
                                        Buffer1[2] = 'N';
                                        Buffer1[3] = 'G';
                                        // We fill the buffer with numbers for the payload
                                        for( i = 4; i < BufferSize1; i++ )
                                        {
                                            Buffer1[i] = i - 4;
                                        }
                                        DelayMs( 1 );
                                        Radio.Send( Buffer1, BufferSize1 );
                                    }
                                    else if( strncmp( ( const char* )Buffer1, ( const char* )PingMsg, 4 ) == 0 )
                                    { // A master already exists then become a slave
                                        *isMaster = false;
                            
                                        Radio.Rx( RX_TIMEOUT_VALUE );
                                    }
                                    else // valid reception but neither a PING or a PONG message
                                    {    // Set device as master ans start again
                                        *isMaster = true;
                                        Radio.Rx( RX_TIMEOUT_VALUE );
                                    }
                                }
                            }
                            else
                            {
                                if( BufferSize1 > 0 )
                                {
                                    if( strncmp( ( const char* )Buffer1, ( const char* )PingMsg, 4 ) == 0 )
                                    {
                                        // Indicates on a LED that the received frame is a PING
                                        printf("Slave send PONG\n");

                                        // Send the reply to the PONG string
                                        Buffer1[0] = 'P';
                                        Buffer1[1] = 'O';
                                        Buffer1[2] = 'N';
                                        Buffer1[3] = 'G';
                                        // We fill the buffer with numbers for the payload
                                        for( i = 4; i < BufferSize1; i++ )
                                        {
                                            Buffer1[i] = i - 4;
                                        }
                                        DelayMs( 1 );
                                        Radio.Send( Buffer1, BufferSize1 );
                                    }
                                    else // valid reception but not a PING as expected
                                    {    // Set device as master and start again
                                        *isMaster = true;
                                        Radio.Rx( RX_TIMEOUT_VALUE );
                                    }
                                }
                            }
                            State = LOWPOWER;
                            break;
        case TX:
                            // Indicates on a LED that we have sent a PING [Master]
                            // Indicates on a LED that we have sent a PONG [Slave]
                        
                            Radio.Rx( RX_TIMEOUT_VALUE );
                            State = LOWPOWER;
                            break;
        case RX_TIMEOUT:
        case RX_ERROR:      
                            if( *isMaster == true )
                            {
                                // Send the next PING frame
                                printf("RX_TIMEOUT RX_ERROR send PING\n");
                                Buffer1[0] = 'P';
                                Buffer1[1] = 'I';
                                Buffer1[2] = 'N';
                                Buffer1[3] = 'G';
                                for( i = 4; i < BufferSize1; i++ )
                                {
                                    Buffer1[i] = i - 4;
                                }
                                DelayMs( 1 );
                                Radio.Send( Buffer1, BufferSize1 );
                            }
                            else
                            {
                                Radio.Rx( RX_TIMEOUT_VALUE );
                            }
                            State = LOWPOWER;
                            break;
        case TX_TIMEOUT:
                            Radio.Rx( RX_TIMEOUT_VALUE );
                            State = LOWPOWER;
                            break;
        case LOWPOWER:
        default:
            // Set low power
            break;
        }
        if( Radio.IrqProcess != NULL )
        {
            printf("Radio.IrqProcess( );\n");
            Radio.IrqProcess( );
        }
}