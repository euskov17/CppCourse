#include "bmp.h"

#ifndef __STEGO_H__
#define __STEGO_H__

int insert(BMP_img_t *img, const char *key_file, const char *msg_txt);

int extract(BMP_img_t *img, const char *key_file, const char *txt_file);

#endif