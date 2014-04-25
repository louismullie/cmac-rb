#include <stdio.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

#include "cmac.h"

static void cmac_xor (unsigned char *out, const unsigned char *in) {
  
  int i;
  
  for (i = 0; i < AES_BLOCK_SIZE; i++) {
    out[i] ^= in[i];
  }
  
}

static void cmac_pad (unsigned char *buf, int len) {

  int i;
  
  for ( i = len; i < AES_BLOCK_SIZE; i++ ) {
    buf[i] = (i == len) ? 0x80 : 0x00;
  }
  
}

static unsigned char cmac_left_shift(unsigned char *out, unsigned char *in, unsigned char *overflow) {
  
  int i;

  for (i = AES_BLOCK_SIZE - 1; i >= 0; i--) {
    out[i] = (in[i] << 1) | (*overflow);
    (*overflow) = CMAC_MSB(&in[i]);
  }
  
  return;
  
}

static void cmac_generate_sub_key(unsigned char *out, unsigned char *in) {
  
  int i; unsigned char overflow = 0;

  cmac_left_shift(out, in, &overflow);
  
  if (overflow) {
    out[AES_BLOCK_SIZE-1] ^= 0x87;
  }
  
  return;
}


void cmac_encrypt (cmac_ctx *ctx, const unsigned char *msg, int msg_len, unsigned char *ct) {
  
  int n, i, k;
  
  unsigned char M[AES_BLOCK_SIZE];
  unsigned char *cursor = (unsigned char *) msg;

  memcpy(ct, zero_block, AES_BLOCK_SIZE);
  memset(M, 0, AES_BLOCK_SIZE);
  
  n = (msg_len + (AES_BLOCK_SIZE - 1)) / AES_BLOCK_SIZE - 1;
  k = (msg_len % AES_BLOCK_SIZE);
  
  for (i = 0; i < n; i++) {
    cmac_xor(ct, cursor);
    AES_ecb_encrypt(ct, ct, &ctx->cmac_key, AES_ENCRYPT);
    cursor += AES_BLOCK_SIZE;
  }
  
  if (k == 0) {
    if (msg != NULL && msg_len != 0) {
      memcpy(M, cursor, AES_BLOCK_SIZE);
      cmac_xor(M, ctx->K1);
    } else {
      cmac_pad(M, 0); cmac_xor(M, ctx->K2);
    }
  } else {
    memcpy(M, cursor, k);
    cmac_pad(M, k); cmac_xor(M, ctx->K2);
  }
  
  cmac_xor(ct, M);
  
  AES_ecb_encrypt(ct, ct, &ctx->cmac_key, AES_ENCRYPT);
  
  return;
  
}

int cmac_init (cmac_ctx *ctx, const unsigned char *key, int key_len)
{
  
  unsigned char L[AES_BLOCK_SIZE];
  
  memset((char *)ctx, 0, sizeof(cmac_ctx));
  
  AES_set_encrypt_key(key, 128, &ctx->cmac_key);
  AES_ecb_encrypt(zero_block, L, &ctx->cmac_key, AES_ENCRYPT);
  
  cmac_generate_sub_key(ctx->K1, L);
  cmac_generate_sub_key(ctx->K2, ctx->K1);

  cmac_encrypt(ctx, zero_block, AES_BLOCK_SIZE, ctx->T);
  
  return 1;
  
}