#include <stdio.h>
#include <irq.h>
#include <generated/csr.h>
#include "loramacnode/boards/LitexLib/defint.h"
#include "loramacnode/boards/LitexLib/delai.h"
#include "loramacnode/boards/LitexLib/isr.h"
#include "loramacnode/boards/LitexLib/libisr.h"

/**
 * @brief Test DIO0 by setting VCC to mentioned pin numbers:
 *          
 */
void dio_test(void);

void dio_test(void)
{   
	dio_init();
    printf("Set pin 2 or 6 or 7 or 8 to VCC to test DIO0, DIO1, DIO2, DIO3");
}



