#include "bmp.h"

static BMP_img_t *init_BMP_img(BMPheader *header, pixel_t **data)
{
    BMP_img_t *img = malloc(sizeof(BMP_img_t));
    if (!img)
        return NULL;
    img->header = header;
    img->data = data;
    return img;
}

static int get_shift(int width)
{
    return (4 - (width * POINT_SIZE) % 4) % 4;
}

static char *make_array2(size_t el_size, int height, int width)
{
    char *array = malloc(el_size * height * width);
    if (!array)
        return NULL;
    char *cur = array;
    char **arr = malloc(sizeof(char *) * height);
    if (!arr)
    {
        free(array);
        return NULL;
    }
    for (int i = 0; i < height; ++i)
    {
        arr[i] = cur;
        cur += width * el_size;
    }
    return (char *)arr;
}

static void free_array2(pixel_t **arr)
{
    free(arr[0]);
    free(arr);
}

void free_img(BMP_img_t *img)
{
    if (!img)
        return;
    free(img->header);
    free_array2(img->data);
    free(img);
}

static void calc_header(BMP_img_t *img, int width, int height)
{
    int image_size = (width * POINT_SIZE + get_shift(width)) * height;
    int file_size = img->header->bfOffBits + image_size;
    img->header->biWidth = width;
    img->header->biHeight = height;
    img->header->biSizeImage = image_size;
    img->header->bfSize = file_size;
}

BMP_img_t *loadBMP(const char *fname)
{
    FILE *file = fopen(fname, "rb");
    if (!file)
        return NULL;
    BMPheader *bh = malloc(sizeof(BMPheader));
    if (!fread(bh, 1, sizeof(BMPheader), file))
    {
        fclose(file);
        free(bh);
        return NULL;
    }
    int width = bh->biWidth, height = bh->biHeight;
    int shift = get_shift(width);
    pixel_t **data = (pixel_t **)make_array2(sizeof(pixel_t), height, width);
    if (!data)
    {
        fclose(file);
        free(bh);
        return NULL;
    }
    for (int i = height - 1; i >= 0; --i)
    {
        if (!fread(data[i], sizeof(pixel_t), width, file))
        {
            fclose(file);
            free(bh);
            return NULL;
        }
        fseek(file, shift, SEEK_CUR);
    }
    fclose(file);
    BMP_img_t *img = init_BMP_img(bh, data);
    if (!img)
    {
        free(bh);
        free_array2(data);
        return NULL;
    }
    return img;
}

BMP_img_t *crop(BMP_img_t *img, pair_t lh, pair_t wh)
{
    if (!img)
        return NULL;
    int width = wh.x;
    int height = wh.y;
    pixel_t **data = (pixel_t **)make_array2(sizeof(pixel_t), height, width);
    if (!data)
    {
        return NULL;
    }
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            data[i][j] = (img->data)[lh.y + i][lh.x + j];
    BMPheader *header = malloc(sizeof(BMPheader));
    if (!header)
    {
        free_array2(data);
        return NULL;
    }
    memcpy(header, img->header, sizeof(BMPheader));
    BMP_img_t *crop_img = init_BMP_img(header, data);
    if (!crop_img)
    {
        free(header);
        free_array2(data);
        return NULL;
    }
    calc_header(crop_img, width, height);
    return crop_img;
}

BMP_img_t *rotate(BMP_img_t *img)
{
    if (!img || !img->header || !img->data)
        return NULL;
    int width, width_rot, height, height_rot;
    width_rot = height = img->header->biHeight;
    height_rot = width = img->header->biWidth;
    pixel_t **data = (pixel_t **)make_array2(sizeof(pixel_t), height_rot, width_rot);
    if (!data)
        return NULL;
    for (int i = 0; i < height_rot; ++i)
        for (int j = 0; j < width_rot; ++j)
            data[i][j] = img->data[height - 1 - j][i];
    printf("Read success\n.\n.\n");
    BMPheader *header = malloc(sizeof(BMPheader));
    if (!header)
    {
        free_array2(data);
        return NULL;
    }
    memcpy(header, img->header, sizeof(BMPheader) * 1);
    BMP_img_t *rot_img = init_BMP_img(header, data);
    if (!rot_img)
    {
        free(header);
        free_array2(data);
        return NULL;
    }
    calc_header(rot_img, width_rot, height_rot);
    return rot_img;
}

int saveBMP(const char *fname, BMP_img_t *img)
{
    if (!img)
        return 1;
    FILE *file = fopen(fname, "wb");
    if (!file)
        return 1;

    if (!fwrite(img->header, sizeof(BMPheader), 1, file))
    {
        fclose(file);
        return 1;
    }
    int width = img->header->biWidth;
    int height = img->header->biHeight;
    int shift = get_shift(img->header->biWidth);
    for (int i = height - 1; i >= 0; --i)
    {
        if (!fwrite(img->data[i], sizeof(pixel_t), width, file))
        {
            fclose(file);
            return 1;
        }
        for (int j = 0; j < shift; ++j)
            putc('\0', file);
    }
    fclose(file);
    return 0;
}


