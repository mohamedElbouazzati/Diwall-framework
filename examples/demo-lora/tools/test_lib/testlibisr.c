/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>

void afficher(int a)
{
    printf("%d",a);
}

typedef enum
{
    IRQ_VERY_LOW_PRIORITY = 0,
    IRQ_LOW_PRIORITY = 1,
    IRQ_MEDIUM_PRIORITY = 2,
    IRQ_HIGH_PRIORITY = 3,
    IRQ_VERY_HIGH_PRIORITY = 4
}IrqPriorities;

typedef struct StateIRQ StateIRQ; 
struct StateIRQ   
{
    IrqPriorities prio;
    void (*pending_alert)(int);
    int num;
    
};



void permut(StateIRQ *a,StateIRQ *b);
void SetOrderPriority(StateIRQ *DIOs,int size);
void afficherDIO(StateIRQ DIO[]);

StateIRQ DIO[6]={   
        {IRQ_VERY_LOW_PRIORITY,&afficher,0}, //DIO0
        {IRQ_LOW_PRIORITY,&afficher,1}, //DIO1
        {IRQ_MEDIUM_PRIORITY,&afficher,2}, //DIO2   
        {IRQ_HIGH_PRIORITY,&afficher,3}, //DIO3       
        {IRQ_VERY_HIGH_PRIORITY,&afficher,4}, //DIO4
        {IRQ_VERY_LOW_PRIORITY,&afficher,5}, //DIO5
} ;

int main()
{
    
    
    SetOrderPriority(DIO,6);
    afficherDIO(DIO);
    //test = ;
    
  
    //printf("Hello World");

    return 0;
}






void SetOrderPriority(StateIRQ *DIOs,int size)
{
    printf("compteur : ");
    for(int i=0;i<size-1;i++)
    {
        printf("%d ",i);
        if(DIOs[i].prio<DIOs[i+1].prio)  // Si le dio a la position la plus prioritaire est inférieur à dio à la position la moins priotaire permuter
        {
            permut(&DIOs[i],&DIOs[i+1]);
            i=-1;
            printf("\n");
        }
    }
    printf("\n");
}

void afficherDIO(StateIRQ DIO[6])
{
    for(int i =0;i<6;i++)
    printf("%d ",DIO[i].num);
}

void permut(StateIRQ *a,StateIRQ *b)
{
    StateIRQ save = *a;
    *a=*b;
    *b=save;
}