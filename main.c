#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/png.h"
#include "headers/stb_image.h"


const char *getColorType(int colorType);
const unsigned int getChannels(int colorType);
PNG *getPNG(FILE *fp, const char *name);
unsigned char *getRawDataPos(PNG *im);
void printPNG(PNG *im);
void drawIt(unsigned char *image_data, PNG *im);


int main(int argc, const char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "No image path provided!\n");
    exit(0);
  }

  FILE *fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open fp: %s", argv[1]);
    fclose(fp);
    return 0;
  }
  PNG *img = getPNG(fp, argv[1]);
  if (argc == 3 && strcmp(argv[2], "-i") == 0) {
    printPNG(img);
    fclose(fp);
    free(img);
    exit(0);
  }
  unsigned char *image_data = getRawDataPos(img);

  drawIt(image_data, img);

  // freedom
  stbi_image_free(image_data);
  fclose(fp);
  free(img);
  return 0;
}


const char *getColorType(int colorType) {
  switch (colorType) {
  case 0:
    return "Grayscale";
  case 2:
    return "Truecolor";
  case 3:
    return "Indexed";
  case 4:
    return "Grayscale & Alpha";
  case 6:
    return "Truecolor & Alpha";
  default:
    return "Unknown";
  }
}
const unsigned int getChannels(int colorType) {
  switch (colorType) {
  case 0:
    return 1;
  case 2:
    return 3;
  case 3:
    return 1;
  case 4:
    return 2;
  case 6:
    return 4;
  default:
    fprintf(stderr, "Image was forced to .png format [Invalid ColorType]\n");
    return 0;
  }
}
PNG *getPNG(FILE *fp, const char *name) {
  unsigned char header[30];
  fread(header, 1, 30, fp);
  PNG *im = (PNG *)malloc(sizeof(PNG));
  im->ptr = fp;
  im->name = name;
  im->chunkLength = header[11];
  im->width = header[19];
  im->height = header[23];
  int i = 0;
  for (; i < 4; ++i) {
    im->chunkType[i] = header[12 + i];
  }
  im->chunkType[i] = '\0';
  im->pixels = im->width * im->height;
  im->bitDepth = header[24];
  im->colorData.colorType = getColorType(header[25]);
  im->colorData.channels = getChannels(header[25]);
  return im;
};
void printPNG(PNG *im) {
  printf("PNG { \n");
  printf(" name: %s\n", im->name);
  printf(" width: %d\n", im->width);
  printf(" height: %d\n", im->height);
  printf(" pixels: %d\n", im->pixels);
  printf(" chunk-length: %d\n", im->chunkLength);
  printf(" chunk-type: %s\n", im->chunkType);
  printf(" bit-depth: %d\n", im->bitDepth);
  printf(" color-data:{\n  color-type: %s\n  channels: %d\n  }\n",
         im->colorData.colorType, im->colorData.channels);
  printf("}\n");
}

unsigned char *getRawDataPos(PNG *im) {
  unsigned char *image_data =
      stbi_load(im->name, &im->width, &im->height, &im->colorData.channels, 3);
  if (image_data == NULL) {
    printf("Error: Could not load image, but tried.\n");
    exit(0);
  }
  return image_data;
}

void drawIt(unsigned char *image_data, PNG *im) {
  int target_width = 30;
  int target_height = 30;
  const char *ascii_palette = " .:-=+*#%@";
  int palette_size = 10;

  for (int y = 0; y < target_height; y++) {
    for (int x = 0; x < target_width; x++) {
      int src_x = (x * im->width) / target_width;
      int src_y = (y * im->height) / target_height;
      int pixel_index = (src_y * im->width + src_x) * im->colorData.channels;

      unsigned char r = image_data[pixel_index];
      unsigned char g = image_data[pixel_index + 1];
      unsigned char b = image_data[pixel_index + 2];

      int brightness = (0.299 * r) + (0.587 * g) + (0.114 * b);
      int char_index = (brightness * (palette_size - 1)) / 255;

      printf("%c%c", ascii_palette[char_index], ascii_palette[char_index]);
    }
    printf("\n");
  }
}