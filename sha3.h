#ifndef SHA3_H	
#define SHA3_H	

#define KECCAK_ROUNDS 24

#define SHA3_L 6
#define SHA3_NR (12 + 2 * SHA3_L)
#define SHA3_B (25 * << SHA3_L)

#define SHA3_KECCAK_SPONGE_WORDS (((1600)/8)/sizeof(uint64_t))


#define DEBUG 
#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

extern unsigned int sha3_rootConst[5][5];

extern unsigned long long sha3_roundConsts[SHA3_NR];
static const unsigned keccakf_rotc[24];

static void keccakf(uint64_t s[25]);

#endif


#ifndef SHA3_ROTL64
#define SHA3_ROTL64(x, y) \
	(((x) << (y)) | ((x) >> ((sizeof(uint64_t)*8) - (y))))
#endif
