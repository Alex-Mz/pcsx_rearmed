#include "pixels_operation.h"
#include "fontdata.h"
#include <string.h>

void bgr888_to_rgba8888(void *dst, const void *src, int bytes) {
    u32 *d = dst;
    const u8 *s = src;
    
    for (int x = 0; x < bytes; x++, s += 3, d++) {
        u8 r = *(s + 0);
        u8 g = *(s + 1);
        u8 b = *(s + 2);
        *d = RGBA8(r, g, b, 0xFF);
    }
}

void bgr555_to_rgba8888(void *dst, const void *src, int bytes) {
    u32 *d = dst;
    const u16 *s = src;
    
    for (int x = 0; x < bytes; x++, s++, d++) {
        u16 color = *s;
        u8 r = (color & 0x1F);
        u8 g = (color & 0x3E0) >> 5;
        u8 b = (color & 0x7C00) >> 10;
        *d = RGBA8(r << 3, g << 3, b << 3, 0xFF);
    }
}

static inline void draw_char(void *buf, int buf_w, int x, int y, unsigned char a, int r, int g, int b) {
    for (int h = 8; h; h--) {
        u32 *dst = (u32 *)buf + (y + 8 - h) * buf_w + x;
        for (int w = 8; w; w--) {
            u32 color = *dst;
            
            if ((fontdata8x8[a * 8 + (8 - h)] >> w) & 1) color = RGBA8(r, g, b, 0xFF);
            *dst++ = color;
        }
    }
}

void draw_string(void *buf, int buf_w, char *str, int x, int y, int r, int g, int b) {
    size_t len = strlen(str);
    for (int i = 0; i < len; i++, x += 6) {
        draw_char(buf, buf_w, x, y, str[i], r, g, b);
    }
}
