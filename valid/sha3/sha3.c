#include "sha3.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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


// двигает биты в Unsiglned Long Long на d влево
unsigned long long leftRotateLL(unsigned long long w, unsigned int d)
{
    d &= 0x3f; // mod 64

    return (w << d) | (w >> (64 - d));
}

void sha3_keccak_f(unsigned long long A[5][5]){

    for(int i = 0; i < SHA3_NR; i++){

        // THETA 
        unsigned long long C[5];
        unsigned long long D[5];

        // Расчет C
        for (int x = 0; x < 5; x++)
        {
            C[x] = 0L;
            for (int y = 0; y < 5; y++)
            {
                C[x] ^= A[y][x];
            }
        }
         // расчет D
        for (int x = 0; x < 5; x++)
        {
            D[x] = C[(x + 4) % 5] ^ leftRotateLL(C[(x + 1) % 5], 1);
        }

        // сборка результата в A
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                A[y][x] ^= D[x];
            }
        }
        // END OT THETA 




        // RHO, PI
        unsigned long long B[5][5];
        memset(B, 0, 5 * 5 * sizeof(unsigned long long));

        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                B[(2 * x + 3 * y) % 5][y] = leftRotateLL(A[y][x], sha3_rotConst[y][x]);
            }
        }
        // END OF PHO, PI



        // CHI
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                A[y][x] = B[y][x] ^ ((~B[y][(x + 1) % 5]) & B[y][(x + 2) % 5]);
            }
        }
        // END OF CHI



        // IOTA 
            A[0][0] ^= sha3_roundConsts[i];
        // END OF IOTA 
    }
};



int  sha3_hash(unsigned char* in, int n, int mode, unsigned char **out){
    int r, ret_len;

    switch (mode)
    {
        case SHA3_128:
            r = 1344;
            break;
        
        case SHA3_256:
            r = 1088;
            break;

        default:
            mode = SHA3_512;
            r = 576;
            break;
    }

    ret_len = mode >> 3; // => получаем длину выхода в байтах
    r >>= 3;             // Константа r в байтах

    // инициализируем выход функции 
    *out = (unsigned char *)malloc(ret_len * sizeof(unsigned char));

    // state variables
    unsigned long long A[5][5];
    memset(A, 0, 5 * 5 * sizeof(unsigned long long)); // заполняем A нулями
    
    int cursor = 0; //используем как указатель для работы с входом
    int noBlocks = n / r + 1; // всего блоков по r для хеширования

    // ABSORBING PHASE = так называемое впитывание (sopnge algoritm)
    for (int i = 0; i < noBlocks; i++)
    {
        /* code */

        int blockCursor = 0;
        int noBytesInBlock = MIN(r, MAX(n - cursor, 0)); // проверка выхода курсора за пределы блока Значение всегда остается между 0 и r 
        int noPadding = r - noBytesInBlock;
        int padIdx = 0;

        //XOR block into the state (fips 6 step)
        for(int y = 0; y < 5; y++){
            for(int x = 0; x < 5; x++){
                unsigned long long tmp = 0L;
                if(blockCursor < r )
                {
                    int noBytesInWord = MIN(8, MAX(noBytesInBlock - blockCursor, 0));
                    if(noBytesInWord){
                        memcpy(&tmp, in + cursor + blockCursor, noBytesInWord);
                    }
                    //
                    if(noBytesInWord != 8){
                        if(!padIdx){
                            tmp != (unsigned long long)(0x06) << (noBytesInWord << 3);
                        }
                        if(padIdx >= noPadding - 8){
                            tmp |= (unsigned long long)(0x01) << 0x3F;
                        }
                    }
                    padIdx += 8 - noBytesInWord;
                }
                
                A[x][y] ^= tmp;
                // 
                blockCursor += 8;

            }
        }

        // Увеливичем указатель сообщения
        cursor += noBytesInBlock;

        sha3_keccak_f(A);
    }
    

    // SQEEZING phase
    cursor = 0;

    while (cursor < ret_len)
    {
        int blockCursor = 0; //cursor in the block

        for(int y = 0; y < 5; y++){
            for(int x = 0; x < 5; x++){
                if(blockCursor < r && cursor + blockCursor < ret_len){
                    // копировать из state array в выход
                    memcpy(*out + cursor + blockCursor, A[x] + y, sizeof(unsigned long long));

                    blockCursor += 8;
                }
                else
                {
                    break;
                }
            }
        }

        if(blockCursor > r || cursor + blockCursor >= ret_len){
            // закончился блок или размер выхода
            break;
        }
    }

    cursor += r;
    sha3_keccak_f(A);
    
    return ret_len;
};
