#include "sha3.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int sha3_rotConst[5][5] = {
    {0, 36, 3, 105, 210},
    {1, 300, 10, 45, 66},
    {190, 6, 171, 15, 253},
    {28, 55, 153, 21, 120},
    {91, 276, 231, 136, 78}};

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
    0x8000000080008008};


FILE * output_file;
// char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char hex[16] = "0123456789abcdef";

unsigned char hexArrFind(char a){
    for (int i = 0; i < 16; i++)
    {
        if(a == hex[i]){
            // retrun must be between 0 and 15 
            return (unsigned char) i;
        }
    }
    return 16; // check if 
}


void printULLbin(unsigned long long a){
    for (size_t i = 0; i < 64; i++)
    {
        printf("%d",( a >> (63 - i)) & 0x01);
    }
    printf("\n");
}

void printULLhex(unsigned long long a){
        printf("0x%llx\n", a);   
}

void printCharArr(unsigned char *arr, int len, int separate){
    for(int i = 0; i < len; i++){
        if(separate) printf("%c%c%s", hex[arr[i] >> 4], hex[arr[i] & 0x0F], " ");
        if(!separate) printf("%c%c%s", hex[arr[i] >> 4], hex[arr[i] & 0x0F], "");   
    }
    printf("\n");
}


void printKeccakStateArray(unsigned long long A[5][5]){
    int bytes_n = 8 * 5 * 5; //always 200 
    int bytes_row_n = 16;
    int new_line = 0;
    unsigned long long tmp;
    fwrite("\n------------------------------\n", 1, 32, output_file);
    for (size_t x = 0; x < 5; x++)
    {
        for (size_t y = 0; y < 5; y++)
        {
            for (size_t z = 1; z <= 8; z++)
            {
                tmp = (A[y][x] >> (64 - z * 8)) & 0xFF;
                char * buff = malloc(2);
                buff[0] = hex[tmp & 0x0F];
                buff[1] = hex[tmp >> 4];
                buff[2] = ' ';
                fwrite(buff, 1, 3, output_file);
                free(buff);
            }

            if(new_line == 1){
                new_line = 0;
                fwrite("\n", 1, 1, output_file);
            }else{
                new_line++;
            }
        }
    }

    for (size_t x = 0; x < 5; x++)
    {
        for (size_t y = 0; y < 5; y++)
        {
            
            char * hexoutbuff = malloc(20);
            sprintf(hexoutbuff, "0x%llx\n", A[y][x]);
            fwrite(hexoutbuff, 1, 20, output_file);
            free(hexoutbuff);
        }
    }
 
    fwrite("\n------------------------------\n", 1, 32, output_file);
}

unsigned long long sha3_rotWord(unsigned long long w, unsigned int d)
{
    d = d & 0x3f; // d mod 64

    unsigned long long max = ~0l;

    unsigned long long wrapPortion = (max << (64 - d)) & w; // get the d msb's
    unsigned long long shiftPortion = (max >> d) & w;       // get the (64 - d) lsb's

    // shift to put in place
    wrapPortion >>= 64 - d;
    shiftPortion <<= d;

    // return the addition
    return wrapPortion | shiftPortion;
}

void sha3_keccak_f(unsigned long long A[5][5])
{   

    for (int i = 0; i < SHA3_NR; i++)
    {
        // THETA ===============================
        unsigned long long C[5];
        unsigned long long D[5];

        // calculate C
        for (int x = 0; x < 5; x++)
        {
            C[x] = 0L;
            for (int y = 0; y < 5; y++)
            {
                C[x] ^= A[x][y];
            }
        }

        // calculate D
        for (int x = 0; x < 5; x++)
        {
            D[x] = C[(x + 4) % 5] ^ sha3_rotWord(C[(x + 1) % 5], 1);
        }

        // XOR A D
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                A[x][y] ^= D[x];
            }
        }
        fwrite("\nAfter Theta\n", 1, 14, output_file);
        printKeccakStateArray(A);

        // RHO, PI ===============================
        unsigned long long B[5][5];
        memset(B, 0, 5 * 5 * sizeof(unsigned long long));

        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                B[y][(2 * x + 3 * y) % 5] = sha3_rotWord(A[x][y], sha3_rotConst[x][y]);
            }
        }

        fwrite("\nAfter Rho and Pi\n", 1, 20, output_file);
        printKeccakStateArray(A);

        // CHI ===============================
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                A[x][y] = B[x][y] ^
                          ((~B[(x + 1) % 5][y]) & B[(x + 2) % 5][y]);
            }
        }

        fwrite("\nAfter Chi\n", 1, 12, output_file);
        printKeccakStateArray(A);

        // IOTA ===============================
        A[0][0] ^= sha3_roundConsts[i];

        fwrite("\nAfter Iota\n", 1, 13, output_file);
        printKeccakStateArray(A);
    }
}

int sha3_hash(unsigned char *in, int n, int mode, unsigned char **out)
{
    // 
    output_file = fopen("out.txt", "w");
    int r, ret_len;

    switch (mode)
    {
        case SHA3_224:
            r = 1152; // В битах
            break;
        case SHA3_256:
            r = 1088; // В битах
            break;
        case SHA3_384:
            r = 832; // В битах
            break;
        default: // SHA3_512
            mode = SHA3_512; 
            r = 576; // В битах
            break;
    }

    ret_len = mode >> 3; // Приводим к байтам
    r >>= 3;             // Приводим к байтам

    // Выделим память под выход
    *out = malloc(ret_len * sizeof(unsigned char));
    if (!(*out)) {
        printf("Sha3 output allocation fali\n");
        return 0;
    }

    // Инициализируем state Array
    unsigned long long A[5][5];
    memset(A, 0, 5 * 5 * sizeof(unsigned long long));
    fwrite("Absorb Begin\n", 1, 13, output_file);
    printKeccakStateArray(A);


    int cursor = 0;           // Указатель для сообщения
    int noBlocks = n / r + 1; // количество блоков

    // ABSORBING PHASE
    for (int i = 0; i < noBlocks; i++) // Пробегаемся по всем блокам сообщения
    {
        // В этом цикле работа происходит с блоками (ниже переменные для блока)
        int blockCursor = 0;                             // Указатель для блока
        int noBytesInBlock = MIN(r, MAX(n - cursor, 0)); // не более чем r, если r меньше чем n - cursor, принимает это знаечение (понадобится для инкремента курсора)
        int noPadding = r - noBytesInBlock;              // number of bytes to pad to get a complete block
        int padIdx = 0;                                  // current number of bytes already padded

        // XOR блоков и state array
        for (int y = 0; y < 5; y++)      // пробегаемся по всему state array
        {                                // пробегамеся по всему state array
            for (int x = 0; x < 5; x++)  // пробегамеся по всему state array
            {
                unsigned long long tmp = 0L;
                // будем писать в tmp 
                if (blockCursor < r) // Проверка на выход за пределы r
                {

                    // ----------------- ЗАПИСЬ ПЕРЕМЕННОЙ TMP -------------
                    int noBytesInWord = MIN(8, MAX(noBytesInBlock - blockCursor, 0)); // Узнаем число блоков в слове (либо 8, либо хвостик последнего блока (например когда число его байт не кратно 8))
                    if (noBytesInWord) // если байты в слове вообще имеются
                    {
                        // пишем в tmp байты крайнего слова 
                        memcpy(&tmp, in + cursor + blockCursor, noBytesInWord); // указатель на входное сообщение + указатель блока + указатель слова
                    }
                    // ----------------- Конец записи переменной TMP -------------

                    // Проскакиваем только в случае если байты слова не кратны 8
                    if (noBytesInWord != 8)
                    {
                        // must pad with 01, then 1000*01
                        if (!padIdx)
                        {
                            // Певрый Pad 001
                            tmp |= (unsigned long long)(0x06) << (noBytesInWord << 3); // noBytes to noBits
                        }
                        if (padIdx >= noPadding - 8)
                        {
                            // Pad последнего байта
                            tmp |= (unsigned long long)(0x01) << 0x3f; // 0b1000000*0
                        }
                        // в противном случае оставлям значение равнрое нулю

                        padIdx += 8 - noBytesInWord;
                    }
                }

                // XOR из стандарта
                A[x][y] ^= tmp;

                // Увечелим курсор блока (пробег по state array)
                blockCursor += 8;
            }
        }

        // Увеличим курсор в сообщении
        cursor += noBytesInBlock;

        fwrite("Data to be absorbed\n", 1, 20, output_file);
        printKeccakStateArray(A);
        // KECCAK-f
        sha3_keccak_f(A);
    }

    // SQUEEZING PHASE
    cursor = 0; // Указатель для строки выхода

    while (cursor < ret_len)
    {
        // Курсор для блока
        int blockCursor = 0;

        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                if (blockCursor < r && cursor + blockCursor < ret_len)
                {
                    // Копируем в выход
                    memcpy(*out + cursor + blockCursor, A[x] + y, sizeof(unsigned long long));

                    blockCursor += 8;
                }
                else
                {
                    break;
                }
            }

            if (blockCursor >= r || cursor + blockCursor >= ret_len)
            {
                // В противном случае завершаем блок
                break;
            }
        }

        cursor += r;

        // KECCAK-f
        sha3_keccak_f(A);
    }
    
    fclose(output_file);
    return ret_len; // Вернуть длину выхода функции (привязана к параметру безопасности)
}