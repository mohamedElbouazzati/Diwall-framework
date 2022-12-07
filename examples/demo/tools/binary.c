
#include "binary.h"
#include "stdio.h"
#include <stdlib.h>
int HexaValue(char digit);
int HexaValue(char digit)
{
    switch(digit)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A':
        case 'a': return 10;
        case 'B':
        case 'b': return 11;
        case 'C':
        case 'c': return 12;
        case 'D':
        case 'd': return 13;
        case 'E':
        case 'e': return 14;
        case 'F':
        case 'f': return 15;
        default:
        {
            //printf("Cannot decode that symbol: %c", digit);
            return -1;
        }
    }
}


res_bin lenstr(char * chaine)
{
    res_bin fx ={2,0,0};
    while(chaine[fx.size]!='\0'&&chaine[fx.size]!='\n'&&chaine[fx.size]!='\r')
    {
       if(chaine[fx.size]!=' ')
       {
           fx.nbbin+=1;
           if(fx.number==0) fx.number=1;
           else fx.number*=2; 
       }
       fx.size+=1;
    }
    return fx;
}


unsigned int strBHD2int(char * chaine)
{
    if(chaine[0]=='0'&&(chaine[1]=='b'||chaine[1]=='B'))//BINAIRE
    {
        printf("Binary detected ! ");
        res_bin a = lenstr(chaine);
        unsigned int res=0;
        for( unsigned short i = 0;i<a.size;i++)
        {
            
            if(chaine[i+2]=='1')
            res+=a.number;
            if(chaine[i+2]=='0'||chaine[i+2]=='1')
            a.number/=2;
        }
        return res;
    }
    else if (chaine[0]=='0'&&(chaine[1]=='x'||chaine[1]=='X'))
    {
        /* code */
        printf("Hexadecimal detected ! \r\n");
        res_bin a = lenstr(chaine);
        int i;
        unsigned int res = 0;
        unsigned int weight = 1;
        
        for(i = a.size - 1; i >= 2; --i)
        {
            char num=HexaValue(chaine[i]);
            if(num!=-1)
            {
                res += weight * num;
                weight *= 16;
            }
        }
        return res;
    }
    
    else
    {
        unsigned int res = (unsigned int)atoi(chaine);
        if(res)printf("Decimal detected ! \r\n");
        //else printf("Bad format\r\n");
        return res;
    }
}


void println_bin(unsigned int val)
{
    print_bin(val);
    printf("\n\r");
}


void print_bin(unsigned int val)
{   
    if(val==1)
    printf("1");
    
    else if(val==0)
    printf("0");
    
    else
    {
        int res=(val/2);
        print_bin(res); 
        printf("%d",val-2*res);
    }
}
