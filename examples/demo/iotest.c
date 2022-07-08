#include "tools/delai.h"
#include <stdio.h>
#include "tools/defint.h"
#include <libbase/console.h>
#include "tools/binary.h"
#include <generated/csr.h>


void iotest(void);
void MODE_GPIO(uint32_t num, uint8_t mode);
uint32_t READ_GPIO(uint32_t num);
void WRITE_GPIO(uint32_t num, bool value);
void SET_IRQ_GPIO(uint32_t num, bool value);
void SET_IRQ_HANDLER(uint32_t num,void (*IRQTEST)(void));

void IRQTEST(void);

void iotest(void)
{   

    /*********************************************/
    //TEST SUR PIN 0 ( STATUS AND OUTPUT VALUE)
    /*dio_oe_write(1);
    for (unsigned char i = 0; i < 5; i++)
    {
       dio_out_write(1);
       delay_ms(1000);
       dio_out_write(0);
       delay_ms(1000);
    }

    dio_oe_write(0);
    while (1)
    {
        if(dio_in_read()&1)printf("Allumé\r");
        else printf("Éteint\r");
    }*/
    /*********************************************/
    
    /*********************************************/
    // TEST IRQ
    MODE_GPIO(0,0);
    println_bin(dio_oe_read());
    SET_IRQ_GPIO(0,true);
    println_bin(dio_ev_status_read());
    SET_IRQ_HANDLER(0,IRQTEST);

    while(1);
    /*********************************************/
}

void MODE_GPIO(uint32_t num, uint8_t mode)
{
    mode ? dio_oe_write(dio_oe_read()|1<< num)
    :dio_oe_write(dio_oe_read()&~1<< num);
}

uint32_t READ_GPIO(uint32_t num)
{
    return dio_in_read()&(1<<num);
}

void WRITE_GPIO(uint32_t num, bool value)
{
    value?dio_out_write(dio_out_read()|1<<num)
    :dio_out_write(dio_out_read()&~1<<num);
}

void SET_IRQ_GPIO(uint32_t num, bool value)
{
    value?dio_ev_enable_write(dio_ev_enable_read()|1<<num)
    :dio_ev_enable_write(dio_ev_enable_read()&~1<<num);
}

void IRQTEST(void)
{
    printf("DÉCLENCHEMENT REUSSI\n\r");
}

void SET_IRQ_HANDLER(uint32_t num,void (*IRQTEST)(void))
{
    IRQTEST=(void (*)(void))CSR_DIO_EV_STATUS_ADDR+num;
}

