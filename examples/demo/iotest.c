#include "delai.h"
#include <stdio.h>
#include "defint.h"
#include <libbase/console.h>
#include "tools/binary.h"
#include <system.h>
#include <generated/csr.h>
#include <irq.h>
#include "isr.h"
#include "libdio.h"

/**
 * @brief POUR TESTER IOTEST:
 *          il faut mettre en commentaire dio_isr() dans le fichier isr.c
 */
void iotest(void);
//void INIT_ALL_GPIO(void);
//void MODE_GPIO(uint32_t num, uint8_t mode);
//uint32_t READ_GPIO(uint32_t num);
//void WRITE_GPIO(uint32_t num, bool value);


//void IRQTEST (void) __attribute__ ((interrupt ("machine")));

void iotest(void)
{   
    printf("pending read started:\n");

    while (1)
    {
        printf("                      \r");
        uint32_t val=dio3_in_read()<<3|dio2_in_read()<<2|dio1_in_read()<<1|dio0_in_read()<<0;//dio_ev_pending_read();//dio_in_read();dio_in_read();
        print_bin(val);
        if(val)
        {
            delay_ms(500);
            //dio_ev_pending_write(0b1111);
        }
    }	
}


/*void INIT_ALL_GPIO(void)
{
    dio_edge_write(0);
    dio_out_write(0);
    dio_mode_write(0);
    dio_ev_enable_write(0);
    dio_ev_pending_write(0);
    dio_oe_write(0);
}

void MODE_GPIO(uint32_t num, uint8_t mode)
{
    mode ? dio_oe_write(dio_oe_read()|1<< num)
    :dio_oe_write(dio_oe_read()&~(1<< num));
}

uint32_t READ_GPIO(uint32_t num)
{
    return dio_in_read()&(1<<num);
}*/

/*void WRITE_GPIO(uint32_t num, bool value)
{
    value?dio_out_write(dio_out_read()|1<<num)
    :dio_out_write(dio_out_read()&~(1<<num));
}*/

