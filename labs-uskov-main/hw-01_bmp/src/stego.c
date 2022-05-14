#include "stego.h"
static char code(char sym)
{
    switch (sym)
    {
    case ' ':
        return 26;
    case '.':
        return 27;
    case ',':
        return 28;
    default:
        return sym - 'A';
    }
}

static char decode(char sym)
{
    switch (sym)
    {
    case 26:
        return ' ';
    case 27:
        return '.';
    case 28:
        return ',';
    default:
        return sym + 'A';
    }
}

int insert(BMP_img_t *img, const char *key_file, const char *msg_txt)
{
    FILE *file_key = fopen(key_file, "r");
    if (!file_key)
        return 1;
    FILE *file_msg = fopen(msg_txt, "r");
    if (!file_msg)
    {
        fclose(file_key);
        return 1;
    }
    char cur_c;
    while (!feof(file_key))
    {
        if ((cur_c = getc(file_msg)) == EOF || cur_c == '\n')
            break;
        cur_c = code(cur_c);
        for (int i = 0; i < 5; ++i)
        {
            char msk = 1 & (cur_c >> i);
            int x, y;
            char colour;
            if (!fscanf(file_key, "%d %d %c", &x, &y, &colour))
            {
                fclose(file_msg);
                fclose(file_key);
                return 1;
            }
            if (x >= img->header->biWidth || y >= img->header->biHeight)
            {
                fclose(file_msg);
                fclose(file_key);
                return 1;
            }
            switch (colour)
            {
            case 'R':
                img->data[y][x].R = (img->data[y][x].R & (~1)) | msk;
                break;
            case 'G':
                img->data[y][x].G = (img->data[y][x].G & (~1)) | msk;
                break;
            case 'B':
                img->data[y][x].B = (img->data[y][x].B & (~1)) | msk;
                break;
            default:
                break;
            }
        }
    }
    fclose(file_msg);
    fclose(file_key);
    return 0;
}

int extract(BMP_img_t *img, const char *key_file, const char *txt_file)
{
    FILE *file_key = fopen(key_file, "r");
    if (!file_key)
        return 1;
    FILE *file_txt = fopen(txt_file, "w");
    if (!file_txt)
    {
        fclose(file_key);
        return 1;
    }
    char cur_c;
    while (!feof(file_key))
    {
        for (int i = 0; i < 5; ++i)
        {
            int x, y;
            char colour;
            if (!fscanf(file_key, "%d %d %c", &x, &y, &colour))
            {
                fclose(file_txt);
                fclose(file_key);
                return 1;
            }
            switch (colour)
            {
            case 'R':
                cur_c |= (img->data[y][x].R & 1) << i;
                break;
            case 'G':
                cur_c |= (img->data[y][x].G & 1) << i;
                break;
            case 'B':
                cur_c |= (img->data[y][x].B & 1) << i;
                break;
            default:
                break;
            }
        }
        putc(decode(cur_c), file_txt);
    }
    fclose(file_txt);
    fclose(file_key);
    return 0;
}