#include <generated/csr.h>
#include "libdio.h"
#include "libspi.h"

#define BIT_RW (1<<7)

void startSPI(void)
{
    loraspi_control_length_write(8);
    SelSPI(0,1);
}

void stopSPI(void)
{
    SelSPI(0,0);                   // init cs
    loraspi_control_write(0);      // init all
}

void InitSPI(void)
{       
    loraspi_cs_write(0);           // init cs
    loraspi_cs_mode_write(1);
    loraspi_control_write(0);      // init all
}


void SelSPI(uint8_t cs_number,bool value)
{   
    value ? loraspi_cs_sel_write(loraspi_cs_sel_read()|1<<cs_number)
          : loraspi_cs_sel_write(loraspi_cs_sel_read()&~1<<cs_number);
     //loraspi_control_write(0); à remettre
}
void WaitXfer(void)
{
    while(!loraspi_status_done_read()); //WAIT SPI TRANFERS BYTE


}
void Write_SPI(uint8_t addr, uint8_t cmd)
{
    startSPI(); // START SPI
    loraspi_mosi_write(addr|BIT_RW);        // ADDR + WRITE
    loraspi_control_start_write(1);
    WaitXfer();
    
    loraspi_mosi_write(cmd);                // WRITTING CMD
    loraspi_control_start_write(1);
    WaitXfer();
    stopSPI();                          // STOP SPI 
}

uint8_t Read_SPI(uint8_t addr)
{
    startSPI();
    loraspi_mosi_write(addr&~BIT_RW);    // ADDR + READ 
    loraspi_control_start_write(1);
    WaitXfer();

    loraspi_mosi_write(0);              //EMPTY CMD FOR WAITTING DATA
    loraspi_control_start_write(1);

    WaitXfer();

    uint8_t res =loraspi_miso_read();   // READING byte
    stopSPI();
    return res;
}

uint32_t WriteRead_SPI(uint32_t outData)
{
    //printf("addr: 0x%x",(uint8_t)(outData & 0xFF));
    //printf("   cmd: 0x%x n°%ld\n",(uint8_t)(outData>>8 & 0xFF),increm);
    //printf("Write_SPI(0x%x",(uint8_t)(outData>>8 & 0xFF));
    //printf(", 0x%x); All value 0x%x\n",(uint8_t)(outData & 0xFF),outData);
    loraspi_mosi_write(outData);
    loraspi_control_start_write(1);
    WaitXfer();
    outData = loraspi_miso_read();  
    return outData;
}
void SetContext( void* context,uint8_t num  )
{
    DIOs[num].context=context;
}
