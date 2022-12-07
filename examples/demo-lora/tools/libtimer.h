/*! -----------------------------------------------------------------------------------------------------
 * \file      libtimer.h libtimer.c
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



typedef struct DateTime DateTime;
struct DateTime
{
    unsigned short ticks=0; // to ms
    unsigned char hours;
    unsigned char minutes;
    unsigned char secondes;

    unsigned char day;
    unsigned char month;
    unsigned short year;
};

void config_timer();

void setDateTime(DateTime configDateTime);

void setDate(unsigned char day,unsigned char mouth,unsigned short year);

void setTime(unsigned char hours, unsigned char minutes, unsigned char secondes);

void setTimestamp(unsigned long secondes);

unsigned char getHours(void);

unsigned char getMinutes(void);

unsigned char getSeconds(void);

unsigned char getTicks(void);
/*void getDateTime(void);*/



