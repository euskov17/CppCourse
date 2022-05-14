#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define POINT_SIZE 3

#ifndef _BMP_H_
#define _BMP_H_

typedef struct pair_t
{
    int x, y;
} pair_t;

typedef __int16_t WORD;
typedef __int32_t DWORD;

#pragma pack(push, 2)

typedef struct
{
    WORD bfType;         // 0x4d42 | 0x4349 | 0x5450
    int bfSize;          // размер файла
    int bfReserved;      // 0
    int bfOffBits;       // смещение до поля данных,
                         // обычно 54 = 16 + biSize
    int biSize;          // размер струкуры в байтах:
                         // 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER)
                         // или 124(BITMAPV5HEADER)
    int biWidth;         // ширина в точках
    int biHeight;        // высота в точках
    WORD biPlanes;       // всегда должно быть 1
    WORD biBitCount;     // 0 | 1 | 4 | 8 | 16 | 24 | 32
    int biCompression;   // BI_RGB | BI_RLE8 | BI_RLE4 |
                         // BI_BITFIELDS | BI_JPEG | BI_PNG
                         // реально используется лишь BI_RGB
    int biSizeImage;     // Количество байт в поле данных
                         // Обычно устанавливается в 0
    int biXPelsPerMeter; // горизонтальное разрешение, точек на дюйм
    int biYPelsPerMeter; // вертикальное разрешение, точек на дюйм
    int biClrUsed;       // Количество используемых цветов
                         // (если есть таблица цветов)
    int biClrImportant;  // Количество существенных цветов.
                         // Можно считать, просто 0
} BMPheader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    char R,G,B;
} pixel_t;
#pragma pack(pop)

typedef struct
{
    BMPheader *header;
    pixel_t **data;
} BMP_img_t;

BMP_img_t *loadBMP(const char *fname);

int saveBMP(const char *fname, BMP_img_t *img);

BMP_img_t *rotate(BMP_img_t *);

BMP_img_t *crop(BMP_img_t *, pair_t lh, pair_t rh);

void free_img(BMP_img_t *img);


#endif