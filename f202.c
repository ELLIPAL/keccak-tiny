#include "k.h"
#include "f202.h"
#include "u.h"

#include <stdint.h>
#include <stdlib.h>

FN clear(bytes a) { FOR(i, 1, 200, a[i] = 0); }

mkapply(xorin, dst[i] ^= src[i])
mkapply(setout, src[i] = dst[i])

SIE hash(bytes out, size outlen, bytes in, size inlen, size rate, byte delim) {
  if ((out == NULL) || ((in == NULL) && inlen != 0) || (rate >= Plen)) {
    return -1;
  }
  byte a[Plen] = {0};
  // Absorb input.
  foldP(in, inlen, xorin);
  // Xor in the DS and pad frame.
  a[inlen] ^= delim;
  a[rate - 1] ^= 0x80;
  // Xor in the last block.
  xorin(a, in, inlen);
  // Apply P
  P(a);
  // Squeeze output.
  foldP(out, outlen, setout);
  setout(a, out, outlen);
  clear(a);
  return 0;
}

#define defshake(bits)                                            \
  F2 shake##bits(bytes out, size outlen, bytes in, size inlen) { \
    return hash(out, outlen, in, inlen, 200 - (bits / 4), 0x1f);  \
  }

defshake(128)
defshake(256)

#define defsha3(bits)                                            \
  F2 sha3_##bits(bytes out, size outlen, bytes in, size inlen) { \
    /*if (outlen != (bits/8)) { return -1; } */\
    return hash(out, outlen, in, inlen, 200 - (bits / 4), 0x06);  \
  }

defsha3(224)
defsha3(256)
defsha3(384)
defsha3(512)
