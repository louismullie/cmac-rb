#ifndef _CMAC_H_
#define _CMAC_H_

#include <openssl/aes.h>

typedef struct _cmac_ctx {
  unsigned char K1[AES_BLOCK_SIZE];
  unsigned char K2[AES_BLOCK_SIZE];
  unsigned char T[AES_BLOCK_SIZE];
  AES_KEY cmac_key;
} cmac_ctx;

#define CMAC_MSB(x) (((x)[0] & 0x80)?1:0)

static unsigned char zero_block[AES_BLOCK_SIZE] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int cmac_init(cmac_ctx * ctx, const unsigned char * key, int key_len);
void cmac_encrypt (cmac_ctx * ctx, const unsigned char * msg, int msg_len, unsigned char * ct);

#endif