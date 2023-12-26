#include "../h/my_string.h"
#include "../h/linked_list.h"
i8 my_strcmp(char* str, char* str1) {
    for(short i = 0; str[i] || str1[i]; i++) {
        if(str[i] != str1[i]) {
            return ((str[i]) - (str1[i]));
        }
    }
    return 0; 
}

char* my_strcpy(char* str1, char* str2) {
    for(short i = 0, j = 0; str2[j]; j++) {
        str1[i++] = str2[j];
    }
    return str1;
}

short my_strlen(char* str) {
    i8 len = 0;
    while(str[len]){
        len++;
    }
    return len;
} 

char* reverse_str(char* str) {
    i8 len = my_strlen(str);
    char temp;
    for(i8 i = 0; i < len/2; i++) {
        temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
    return str;
}

char* decimal_change_octal(long int decimal_number, short free_space) {
    char* str;
    if(free_space) {
        str = calloc(sizeof(char), (free_space + 1));
    }
    else {
        return NULL;
    }
    for(i8 i = 0; i < free_space; i++) {
        if(decimal_number != 0) {
            str[i] = (char)(decimal_number % 8 + 48);
            decimal_number = decimal_number/8;
        }
        else {
            str[i] = '0';
        }
    }
    return reverse_str(str);
}

int ascii(char* str) {
    int property = 0;
    for(i8 i = 0 ;str[i]; i++) {
        property += (int)str[i];
    }
    return property;
}

i8 my_isdigit(char* str) {
    for(i8 i = 0; str[i]; i++) {
        if(str[i] > 56 && str[i] < 48) {
            return 1;
        }
    }
    return 0;
}
