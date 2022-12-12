#ifndef DEFINT_H
#define DEFINT_H

#ifndef _SYS__STDINT_H
    //typedef unsigned char   uint8_t;
    //typedef unsigned short  uint16_t;
    //typedef unsigned int    uint32_t;
    //typedef unsigned long   uint64_t;
    
    #ifdef ___int8_t_defined
            #ifndef _INT8_T_DECLARED
                typedef __int8_t int8_t ;
                #define _INT8_T_DECLARED
            #endif
        typedef short   int16_t;
        typedef int     int32_t;
        typedef long    int64_t;
    #endif

    
    #include <generated/csr.h>
    #include <irq.h>

#endif
#include <stdbool.h>
#endif