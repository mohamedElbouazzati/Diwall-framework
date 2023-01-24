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
 * @return resultat of string. If no valid conversion could be performed, it returns zero
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
void print_all_reg(void);

#endif