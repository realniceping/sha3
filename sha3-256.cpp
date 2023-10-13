//     _         ____
//  __| |_  __ _|__ /
// (_-< ' \/ _` ||_ \
// /__/_||_\__,_|___/
           
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <bitset>
#include "sha3.h"
#include <string>
    





int main()
{

    std::string input_strign = "hello from crypto world";
    const int input_size = sizeof(input_strign);
    std::bitset<16> set;
    set[0] = 0xFF;
    set[1] = 0xAA;
    set[2] = 0xAA;
    std::cout << set;
    return 0;
    for (char input_ptr = 0; input_ptr < input_size; input_ptr++)
    {
        char buff = input_strign[input_ptr];
    }
   

    return 0;
}


unsigned int sha3_rotConst[5][5] = {
        {0, 1, 190, 28, 91},
        {36, 300, 6, 55, 276},
        {3, 10, 171, 153, 231},
        {105, 45, 15, 21, 136},
        {210, 66, 253, 120, 78}
};

unsigned long long sha3_roundConsts[SHA3_NR] = {
       0x0000000000000001,
       0x0000000000008082,
       0x800000000000808A,
       0x8000000080008000,
       0x000000000000808B,
       0x0000000080000001,
       0x8000000080008081,
       0x8000000000008009,
       0x000000000000008A,
       0x0000000000000088,
       0x0000000080008009,
       0x000000008000000A,
       0x000000008000808B,
       0x800000000000008B,
       0x8000000000008089,
       0x8000000000008003,
       0x8000000000008002,
       0x8000000000000080,
       0x000000000000800A,
       0x800000008000000A,
       0x8000000080008081,
       0x8000000000008080,
       0x0000000080000001,
       0x8000000080008008
};