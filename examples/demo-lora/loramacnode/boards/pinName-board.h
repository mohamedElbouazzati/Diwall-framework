/*!
 * \file      pinName-board.h
 *
 * \brief     Target board GPIO pins definitions
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#ifndef __PIN_NAME_BOARD_H__
#define __PIN_NAME_BOARD_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * STM32 Pin Names
 */

#define DIO_PIN \
    DIO0 = (1<<0),\
    DIO1 = (1<<1),\
    DIO2 = (1<<2),\
    DIO3 = (1<<3),\
    DIO4 = (1<<4),\
    DIO5 = (1<<5)\

#define RST_PIN \
    RST = (1<<0)\

#ifdef __cplusplus
}
#endif

#endif // __PIN_NAME_BOARD_H__
