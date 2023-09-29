#include "libDiwall.h"





  void HPM_stop(void){
 __asm__ volatile(".option rvc");
  __asm__ volatile("csrw  0x320, %0\n" : : "r"(0x1F));
}

  void HPM_enable(void){
 __asm__ volatile(".option rvc");
 // set tracking by mhpmcounterX by events X           *** EVENTS ***
  __asm__ volatile("csrw 0x323, %0 " :: "r"(0x4)); // HPC1
  __asm__ volatile("csrw 0x324, %0 " :: "r"(0x8)); // HPC2

  __asm__ volatile("csrwi 0xB03, 0x0");  // reset HPC1 counter 
  __asm__ volatile("csrwi 0xB04, 0x0");  // reset HPC2 counter  
  __asm__ volatile("csrwi 0xB05, 0x0");  //reset NwHPC register

 __asm__ volatile("csrw  0x320, %0\n" : : "r"(0x0)); 
}


void HPM_read(void){
// only for dataset generation : 
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

printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",CYCLES,INSTR,LD_STALL,JMP_STALL,IMISS,LD, ST, JUMP, BRANCH,BRANCH_TAKEN,COMP_INSTR,PIPE_STALL);

}