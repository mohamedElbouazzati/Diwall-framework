#include <generated/csr.h>
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
    value ? loraspi_cs_sel_write(loraspi_cs_sel_read()|1<<cs_number):
    loraspi_cs_sel_write(loraspi_cs_sel_read()&~1<<cs_number);
}
void WaitXfer(void)
{
    while(!loraspi_status_done_read()); //WAIT SPI TRANFERS BYTE
}
void Write_SPI(uint8_t addr, uint8_t cmd)
{
    startSPI();                         // START SPI
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

uint8_t WriteRead_SPI(uint8_t addr, uint8_t cmd)
{
    Write_SPI(addr,cmd);
    return Read_SPI(addr);
}