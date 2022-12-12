#include "tools/delai.h"
#include <stdio.h>
#include "tools/defint.h"
#include <libbase/console.h>
#include "tools/binary.h"
#include <system.h>
#include <generated/csr.h>
#include <irq.h>
#include "tools/isr.h"
#include "tools/libisr.h"
#include "tools/libtimer.h"

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
   HwTimerInit();//RunTimerWithConfig(COUNTER_TIMER,true,true,true,true,TIMER1);
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

