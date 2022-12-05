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
#define BUFFER_SIZE                                 4 // Define the payload size here
  // carrier frequency:           868.3 MHz
  // bandwidth:                   125 kHz
  // spreading factor:            7
  // coding rate:                 5
  // sync word:                   0x34 reserved for lorawan network
  // output power:                17 dBm (accepted range is -3 - 17 dBm)
  // current limit:               100 mA
  // preamble length:             8 symbols
  // amplifier gain:              0 (automatic gain control)

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];
uint8_t Buf[BUFFER_SIZE];
static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

void OnRadioTxTimeout( void );
int8_t RssiValue = 0;
int8_t SnrValue = 0;


int test1( void )
{  
  //while (1)
 //{    
    for (uint16_t addr = 0; addr < 0x3F; addr++) {
        uint8_t val = Read_SPI(addr);
        printf("Register 0x%02x = 0x%02x\r\n", addr, val);
    }
    BoardInitMcu();

    int t=0;
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnRadioTxTimeout;
    printf(" init done ");
    Radio.Init( &RadioEvents );
        
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, LORA_CODINGRATE, LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    SX1276Write( REG_LR_SYNCWORD, 0x34 );
   
  
    Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE );
    Radio.Rx( 0 );
//while( 1 )
//{

 for (uint16_t addr = 0; addr < 0x3F; addr++) {
        uint8_t val = Read_SPI(addr);
        printf("Register 0x%02x = 0x%02x\r\n", addr, val);
    }
    for (uint16_t addr = 0; addr < 0x3F; addr++) {
        uint8_t val = SX1276Read(addr);
        printf("SX Register 0x%02x = 0x%02x\r\n", addr, val);
    }


    //DelayMs(20);

   // for( int i = 4; i < BufferSize; i++ )
     // {
      // Buffer[i] = i - 1;
     // }
 //while (1)
 //{


    //Buffer[0] = 'T';
    //Buffer[1] = 'E';
   // Buffer[2] = 'S';
   // Buffer[3] = 'T';
   // Radio.Send( Buffer,BufferSize );


    //printf("DIO0 %d \n", GpioRead(&SX1276.DIO2));
// }
 

   	//DelayMs(500);
//}
/*     
while( 1 )
{      // Send the PING frame
                        

   //s SX1276Reset(); 
    DelayMs( 10 );   
   Radio.Send( Buffer, BufferSize );
  //  Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, 65535 );
  
    DelayMs(500);
      printf("packet sent !!!!!!!!!\n");
     //   SX1276Reset();

    SX1276ReadBuffer(0,&Buf,BUFFER_SIZE);
    printf("le packet est : %x and buffer %x\n ",Buf[0],Buffer[0]);
    printf("le packet est : %x and buffer %x\n ",Buf[1],Buffer[1]);
    printf("le packet est : %x and buffer %x\n ",Buf[2],Buffer[2]);
    printf("le packet est : %x and buffer %x\n ",Buf[3],Buffer[3]);
     printf("le packet est : %x and buffer %x\n ",Buf[4],Buffer[4]);
      printf("le packet est : %x and buffer %x\n ",Buf[5],Buffer[5]);
       printf("le packet est : %x and buffer %x\n ",Buf[6],Buffer[6]);
    
    DelayMs(100);
    printf("packet sent !!!!!!!!!\n");
        for (uint16_t addr = 0; addr < 0x10; addr++) {
        //  Read the register
        uint8_t val = SX1276Read(addr);

        //  Print the register value
        printf("Register 0x%02x = 0x%02x\r\n", addr, val);
    }
      
    DelayMs( 2000); 




    /*  
    for (uint16_t addr = 0; addr < 0x3F; addr++) {
        //  Read the register
        uint8_t val = SX1276Read(addr);

        //  Print the register value
        printf("Register 0x%02x = 0x%02x", addr, val);
        printf(" bin = ");
        println_bin(SX1276Read(addr));
        printf("\r\n");
    }


    printf("read module reg other: ");
    println_bin(SX1276Read(0x1D));
    printf("read module RSSI reg: ");
    println_bin(SX1276Read(0x1B));
    printf("\n");
    SX1276ReadBuffer(0,&Buf,BUFFER_SIZE);
    printf("le packet est : %c ",Buf[0]);
    printf("%c",Buf[1]);
    printf("%c",Buf[2]);
        printf(" %c ",Buf[3]);
    printf("%c",Buf[7]);
    printf("%c",Buf[8]);
    printf("%c\n",Buf[30]); 
    DelayMs(100);*/

  //  }
}
 

void OnTxDone( void )
{
    printf("Tx done and LED 4 on");
    leds_out_write(0b1000);


  
}




void OnRadioTxTimeout( void )
{
    // Restarts continuous wave transmission when timeout expires
    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, 65535 );
}


void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
   // printf("Rx done and LED 3 on");
    leds_out_write(0b0100);
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    SnrValue = snr;
    printf("\nPacket received: ");
    printf("\t  %s ", Buffer);
    printf("\t RSSI : %d",  RssiValue);
    printf("\t SNR : %d \n", SnrValue);
    //Radio.Sleep( );



}