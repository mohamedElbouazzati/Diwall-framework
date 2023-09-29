/*!
 * \file      console.h console.c
 *
 * \brief     console implementation for Litex
 *
 * \copyright MadHighTech of source \ref LICENSE.
 *
 * \code
 *            _______              __                                            
 *           /       \            /  |                                           
 *           $$$$$$$  | ______   _$$ |_     ______                               
 *           $$ |__$$ |/      \ / $$   |   /      \                              
 *           $$    $$//$$$$$$  |$$$$$$/   /$$$$$$  |                             
 *           $$$$$$$/ $$ |  $$ |  $$ | __ $$ |  $$ |                             
 *           $$ |     $$ \__$$ |  $$ |/  |$$ \__$$ |                             
 *           $$ |     $$    $$/   $$  $$/ $$    $$/                              
 *           $$/       $$$$$$/     $$$$/   $$$$$$/                                                                                                                                                                                
 *                                   ╔═╗╔╦╗         
 *                                   ║   ║          
 *                                   ╚═╝╔═╗┬ ┬┬ ┬  ┬
 *                                      ╚═╗└┬┘│ └┐┌┘
 *                                      ╚═╝ ┴ ┴─┘└┘                                                                              
 * \endcode
 *
 * \author    TERRINE Christophe ( MadHighTech )
 */

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
char *input(void);
char *inputF(void (*function)(char *));
#ifdef __cplusplus
}
#endif

#endif /* __CONSOLE_H */
