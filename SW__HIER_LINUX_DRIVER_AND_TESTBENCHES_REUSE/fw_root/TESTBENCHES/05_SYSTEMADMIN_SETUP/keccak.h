#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>


#define SHA3_ASSERT( x ) 
#if defined(_MSC_VER) 
#define SHA3_TRACE( format, ...) 
#define SHA3_TRACE_BUF( format, buf, l, ...) 
#else 
#define SHA3_TRACE(format, args...) 
#define SHA3_TRACE_BUF(format, buf, l, args...) 
#endif 


//#define SHA3_USE_KECCAK 
/*  
 * Define SHA3_USE_KECCAK to run "pure" Keccak, as opposed to SHA3. 
 * The tests that this macro enables use the input and output from [Keccak] 
 * (see the reference below). The used test vectors aren't correct for SHA3,  
 * however, they are helpful to verify the implementation. 
 * SHA3_USE_KECCAK only changes one line of code in Finalize. 
 */ 


#if defined(_MSC_VER) 
#define SHA3_CONST(x) x 
#else 
#define SHA3_CONST(x) x##L 
#endif 


/* The following state definition should normally be in a separate  
 * header file  
 */ 


/* 'Words' here refers to uint64_t */ 
#define SHA3_KECCAK_SPONGE_WORDS (((1600)/8/*bits to byte*/)/sizeof(uint64_t)) 
typedef struct sha3_context_ { 
    uint64_t saved;             /* the portion of the input message that we 
                                 * didn't consume yet */ 
    union {                     /* Keccak's state */ 
        uint64_t s[SHA3_KECCAK_SPONGE_WORDS]; 
        uint8_t sb[SHA3_KECCAK_SPONGE_WORDS * 8]; 
    }; 
    unsigned byteIndex;         /* 0..7--the next byte after the set one 
                                 * (starts from 0; 0--none are buffered) */ 
    unsigned wordIndex;         /* 0..24--the next word to integrate input 
                                 * (starts from 0) */ 
    unsigned capacityWords;     /* the double size of the hash output in 
                                 * words (e.g. 16 for Keccak 512) */ 
} sha3_context; 


#ifndef SHA3_ROTL64 
#define SHA3_ROTL64(x, y) (((x) << (y)) | ((x) >> ((sizeof(uint64_t)*8) - (y)))) 
#endif 


const uint64_t keccakf_rndc[24] = { 
    SHA3_CONST(0x0000000000000001UL), SHA3_CONST(0x0000000000008082UL), 
    SHA3_CONST(0x800000000000808aUL), SHA3_CONST(0x8000000080008000UL), 
    SHA3_CONST(0x000000000000808bUL), SHA3_CONST(0x0000000080000001UL), 
    SHA3_CONST(0x8000000080008081UL), SHA3_CONST(0x8000000000008009UL), 
    SHA3_CONST(0x000000000000008aUL), SHA3_CONST(0x0000000000000088UL), 
    SHA3_CONST(0x0000000080008009UL), SHA3_CONST(0x000000008000000aUL), 
    SHA3_CONST(0x000000008000808bUL), SHA3_CONST(0x800000000000008bUL), 
    SHA3_CONST(0x8000000000008089UL), SHA3_CONST(0x8000000000008003UL), 
    SHA3_CONST(0x8000000000008002UL), SHA3_CONST(0x8000000000000080UL), 
    SHA3_CONST(0x000000000000800aUL), SHA3_CONST(0x800000008000000aUL), 
    SHA3_CONST(0x8000000080008081UL), SHA3_CONST(0x8000000000008080UL), 
    SHA3_CONST(0x0000000080000001UL), SHA3_CONST(0x8000000080008008UL) 
}; 


const unsigned keccakf_rotc[24] = { 
    1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 2, 14, 27, 41, 56, 8, 25, 43, 62, 
    18, 39, 61, 20, 44 
}; 


const unsigned keccakf_piln[24] = { 
    10, 7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4, 15, 23, 19, 13, 12, 2, 20, 
    14, 22, 9, 6, 1 
}; 


void sha3_Init256(void *priv);
void sha3_Init384(void *priv); 
void sha3_Init512(void *priv);
void sha3_Update(void *priv, void const *bufIn, size_t len);
void const *sha3_Finalize(void *priv); 
