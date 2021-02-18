#ifndef _FONT_H_
#define _FONT_H_

#include <stdint.h>

typedef struct {
    uint32_t width;     /* pic's width */
    uint32_t height;    /* pic's height */
    char *data;         /* Address of pic's data */
} font_pic_t;

void *font_init(const char *path);
int font_deinit(void *font);
int font_text(void *font, const char *text, uint32_t size, font_pic_t *pic);

#endif /* _FONT_H_ */