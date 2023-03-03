#include "console.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = getchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					fputs("\x08 \x08", stdout);
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				fputs("\n", stdout);
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				fputs(c, stdout);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strlen(*str);//strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

/*----------------------------------------------*/
// FONCTION POUR ENTRER DES VALEURS
/*----------------------------------------------*/
char *inputF(void (*function)(char *))
{
					
            char *buff;
            char *token;
            buff = readstr();
            if(buff !=NULL)
            {
                token = get_token(&buff);
                if(token[0]!='\0' && token[0]!='\n')
                {
                    function(token);
                    return token;
                }
                else return NULL;    
            }
            else return NULL;
            
}

char *input(void)
{
	char * val;
	printf("Write here : ");
	do
    {
		
        char *buff;
            char *token;
            buff = readstr();
            if(buff !=NULL)
            {
                token = get_token(&buff);
                if(token[0]!='\0' && token[0]!='\n')
                {
					
                    val =token;
					//printf("token val = %s\n",val);
                    //return token;
                }
                else val = NULL;    
            }
            else val= NULL;
    }while(val==NULL);		
            return val;
            
}
