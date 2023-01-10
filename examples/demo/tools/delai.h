/*! -----------------------------------------------------------------------------------------------------
 * \file      delai.h delai.c
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
#ifndef DELAI_H
#define DELAI_H

/**
 * @brief Add delay in process
 *          delay_ms : function for millisecond
 *          delay_us : function for microsecond
 * @param ms value in millisecond
 * @param us value in microsecond
 */
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);
int elapsed(int *last_event, int period);

#endif