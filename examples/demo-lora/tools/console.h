/*! -----------------------------------------------------------------------------------------------------
 * \file      console.h console.c
 *
 * \brief     SPI implementation for Litex (loraspi)
 *
 * \code                                                                                                                                                                          
 *          ___  _ __   ___ _ __                  
 *         / _ \| '_ \ / _ \ '_ \                 
 *        | (_) | |_) |  __/ | | |                
 *         \___/| .__/_\___|_| |_| _ _ __ ___ ___ 
 *              | | / __|/ _ \| | | | '__/ __/ _ \
 *              |_| \__ \ (_) | |_| | | | (_|  __/
 *                  |___/\___/ \__,_|_|  \___\___|
 *                                                                                                                      
 * \endcode
 *
 * \author    TERRINE Christophe ( www.ct-sylv.com )
 * \author    Mohamed El Bouazzati ()  
 *-------------------------------------------------------------------------------------------------------*/

#ifndef __CONSOLE_H
#define __CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <libbase/uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <libbase/uart.h>
#include <stdbool.h>
#define readchar getchar
#define putsnonl(X) fputs(X, stdout)


int readchar_nonblock(void);
char *readstr(void);
char *get_token(char **str);
char *input(void (*function)(char *));
#ifdef __cplusplus
}
#endif

#endif /* __CONSOLE_H */
