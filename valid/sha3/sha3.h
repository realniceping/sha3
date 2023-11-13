#ifndef SHA3_H
#define SHA3_H

// Определить базовые константы для kecchak-p
#define SHA3_L 6
#define SHA3_NR (12 + 2 * SHA3_L)
#define SHA3_B (25 << SHA3_L)

#define SHA3_128 128
#define SHA3_256 256
#define SHA3_512 512


extern unsigned int sha3_rotConst[5][5];
extern unsigned long long sha3_roundConsts[SHA3_NR];

unsigned long long leftRotateLL(unsigned long long w, unsigned int d);

//необходимые функции нахождения минимального из двух чисел и максимального
#define MAX(i1, i2) (i1 > i2 ? i1 : i2) 
#define MIN(i1, i2) (i1 < i2 ? i1 : i2)



// kecchak signatures
/*
* Cигнатура функции keccak
* @param A State array из функции keccak
*/
void sha3_keccak_f(unsigned long long A[5][5]);

/*
*sha3 hash function
* @param in Входные данные для хеширования (открытый текст)
* @param n Длинна сообщения
* @param mode Уровень защищенности (512 256 128) 
* @param out Выход функции (хешированное сообщение)
*/
int sha3_hash(unsigned char* in, int n, int mode, unsigned char **out);

#endif