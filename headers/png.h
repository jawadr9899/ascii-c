#pragma once
#include <stdio.h>

typedef struct {
  const char *colorType;
  int channels;
} ColorType;

typedef struct {
  FILE *ptr;
  const char *name;
  int width;
  int height;
  int pixels;
  int chunkLength;
  ColorType colorData;
  char chunkType[5];
  int bitDepth;
} PNG;

const char *getColorType(int colorType);
const unsigned int getChannels(int colorType);
void printPNG(PNG *im);
unsigned char *getRawDataPos(PNG *im);