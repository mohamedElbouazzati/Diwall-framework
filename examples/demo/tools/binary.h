/*!
 * \file      binary.h binary.c
 *
 * \brief     String converter implementation for Litex
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

#ifndef BINARY_H
#define BINARY_H

typedef struct res_bin res_bin;
struct res_bin
{
    unsigned short size;    //size of str
    unsigned int number;    // number of space
    unsigned char nbbin;    // number of bin caracter
};

/**
 * @brief Calcule Size of String format
 *          String value "0b 11 11"  -> return {8,2,4}
 *          String value "0x  F"     -> return {5,2,1} 
 * @param chaine String value
 * @return res_bin Size value
 */
res_bin lenstr(char * chaine);

/**
 * @brief Transforms BINARY, HEXADECIMAL and DECIMAL strings into (INTEGER)
 *        exemple:
 *          String value "0b 1000 1010" -> return 90
 *                 value "0xFF"         -> return 255
 *                 value "17"           -> return 17 
 *          
 * @param chaine BINARY OR HEXADECIMAL OR DECIMAL string format
 * @return resultat of string
 */
unsigned int strBHD2int(char * chaine);

/**
 * @brief Prints on the screen the result of the parameter value in binary
 *        - print_bin: no return to line.
 *        - println_bin: with return to line.
 *          exemple :
 *              Integer value 88 -> printed on screen(10001000)
 * @param val INTEGER value.
 */
void print_bin(unsigned int val);
void println_bin(unsigned int val);

#endif