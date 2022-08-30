#ifndef __FB_H__
#define __FB_H__

#include <limine.h>

#define DEFAULT_FG 0xffffffff
#define DEFAULT_BG 0x22282a36

void init_fb(struct limine_framebuffer *limine_framebuffer);
void putstr(char *s);
void putchar(char c);

#endif
