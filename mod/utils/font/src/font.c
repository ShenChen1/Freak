#include "font.h"
#include "common.h"
#include "font-hdr.h"
#include "log.h"

typedef struct {
    font_file_hdr_t *fileHdr;
    font_char_hdr_t *charHdr;
    char *data;
    char *buf;
} font_priv;

void *font_init(const char *path)
{
    font_priv *priv = NULL;

    priv = malloc(sizeof(font_priv));
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, sizeof(font_priv));
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        goto err;
    }

    fseek(fp, 0, SEEK_END);
    size_t filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    priv->buf = malloc(filesize);
    fread(priv->buf, filesize, 1, fp);
    fclose(fp);

    priv->fileHdr = (void *)priv->buf;
    priv->charHdr = (void *)(priv->buf + sizeof(font_file_hdr_t));
    priv->data    = (void *)(priv->buf + sizeof(font_file_hdr_t) + priv->fileHdr->number * sizeof(font_char_hdr_t));
    return priv;

err:
    free(priv);
    return NULL;
}

int font_deinit(void *font)
{
    font_priv *priv = font;

    if (priv == NULL) {
        return -1;
    }

    free(priv->buf);
    free(priv);

    return 0;
}

static int __binary_search(const font_char_hdr_t arr[], int start, int end, uint32_t key)
{
    int ret = -1;

    int mid;
    while (start <= end) {
        mid = start + (end - start) / 2;
        if (arr[mid].code < key)
            start = mid + 1;
        else if (arr[mid].code > key)
            end = mid - 1;
        else {
            ret = mid;
            break;
        }
    }

    return ret;
}

static int read_bit(char *data, int num)
{
    int base  = num / 8;
    int shift = num % 8;

    return (data[base] >> (7 - shift)) & 0x1;
}

#ifdef DEBUG
static void __show_matrix(font_pic_t *pic)
{
    int i, j;
    printf("width:%u, height:%u\n", pic->width, pic->height);
    for (j = 0; j < pic->height; j++) {
        for (i = 0; i < pic->width; i++) {
            if (read_bit(pic->data, j * pic->width + i)) {
                printf("0");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}
#endif

static int __merge_matrix(void *font, char *buf, uint32_t width, int index, uint32_t start)
{
    int i, j;
    font_priv *priv          = font;
    font_char_hdr_t *charHdr = &priv->charHdr[index];

    for (j = 0; j < charHdr->height; j++) {
        for (i = 0; i < charHdr->width; i++) {
            buf[start + j * width + i] = read_bit(&priv->data[charHdr->offset], j * charHdr->width + i);
        }
    }

    return 0;
}

static int __scale_matrix(void *font, font_pic_t *pic, uint32_t size)
{
    return 0;
}

int font_text(void *font, const char *text, uint32_t size, font_pic_t *pic)
{
    int i, index;
    uint32_t code;
    uint32_t width, height, start;
    font_priv *priv = font;
    char *databuf   = NULL;
    size_t datablen = 0;

    if (priv == NULL || text == NULL || pic == NULL) {
        return -1;
    }

    // 1. parse text to get the string length, so that we calculate width & height
    for (width = height = 0, i = 0;; i += 2) {
        code = (text[i] << 8) + text[i + 1];
        debugf("code:0x%x", code);
        if (!code) {
            break;
        }
        index  = __binary_search(priv->charHdr, 0, priv->fileHdr->number, code);
        height = max(height, priv->charHdr[index].height);
        width += priv->charHdr[index].width;
    }

    if (width == 0) {
        return -2;
    }

    // 2. allocate memroy depending on width & height
    datablen = width * height * sizeof(char);
    databuf  = realloc(pic->data, datablen);

    // 3. get the matrix one by one depending on unicode to build the picture
    for (start = 0, i = 0;; i += 2) {
        code = (text[i] << 8) + text[i + 1];
        if (!code) {
            break;
        }
        index = __binary_search(priv->charHdr, 0, priv->fileHdr->number, code);
#ifdef DEBUG
        pic->data   = &priv->data[priv->charHdr[index].offset];
        pic->width  = priv->charHdr[index].width;
        pic->height = priv->charHdr[index].height;
        __show_matrix(pic);
#endif
        __merge_matrix(font, databuf, width, index, start);
        start += priv->charHdr[index].width;
    }

    pic->data   = databuf;
    pic->width  = width;
    pic->height = height;

    // 4. scale the picture depending on font size
    __scale_matrix(font, pic, size);
    return 0;
}
