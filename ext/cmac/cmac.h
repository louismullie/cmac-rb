#ifndef _CMAC_H_
#define _CMAC_H_

#include <openssl/aes.h>

typedef struct _cmac_ctx {
  unsigned char K1[AES_BLOCK_SIZE];
  unsigned char K2[AES_BLOCK_SIZE];
  unsigned char T[AES_BLOCK_SIZE];
  AES_KEY cmac_key;
} cmac_ctx;

int cmac_init(cmac_ctx * ctx, const unsigned char * key, int key_len);
void cmac_encrypt (cmac_ctx * ctx, const unsigned char * msg, int msg_len, unsigned char * ct);

#endif