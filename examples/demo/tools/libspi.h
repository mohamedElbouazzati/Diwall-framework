/*! -----------------------------------------------------------------------------------------------------
 * \file      libspi.h libspi.c
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
#ifndef LIBSPI_H 
#define LIBSPI_H


#include "defint.h"

/**
 * @brief This function initializes the SPI. Activate spi in manual mode on the 
 *        chipselect, and initialize the cs and the spi controller.  
 */
void InitSPI(void);

/**
 * @brief This function starts the SPI (0).
 */
void startSPI(void);

/**
 * @brief This function stops the SPI (0).
 */
void stopSPI(void);

/**
 * @brief This function assigns the value to the CS number.
 * 
 * @param cs_number CS number.
 * @param value Value (true,false).
 */
void SelSPI(uint8_t cs_number,bool value);

/**
 * @brief this function waits for the end of the sending of the byte.
 */
void WaitXfer(void);

/**
 * @brief This function allows to write in a register.
 * 
 * @param addr Address of register.
 * @param cmd Command to write in register.
 */
void Write_SPI(uint8_t addr, uint8_t cmd);

/**
 * @brief This function allows to read in a register
 * 
 * @param addr Address of register.
 * @return uint8_t Value of register.
 */
uint8_t Read_SPI(uint8_t addr);

/**
 * @brief This function allows you to write and read in a register.
 * 
 * @param outData value to write in spi
 * @return uint32t Value return on register.
 */
uint32_t WriteRead_SPI(uint32_t outData);

#endif