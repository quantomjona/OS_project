#include "string.h"
char* strcpy(char* destination, const char* source){
    if(destination!=NULL && source!=NULL){
        char *temp = destination;
        while(*destination++ = *source++); // or while((*strDest++=*strSrc++) != '\0');
        return temp;
    }
    

}
char *strcat(char *destination, const char *source){
    char *temp = destination;
    while(*destination++);
    while(*destination++ = *source++);
    return temp;
    

}
size_t strlen(const char *str)
{
        const char *s;

        for (s = str; *s; ++s)
                ;
        return (s - str);
}
void *memcpy(void *dest_str, const void * src_str, size_t n){
    char *d = dest_str;
    const char *s = src_str;
    while (n--)
        *d++ = *s++;
    return dest_str;
}
void *memset(void *str, int c, size_t n){
    unsigned char * ptr = str;

    while(n-- >0){
        *ptr++ = c;
    }
    return str;
}
