#include <string.h>
#include <stdio.h>
#include "bmp.h"
#include "stego.h"

typedef struct content_t
{
    char *action;
    const char *in_bmp;
    const char *out_bmp;

    BMP_img_t *img;
} content_t;

int check_size(pair_t left_high, pair_t width_height, int W, int H)
{
    int x = left_high.x, y = left_high.y;
    int w = width_height.x, h = width_height.y;
    if (x < 0 || x + w > W || w < 0 || h < 0 || y < 0 || y + h > H)
    {
        printf("Out of bound img\n");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 8 && argc != 6 && argc != 5)
        return 1;
    content_t content = {
        .action = argv[1],
        .in_bmp = argv[2],
        .out_bmp = argv[3]};
    if (argc == 8)
    {
        if (strcmp(content.action, "crop-rotate"))
            return 1;
        pair_t left_high = {
            .x = atoi(argv[4]),
            .y = atoi(argv[5])};
        pair_t width_height = {
            .x = atoi(argv[6]),
            .y = atoi(argv[7])};
        content.img = loadBMP(content.in_bmp);
        if (content.img && check_size(left_high, width_height, content.img->header->biWidth, content.img->header->biHeight))
        {
            free_img(content.img);
            return 1;
        }
        BMP_img_t *crop_img = crop(content.img, left_high, width_height);
        BMP_img_t *rot_img = rotate(crop_img);
        int k = saveBMP(content.out_bmp, rot_img);
        free_img(content.img);
        free_img(crop_img);
        free_img(rot_img);
        printf("All is correct k = %d", k);
        if (k)
            return 1;
    }
    else if (argc == 6)
    {
        if (strcmp(content.action, "insert"))
            return 1;
        content.img = loadBMP(content.in_bmp);
        if (!content.img)
            return 1;
        if (insert(content.img, argv[4], argv[5]))
        {
            free_img(content.img);
            return 1;
        }
        if (saveBMP(content.out_bmp, content.img))
        {
            free_img(content.img);
            return 1;
        }
        free_img(content.img);
    }
    else if (argc == 5)
    {
        if (strcmp(content.action, "extract"))
            return 1;
        content.img = loadBMP(content.in_bmp);
        if (!content.img)
            return 1;
        if (extract(content.img, argv[3], argv[4]))
        {
            free_img(content.img);
            return 1;
        }
        free_img(content.img);
    }
    return 0;
}