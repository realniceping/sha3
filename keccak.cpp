#include "sha3.h";
#include <stdint.h>

void Theta(uint64_t s[25]) {
    int i, j, round;
    uint64_t t, bc[5];

    for (i = 0; i < 5; i++)
        bc[i] = s[i] ^ s[i + 5] ^ s[i + 10] ^ s[i + 15] ^ s[i + 20];

    for (i = 0; i < 5; i++) {
        t = bc[(i + 4) % 5] ^ SHA3_ROTL64(bc[(i + 1) % 5], 1);
        for (j = 0; j < 25; j += 5)
            s[j + i] ^= t;
    }
}

void Rho(uint64_t s[25]) {

}

void Pi(uint64_t s[25]) {

}

void CHi(uint64_t s[25]) {

}

void Iota(uint64_t s[25]) {

}