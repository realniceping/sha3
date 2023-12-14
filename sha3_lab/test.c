#include "sha3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int func(int * arr){

    return 0;
}

int main(int argc, char ** argv)
{
    


    return 0;

    int input_30_size = 4;
    char * input_30 = malloc(4);
    input_30[0] = 0b11001010;
    input_30[1] = 0b00011010;
    input_30[2] = 0b11011110;
    input_30[3] = 0b10011000;

    unsigned char* input_30_out = NULL;
    printCharArr(input_30, 4, 1);
    printf("--------------------------------------------\n");
    int test_30_hash_len = sha3_hash(input_30, input_30_size, SHA3_256, &input_30_out);
    printf("Test 30 output\n");
    printCharArr(input_30_out, test_30_hash_len, 0); 
    return 0;

    int test_message_size = 200;
    unsigned char * test_message = malloc(test_message_size);
    memset(test_message, nist_1600_test_char, test_message_size);
    memset(test_message + 200, '\0', 1);
    unsigned char* test_out = NULL;

    printf("Try to hash test variant\nTest message length = %d\n", strlen(test_message));
    printf("input array = \n");
    printCharArr(test_message, test_message_size, 1);
    printf("--------------------------------------------\n");

    int test_len = sha3_hash(test_message, test_message_size, SHA3_256, &test_out);

    printf("Hash output\n");

    printCharArr(test_out, test_len, 0); 
    return 0;


    return 0;
}