#include <string.h>
#include "lora/boards/board.h"
#include "lora/system/gpio.h"
#include "lora/system/delay.h"
#include "lora/system/timer.h"
#include "lora/radio/radio.h"
#include "lora/radio/sx1276.h"
#include <generated/csr.h>
#include "tools/libspi.h"
#include <stdio.h>

#include "lora/system/gpio.h"

#include "lora/boards/sx1276-board.h"
#include "lora/radio/sx1276Regs-LoRa.h"

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
#define BUFFER_SIZE                                 50 // Define the payload size here
  // carrier frequency:           868.3 MHz
  // bandwidth:                   125 kHz
  // spreading factor:            7
  // coding rate:                 5
  // sync word:                   0x34 reserved for lorawan network
  // output power:                17 dBm (accepted range is -3 - 17 dBm)
  // current limit:               100 mA
  // preamble length:             8 symbols
  // amplifier gain:              0 (automatic gain control)
typedef enum
{
    LOWPOWER,
    RX,
    RX_TIMEOUT,
    RX_ERROR,
    TX,
    TX_TIMEOUT,
}States_t;

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 64 // Define the payload size here

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];
// moi
bool active_send_dbg=false;
States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );


/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

void test_send(void);
void pingpong(States_t *memState,bool *isMaster);

int test1( void )
{  
     States_t memState=255;
     bool isMaster = false;
     BoardInitMcu();
    //BoardInitPeriph( );
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    //RadioEvents.TxTimeout = OnTxTimeout;
    //RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;
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
        //test_send();
        pingpong(&memState,&isMaster);
    }
}
 

void OnTxDone( void )
{
    printf("Transmit ok \n");
    State = active_send_dbg?LOWPOWER:TX;
   
}




void OnRadioTxTimeout( void )
{
    // Restarts continuous wave transmission when timeout expires
    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, 65535 );
}


void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    SnrValue = snr;
    State = RX;
    printf("\nPacket received: ");
    printf("\t  %s ", Buffer);
    printf("\t RSSI : %d",  RssiValue);
    printf("\t SNR : %d \n", SnrValue);
    State = RX;
    Radio.Sleep( );
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
}


void test_send()
{
    active_send_dbg=true;
   
        // Indicates on a LED that the received frame is a PING
        printf("Slave send PONG\n");

        // Send the reply to the PONG string
        Buffer[0] = 'P';
        Buffer[1] = 'O';
        Buffer[2] = 'N';
        Buffer[3] = 'G';
        // We fill the buffer with numbers for the payload
        for( int i = 4; i < BufferSize; i++ )
        {
            Buffer[i] = i - 4;
        }
        DelayMs( 1 );
        Radio.Send( Buffer, BufferSize );
   
        Radio.Rx( RX_TIMEOUT_VALUE );
        DelayMs(1000);
    
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
                                if( BufferSize > 0 )
                                {
                                    if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
                                    {
                                        // Indicates on a LED that the received frame is a PONG
                                    
                                        printf("Master send PING \n");
                                        // Send the next PING frame
                                        Buffer[0] = 'P';
                                        Buffer[1] = 'I';
                                        Buffer[2] = 'N';
                                        Buffer[3] = 'G';
                                        // We fill the buffer with numbers for the payload
                                        for( i = 4; i < BufferSize; i++ )
                                        {
                                            Buffer[i] = i - 4;
                                        }
                                        DelayMs( 1 );
                                        Radio.Send( Buffer, BufferSize );
                                    }
                                    else if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
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
                                if( BufferSize > 0 )
                                {
                                    if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
                                    {
                                        // Indicates on a LED that the received frame is a PING
                                        printf("Slave send PONG\n");

                                        // Send the reply to the PONG string
                                        Buffer[0] = 'P';
                                        Buffer[1] = 'O';
                                        Buffer[2] = 'N';
                                        Buffer[3] = 'G';
                                        // We fill the buffer with numbers for the payload
                                        for( i = 4; i < BufferSize; i++ )
                                        {
                                            Buffer[i] = i - 4;
                                        }
                                        DelayMs( 1 );
                                        Radio.Send( Buffer, BufferSize );
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
                                Buffer[0] = 'P';
                                Buffer[1] = 'I';
                                Buffer[2] = 'N';
                                Buffer[3] = 'G';
                                for( i = 4; i < BufferSize; i++ )
                                {
                                    Buffer[i] = i - 4;
                                }
                                DelayMs( 1 );
                                Radio.Send( Buffer, BufferSize );
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