#include <stdio.h>
#include <string.h>
#include <generated/csr.h>

/**
 * @brief RESETEST:
 *          Set Reset pin to 0 to reset LoRa Module 
 */
void rest_test(void);

void reset_test(void)
{   
    rst_out_write(rst_out_read() & ~(1 << 0) );  // set reset pin to 0
    DelayMs(20);
    printf("Reset Lora module Set",rst_out_read());
    rst_out_write(rst_out_read() | (1 << 0) );     // set reset pin to 1
    DelayMs(6);
    printf("Reset Lora module removed",rst_out_read());
}

