// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD
#include <generated/csr.h>
#include <libbase/console.h>
#include <irq.h>
#include <libbase/uart.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools/libdio.h"
#include "lora/system/gpio.h"
#include "lora/radio/sx1276.h"
#include "lora/boards/sx1276-board.h"
#include "lora/radio/sx1276Regs-LoRa.h"
#include "tools/delai.h"

void testgpio(void);
void _spi(uint32_t test_value );


/*-----------------------------------------------------------------------*/
/* Uart                                                                  */
/*-----------------------------------------------------------------------*/

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = getchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					fputs("\x08 \x08", stdout);
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				fputs("\n", stdout);
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				fputs(c, stdout);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("\e[92;1mlitex-demo-app\e[0m> ");
}

/*-----------------------------------------------------------------------*/
/* Help                                                                  */
/*-----------------------------------------------------------------------*/

static void help(void)
{
	puts("\nLiteX minimal demo app built "__DATE__" "__TIME__"\n");
	puts("Available commands:");
	puts("help               - Show this command");
	puts("reboot             - Reboot CPU");
#ifdef CSR_LEDS_BASE
	puts("led                - Led demo");
#endif

	puts("testspi            - Test du spi");
	puts("simple             - Test simple spi");
#ifdef __RADIO_H__
	puts("a                  - Programme d'essai Ramdom");
#endif
	puts("iotest             - Programme de test des gpios I/O");
	puts("donut              - Spinning Donut demo");
	puts("helloc             - Hello C");
	puts("test1              - Test reception");

	puts("input_test         - GPIO TEST input test");
	puts("output_test        - GPIO TEST output test");
	puts("interruptIO_test   - GPIO TEST interrupt test");

	puts("test_spiInOut      - SPI TEST spiinout test");

	puts("timer_oneshot      - TIMER TEST timer test");
	puts("timer_reloadshot   - TIMER TEST timer test");
	puts("timer_interruption - TIMER TEST timer test");

#ifdef WITH_CXX
	puts("hellocpp           - Hello C++");
#endif
}

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/

static void reboot_cmd(void)
{
	ctrl_reset_write(1);
}

#ifdef CSR_LEDS_BASE
static void led_cmd(void)
{
	int i;
	printf("Led demo...\n");

	printf("Counter mode...\n");
	for(i=0; i<32; i++) {
		leds_out_write(i);
		busy_wait(100);
	}

	printf("Shift mode...\n");
	for(i=0; i<4; i++) {
		leds_out_write(1<<i);
		busy_wait(200);
	}
	for(i=0; i<4; i++) {
		leds_out_write(1<<(3-i));
		busy_wait(200);
	}

	printf("Dance mode...\n");
	for(i=0; i<4; i++) {
		leds_out_write(0x55);
		busy_wait(200);
		leds_out_write(0xaa);
		busy_wait(200);
	}
}
#endif

#ifdef CSR_LORASPI_BASE
extern void testspi(void);
static void testspi_cmd(void)
{
	printf("lancement testspi ...\n");
	testspi();
}
#endif

extern void iotest(void);
static void iotest_cmd(void)
{
	printf("Lancement du test de GPIO ...\n\r");
	iotest();
}

extern void simple_test(void);
static void simple_test_cmd(void)
{
	simple_test();
}

#ifdef __RADIO_H__
extern void a(void);
static void a_cmd(void)
{
	printf("Ecrire un data spi 0bxxxx xxxx: ...\n");
	//a();
}
#endif

extern void donut(void);
static void donut_cmd(void)
{
	printf("Donut demo...\n");
	donut();
}

extern void helloc(void);
static void helloc_cmd(void)
{
	printf("Hello C demo...\n");
	helloc();
}
extern void test1(void);
static void test1_cmd(void)
{
	printf("test1 C demo...\n");
    test1();
}


extern void input_test(void);
static void input_test_cmd(void)
{
	printf("input_test C demo...\n");
    input_test();
}

extern void output_test(void);
static void output_test_cmd(void)
{
	printf("output_test C demo...\n");
    output_test();
}

extern void interruptIO_test(void);
static void interruptIO_test_cmd(void)
{
	printf("interruptIO_test C demo...\n");
    interruptIO_test();
}

extern void test_spiInOut(void);
static void test_spiInOut_cmd(void)
{
	printf("test_spiInOut C demo...\n");
    test_spiInOut();
}

extern void timer_oneshot(void);
static void timer_oneshot_cmd(void)
{
	printf("timer_oneshot C demo...\n");
    timer_oneshot();
}

extern void timer_reloadshot(void);
static void timer_reloadshot_cmd(void)
{
	printf("timer_reloadshot C demo...\n");
    timer_reloadshot();
}

extern void timer_interruption(void);
static void timer_interruption_cmd(void)
{
	printf("timer_interruption C demo...\n");
    timer_interruption();
}



#ifdef WITH_CXX
extern void hellocpp(void);

static void hellocpp_cmd(void)
{
	printf("Hello C++ demo...\n");
	hellocpp();
}
#endif

/*-----------------------------------------------------------------------*/
/* Console service / Main                                                */
/*-----------------------------------------------------------------------*/

static void console_service(void)
{
	char *str;
	char *token;
	
	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);

	if(strcmp(token, "help") == 0)
		help();

	else if(strcmp(token, "reboot") == 0)
		reboot_cmd();

#ifdef CSR_LEDS_BASE
	else if(strcmp(token, "led") == 0)
		led_cmd();
#endif

	else if(strcmp(token, "testspi") == 0)
		testspi_cmd();
	
	else if(strcmp(token, "simple") == 0)
		simple_test_cmd();

	else if(strcmp(token, "iotest") == 0)
			iotest_cmd();
	else if(strcmp(token, "test1") == 0)
			test1_cmd();

	else if(strcmp(token, "input_test") == 0)
			input_test_cmd();
	else if(strcmp(token, "output_test") == 0)
			output_test_cmd();
	else if(strcmp(token, "interruptIO_test") == 0)
			interruptIO_test_cmd();
	else if(strcmp(token, "test_spiInOut") == 0)
			test_spiInOut_cmd();
	else if(strcmp(token, "timer_oneshot") == 0)
			timer_oneshot_cmd();
	else if(strcmp(token, "timer_reloadshot") == 0)
			timer_reloadshot_cmd();
	else if(strcmp(token, "timer_interruption") == 0)
			timer_interruption_cmd();


#ifdef __RADIO_H__
	else if(strcmp(token, "a") == 0)
			a_cmd();
#endif

	else if(strcmp(token, "donut") == 0)
		donut_cmd();

	else if(strcmp(token, "helloc") == 0)
		helloc_cmd();

#ifdef WITH_CXX
	else if(strcmp(token, "hellocpp") == 0)
		hellocpp_cmd();
#endif
	prompt();
}


int main(void)
{
	
	#ifdef CONFIG_CPU_HAS_INTERRUPT
		irq_setmask(0);
		irq_setie(1);
	#endif

	uart_init();
	
    //rst_oe_write(1);
	//dio_init();

	help();
	prompt();
	//_spi(0xaa )	;

	while(1) {
		console_service();
			//testgpio();
	}

	return 0;
}
void testgpio(void){

	SX1276IoInit();
	//InitSPI();
	//printf("DIO1 = %d",SX1276GetDio1PinState());
	//GpioWrite(&SX1276.DIO2,1);
	//printf("DIO2 = %d",GpioRead(&SX1276.DIO2));
	//printf("test module = ");
	//println_bin(SX1276Read(REG_LR_OPMODE));	
	//printf("test SPI = ");
	//println_bin(Read_SPI(REG_LR_OPMODE));
    printf("Write command : ");
	//printf("test SPI = ");
 	SX1276Write(REG_LR_OPMODE,0x08);
	printf("test module = ");
	println_bin(SX1276Read(REG_LR_OPMODE));
	SX1276Write(REG_LR_OPMODE,0x00);
	println_bin(SX1276Read(REG_LR_OPMODE));
	//printf("test SPI = ");
	//println_bin(Read_SPI(REG_LR_OPMODE));
	delay_ms(300);
	
}

void _spi(uint32_t test_value )
{	

    printf("Before Transmission: \n");
	
    printf("Contents of Control Register: %lx\n", loraspi_control_read());
    printf("Contents of Status Register: 0x%lx\n", loraspi_status_done_read());
    printf("Contents of MOSI:  0x%lx\n", loraspi_mosi_read());
    unsigned int ctrl = (8 << 8) | (1 << 0);  // Value to write in the Control Register, 
    printf("THE VALUE OF CTRL IS: 0x%x\n" ,ctrl);
    
    printf("Starting transmission \n");

    loraspi_loopback_write(1); // Enable Loopback test
    delay_ms(10);
    SX1276Write(0,test_value);
    //loraspi_mosi_write(test_value);     // Write value to MOSI Register
    delay_ms(10);
    printf("Contents of MOSI:  0x%lx\n", loraspi_mosi_read());

   // loraspi_control_write(ctrl); // set message length and start transmission

    //loraspi_control_start_write(0x1);
    printf("Contents of Status Register: 0x%lx\n", loraspi_status_done_read());
    printf("after writing to mosi, contents of Status Register: 0x%lx\n", loraspi_status_done_read());

  //  while (!loraspi_status_read()); //wait for transmission to finish

    printf("MISO: 0x%lx\n",loraspi_miso_read()); // check if transmission has been successful
	

}
