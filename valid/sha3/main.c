#include "sha3.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char hex[16] = "0123456789ABCDEF";
void printCharArr(unsigned char *arr, int len){
    for(int i = 0; i < len; i++){
        printf("%c%c%s", hex[arr[i] >> 4], hex[arr[i] & 0x0F], "");
    }
}

int main(){
    printf("sha3 begin\n");
    unsigned char* message = "abcdef"; 
    unsigned char* output = NULL;
    int len = sha3_hash(message, 6, SHA3_512, &output);

    printCharArr(output, len); 

}