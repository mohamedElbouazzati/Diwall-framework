/*!
 * \file      delai.h delai.c
 *
 * \brief     delay implementation for Litex
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


#endif