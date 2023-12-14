#ifndef SHA3_H
#define SHA3_H

// Определить базовые константы для kecchak-p
#define SHA3_L 6
#define SHA3_NR (12 + 2 * SHA3_L)
#define SHA3_B (25 << SHA3_L)


#define SHA3_224 224
#define SHA3_256 256
#define SHA3_384 384
#define SHA3_512 512

#define nist_1600_test_char 0b11000101

extern unsigned int sha3_rotConst[5][5];
extern unsigned long long sha3_roundConsts[SHA3_NR];

//необходимые функции нахождения минимального из двух чисел и максимального
#define MAX(i1, i2) (i1 > i2 ? i1 : i2) 
#define MIN(i1, i2) (i1 < i2 ? i1 : i2)

void printULLbin(unsigned long long a);
void printULLhex(unsigned long long a);


/*
* @param a Символ в кодировке ASCII, возвращает его значение от 0x0 до 0xF
* Вспомогательная функция для преобразования шестнадцатиричного числа из строки ASCII в байтовую строку
*/
unsigned char hexArrFind(char a);

/*
* Выводит массив типа char на консоль
* @param arr Массив, который необходимо вывести
* @param len Длинна массива
* @param separate Необходимо ли ставить пробел между битами, 1 или 0
*/
void printCharArr(unsigned char *arr, int len, int separate);


/*
* Выводит состояние функции keccak на консоль так, как это представлено в тестовом файле fips.
* 16 байт в строке, с пробелами
* @param A Массив состояния keccak 
*/
void printKeccakStateArray(unsigned long long A[5][5]);

/*
* Осущетсвляет поворот 64 битного слова в сторону MSB
* @param w 64 bits Word, слово, которое необохдимо "провернуть"
* @param d  Значение на которое слово необходимо повернуть
*/
unsigned long long sha3_rotWord(unsigned long long w, unsigned int d);


// kecchak signatures
/*
* Cигнатура функции keccak. В нашем случае функция keccak реализуется со state array = 1600 и числом раундов 24
* @param A State array из функции keccak
*/
void sha3_keccak_f(unsigned long long A[5][5]);

/*
* sha3 Хеш функция
* @param in Входные данные для хеширования (открытый текст)
* @param n Длинна сообщения
* @param mode Уровень защищенности (512 256 128) 
* @param out Выход функции (хешированное сообщение)
*/
int sha3_hash(unsigned char* in, int n, int mode, unsigned char **out);

#endif