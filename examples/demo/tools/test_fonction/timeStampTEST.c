
#include <stdio.h>

int main()
{
    unsigned long utcSecondes=1670334518;
    
    printf("%da:",(unsigned short)(utcSecondes/31556926));
    printf("%dm:",(unsigned char)((utcSecondes/2629743)%13));
    printf("%dj:",(unsigned char)((utcSecondes/86400)%32));
    printf("%dh:",(unsigned char)((utcSecondes/3600)%24));

    printf("%dm:",(unsigned char)((utcSecondes/60)%60));

    printf("%ds",(unsigned char)(utcSecondes%60));
    return 0;
}
