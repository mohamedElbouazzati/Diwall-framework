#ifndef __DUMMYMAC_H__
#define __DUMMYMAC_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


static void HPM_read(void);
static void HPM_stop(void);
static void HPM_enable(void);
void DummyMacParser( uint8_t *Packet, uint16_t PacketSize);

#endif //__DUMMYMAC_H__