#ifndef DIVX_ASI_H
#define DIVX_ASI_H

#define FOURCC(A, B, C, D) (((uint8_t)(A)) | (((uint8_t)(B)) << 8) | (((uint8_t)(C)) << 16) | (((uint8_t)(D) << 24)))

#define MAGIC 12481632
#define RW 320
#define RH 240

struct divx_header {
  int magic;
  int width;
  int height;
  int frames;
};

#endif
