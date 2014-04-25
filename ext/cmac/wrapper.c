#include <ruby.h>
#include "cmac.h"

// Top-level objects for the native extension
static VALUE cmac_rb;
static VALUE cmac_rb_digest;

/* 
 * Initialize an CMAC::Digest object with a key.
 * Performs basic length validation on the key.
 */
static VALUE cmac_rb_initialize(VALUE self, VALUE key) {
 
  int keyLen;
  
  // Replace key value with key.to_str
  StringValue(key);
  
  // Get the key length as an int.
  keyLen = RSTRING_LEN(key);
  
  // Make sure key is not empty
  if (keyLen == 0) {
    rb_raise(rb_eArgError, "Key must be non-empty.");
  }
  
  // Make sure key is acceptable size
  if (keyLen != AES_BLOCK_SIZE) {
    rb_raise(rb_eArgError, "Only 128-bit keys are supported.");
  }
  
  // Set key as instance variable
  rb_iv_set(self, "@key", key);
  
  return self;
  
}

/*
 * Get an cmac_ctx object for the current instance
 * by fetching the @key instance variable, converting
 * it to a byte array and feeding it into cmac_init.
 *
 * Returns 1 upon success, 0 upon failure.
 */
static int cmac_rb_get_ctx(VALUE self, cmac_ctx* ctx) {
  
  VALUE key; unsigned char* cKey; int cKeyLen;
  
  // Get the key instance variable
  key = rb_iv_get(self, "@key");
  
  // Convert the key to a byte array
  cKey = (unsigned char*) RSTRING_PTR(key);
  cKeyLen = RSTRING_LEN(key);
  
  // Initialize the context with the key
  if (cmac_init(ctx, cKey, cKeyLen * 8) < 0) {
    return 0;
  }
  
  // Return 1 upon successful initialization
  return 1;
  
}

/*
 * Compute the digest of some plaintext
 */
static VALUE cmac_rb_update(VALUE self, VALUE plaintext) {
  
  // Holds the CMAC context object.
  cmac_ctx ctx;
  
  // Input plaintext as byte array.
  const unsigned char* cPlaintext;
  
  // Length of the input plaintext.
  int cPlaintextLen;
  
  // Holds the CMAC ciphertext object.
  unsigned char* cDigest;
  
  // Holds the length of the CMAC digest.
  int cDigestLen;
  
  // Get the CMAC context based on the instance's key.
  if (!cmac_rb_get_ctx(self, &ctx)) {
    rb_raise(rb_eRuntimeError, "Could not get CMAC context");
  }
  
  // Replace the plaintext with plaintext.to_str
  StringValue(plaintext);
  
  // Convert the plaintext to a byte array.
  cPlaintext = (const unsigned char*) RSTRING_PTR(plaintext);
  cPlaintextLen = RSTRING_LEN(plaintext);
  
  // Allocate space for the ciphertext.
  cDigestLen = sizeof(unsigned char) * AES_BLOCK_SIZE;
  cDigest = (unsigned char*) malloc(cDigestLen);
  
  // Call aes_cmac with all parameters.
  cmac_encrypt(&ctx, cPlaintext, cPlaintextLen, cDigest);
  
  // Return a new Ruby string with the resulting value.
  return rb_str_new((const char*) cDigest, cDigestLen);
  
}

/*
 * Main wrapper for the CMAC Ruby native extension.
 */
void Init_wrapper(void) {
  
  // Define the top-level module
	cmac_rb = rb_define_module("CMAC");
	
	// Define the cipher class
	cmac_rb_digest = rb_define_class_under(cmac_rb, "Digest", rb_cObject);
	
	// Define the implemented methods.
	rb_define_method(cmac_rb_digest, "initialize", cmac_rb_initialize, 1);
	rb_define_method(cmac_rb_digest, "update", cmac_rb_update, 1);
	
  return;
	
}

/*
Debug helper method to print byte arrays in hex format.

void print_hex(const char* header, const unsigned char *bytes, int len) {
  
  int i = 0; printf("\n%s (%d): ", header, len);
  for (i = 0; i < len; ++i) printf("%x", bytes[i]);
  printf("\n");
  
}
*/