#ifndef MY_STRING_H  
#define MY_STRING_H
#include <stdint.h>
#include <string.h>
#ifndef I8
#define I8
typedef int8_t i8;
#endif
i8 my_strcmp(char* str, char* str1);
char* my_strcpy(char* str1, char* str2);
short my_strlen(char* str);
char* reverse_str(char* str);
char* decimal_change_octal(long int decimal_number, short free_space); 
int ascii(char* str);
i8 my_isdigit(char* str);
#endif
