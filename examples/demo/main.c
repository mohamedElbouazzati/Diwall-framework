// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools/console.h"
#include <irq.h>

#include <generated/csr.h>

/*-----------------------------------------------------------------------*/
/* Uart                                                                  */
/*-----------------------------------------------------------------------*/

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
#ifdef __RADIO_H__
	puts("a                  - Programme d'essai Ramdom");
#endif
	puts("iotest             - Programme de test des gpios I/O");
	puts("donut              - Spinning Donut demo");
	puts("helloc             - Hello C");
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

#ifdef __RADIO_H__
extern void a(void);
static void a_cmd(void)
{
	printf("Ecrire un data spi 0bxxxx xxxx: ...\n");
	a();
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

	else if(strcmp(token, "iotest") == 0)
			iotest_cmd();

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

	help();
	prompt();

	while(1) {
		console_service();
	}

	return 0;
}
