#include "sha3.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>





int main(int argc, char** argv){
    

    int security_param = SHA3_512;

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
    
    printf("input string: %s\n", argv[2]);
    printf("input string length: %d\n", strlen(argv[2]));

    printf("sha3 begin with security %d \n", security_param);

    unsigned char* output = NULL;
    int len = sha3_hash(argv[2], strlen(argv[2]), security_param, &output);

    printCharArr(output, len, 0); 
    return 0;
    

}

// SHA512
// 0xFF = a6f098adf45424539eb214272e0436894bb6fe3f22f5bf45725e1d4f37313a9547415ca108ea84664995d9ccd3983dc21806765fb8e20d6b686ce51ee6583ec8
// hello = 75d527c368f2efe848ecf6b073a36767800805e9eef2b1857d5f984f036eb6df891d75f72d9b154518c1cd58835286d1da9a38deba3de98b5a53e5ed78a84976