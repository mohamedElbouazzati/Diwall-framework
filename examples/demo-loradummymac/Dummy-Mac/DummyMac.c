#include "DummyMac.h"

void DummyMacParser( uint8_t *Packet, uint16_t PacketSize){
    char Payload[10]={};
    HPM_enable();
    memcpy(Payload,Packet,PacketSize);
    HPM_stop();
    printf("\nParsing by Dummy Mac Done\n");
    HPM_read();
   
}

static void HPM_stop(void){
  //__asm__ volatile(".option rvc");
  __asm__ volatile("csrw  0x320, %0\n" : : "r"(0xFFFFFFFFF));
}

static void HPM_enable(){
 //__asm__ volatile(".option rvc");
 // set tracking by mhpmcounterX by events X           *** EVENTS ***
  __asm__ volatile("csrw 0x323, %0 " :: "r"(0x4)); // LD_STALL : Number of load use hazards 
  __asm__ volatile("csrw 0x324, %0 " :: "r"(0x8)); // JMP_STALL : Number of jump register hazards
  __asm__ volatile("csrw 0x325, %0 " :: "r"(0x10)); // IMISS Cycles waiting for instruction fethces, excluding jumps and branches
  __asm__ volatile("csrw 0x326, %0 " :: "r"(0x20)); // LD : Number of load instructions
  __asm__ volatile("csrw 0x327, %0 " :: "r"(0x40)); // ST : Number of store instructions
  __asm__ volatile("csrw 0x328, %0 " :: "r"(0x80)); // JUMP : Number of jumps(unconditional)
  __asm__ volatile("csrw 0x329, %0 " :: "r"(0x100)); // BRANCH : Number of branches(conditional)
  __asm__ volatile("csrw 0x32a, %0 " :: "r"(0x200)); // Branch_TAKEN : Number of branches taken (conditional)
  __asm__ volatile("csrw 0x32b, %0 " :: "r"(0x400)); // COMP_INSTR : Number of compressed instructions retired
  //__asm__ volatile("csrw 0x32c, %0 " :: "r"(0x800)); // PIP_STALL : Cycles from stalled pipeline   

  __asm__ volatile("csrwi 0xB00, 0x0");  // reset value of mcycle counter    
  __asm__ volatile("csrwi 0xB02, 0x0");  // reset value of minstret counter
  __asm__ volatile("csrwi 0xB03, 0x0");  // reset value of Number of load use hazards counter 
  __asm__ volatile("csrwi 0xB04, 0x0");  // reset value of Number of jump register hazards counter  
  __asm__ volatile("csrwi 0xB05, 0x0");  // reset value of Number of Cycles waiting for instruction fethces, excluding jumps and branches counter
  __asm__ volatile("csrwi 0xB06, 0x0");  // reset value of Number of load instructions counter 
  __asm__ volatile("csrwi 0xB07, 0x0");  // reset value of Number of store instructions counter
  __asm__ volatile("csrwi 0xB08, 0x0");  // reset value of Number of jumps(unconditional) counter
  __asm__ volatile("csrwi 0xB09, 0x0");  // reset value of Number of branches(conditional) counter
  __asm__ volatile("csrwi 0xB0a, 0x0");  // reset value of Number of branches taken (conditional) counter 
  __asm__ volatile("csrwi 0xB0b, 0x0");  // reset value of Number of compressed instructions retired counter
  __asm__ volatile("csrwi 0xB0c, 0x0");  // reset value of Number of cycles from stalled pipeline counter 
 // enable traces
 __asm__ volatile("csrw  0x320, %0\n" : : "r"(0x0)); 
}

static void HPM_read(void){
  volatile unsigned int CYCLES=0;
  volatile unsigned int INSTR=0;
  volatile unsigned int LD_STALL=0;
  volatile unsigned int JMP_STALL=0;
  volatile unsigned int IMISS=0;
  volatile unsigned int LD=0;
  volatile unsigned int ST=0;
  volatile unsigned int JUMP=0;
  volatile unsigned int BRANCH=0;
  volatile unsigned int BRANCH_TAKEN=0;
  volatile unsigned int COMP_INSTR=0;
  volatile unsigned int PIPE_STALL=0;

 // __asm__ volatile(".option rvc");
  __asm__ volatile("csrr %0, 0xB00" : "=r"(CYCLES));
  __asm__ volatile("csrr %0, 0xB02" : "=r"(INSTR)); 
  __asm__ volatile("csrr %0, 0xB03" : "=r"(LD_STALL));
  __asm__ volatile("csrr %0, 0xB04" : "=r"(JMP_STALL));
  __asm__ volatile("csrr %0, 0xB05" : "=r"(IMISS));
  __asm__ volatile("csrr %0, 0xB06" : "=r"(LD));
  __asm__ volatile("csrr %0, 0xB07" : "=r"(ST));
  __asm__ volatile("csrr %0, 0xB08" : "=r"(JUMP));
  __asm__ volatile("csrr %0, 0xB09" : "=r"(BRANCH));
  __asm__ volatile("csrr %0, 0xB0a" : "=r"(BRANCH_TAKEN));
  __asm__ volatile("csrr %0, 0xB0b" : "=r"(COMP_INSTR));
  __asm__ volatile("csrr %0, 0xB0c" : "=r"(PIPE_STALL));

printf("CYCLES,INSTR,LD_STALL,JMP_STALL,IMISS,LD,ST,JUMP,BRANCH,BRANCH_TAKEN,COMP_INSTR,PIPE_STALL\n");
printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",CYCLES,INSTR,LD_STALL,JMP_STALL,IMISS,LD, ST, JUMP, BRANCH,BRANCH_TAKEN,COMP_INSTR,PIPE_STALL);

}

