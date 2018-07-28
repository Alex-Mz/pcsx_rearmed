#include <switch.h>

extern void bgr555_to_rgba8888(void *dst, const void *src, int bytes);
extern void draw_string(void *buf, int buf_w, char *str, int x, int y, int r, int g, int b);
