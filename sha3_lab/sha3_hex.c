#include "sha3.h" 
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

// HEX тесты [9] в документе 
// https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program

int stringToHex(int len, char * in, unsigned char ** out){
    *out = malloc(len);
    if(*out == NULL){
        printf("String To Hex allocation fail");
    }
    int cur = 0;    
    for (int i = 0; i < len; i++)
    {
        // 15 = 0 00011
        unsigned char tmp = (unsigned char)0; 
        unsigned char msb = hexArrFind(in[cur]);
        unsigned char lsb = hexArrFind(in[cur + 1]);
        
        if(msb > 15 || lsb > 15){
            printf("Detected unsupportd char for hex");
            return 1;
        }

        tmp = ((msb << 4) | lsb);

        // memcpy(*out + i, &tmp,1);
        (*out)[i] = tmp; 


        cur += 2;
    }
    return 0;
}

int main(int argc, char ** argv){
    
    int security_param = SHA3_512; //512 DEFAULT!!

    if(argc < 2){
        printf("Security parametr required\n");
        return 1;
    }

    if(!strcmp(argv[1], "224")){
        security_param = SHA3_224;
    }

    if(!strcmp(argv[1], "256")){
        security_param = SHA3_256;
    }

    if(!strcmp(argv[1], "384")){
        security_param = SHA3_384;
    }


    if(argc < 3){
        printf("Data input requered\n");
        return 1;    
    }


    unsigned char * hex_input = NULL;
    if(stringToHex(strlen(argv[2])/2, argv[2], &hex_input) == 1){
        printf("EXIT FAIL");
        return EXIT_FAILURE;
    }

    unsigned char * hash_out = NULL;
    int hash_len = sha3_hash(hex_input, strlen(argv[2])/2, security_param, &hash_out);

    printCharArr(hash_out, hash_len, 0);


    return EXIT_SUCCESS;
}
