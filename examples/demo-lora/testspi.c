
#include <stdio.h>
#include <string.h>
#include <generated/csr.h>

#include "tools/libspi.h"
#include "tools/console.h"
#include "tools/binary.h"

#include "lora/radio/sx1276Regs-LoRa.h"

int etat  = 0;
void test0(char * command);
void testspi(void);
void f(char *text,bool *exit);
void test0(char * command)
{

        unsigned int commandDec = strBHD2int(command);
        if(!commandDec);
        else
        {
            //InitSPI();
            //println_bin(Read_SPI(REG_LR_OPMODE));
            //printf("Write command : ");
            //println_bin(strBHD2int(command));
            //println_bin(WriteRead_SPI(REG_LR_OPMODE,commandDec));
            
        }     

    
}

void f(char *text,bool *exit)
{
    if(!exit)
    {
        do
        {
           text=input(&test0);
        } while(text==NULL);
        /*do
        {
           text=input(&test0);
        } while(text==NULL);*/

        if( !strcmp(text,"exit"))
        {
            *exit=true;
            //break;
        }
        else if( !strcmp(text,"EXIT"))
        {
            *exit=true;
            //break;
        }
        else if( !strcmp(text,"Exit"))
        {
            *exit=true;
            //break;
        }
        else printf("Bad format\r\n");
    }
}

void testspi(void)
{
    char *cmd=NULL;
    char *address=NULL;
    bool exit = 0;
    while (!exit)
    {
        f(cmd,&exit);
        f(address,&exit);
    }
    printf("Testspi closed.\n");
}



void print_all_reg(void)
{
    printf("cmd = ");
    print_bin(loraspi_control_read());
    printf("\n");

    printf("sts = ");
    print_bin(loraspi_status_read());
    printf("\n");

    printf("mosi = ");
    print_bin(loraspi_mosi_read());
    printf("\n");

    printf("miso = ");
    print_bin(loraspi_miso_read());
    printf("\n");
    
    printf("cs_n = ");
    print_bin(loraspi_cs_read());
    printf("\n\n\n");
}
