#include "fb.h"

#include <lib/font.h>
#include <lib/types.h>
#include <mem/mem.h>

static u32 *fb;

static u64 fb_height, fb_width;
static u32 fg_color = DEFAULT_FG, bg_color = DEFAULT_BG;
static u64 print_x = 0, print_y = 0;

static void plot_pixel(u64 x, u64 y, u32 color)
{
    fb[y * fb_width + x] = color;
}

static void put_literal_char(i32 x, i32 y, u8 c)
{
    for (i32 ly = 0; ly < FONT_HEIGHT; ly++)
    {
        for (i32 lx = 0; lx < FONT_WIDTH; lx++)
        {
            if (font8x8_basic[c][ly] & (1 << lx))
                plot_pixel(x + lx, y + ly, fg_color);
            else
                plot_pixel(x + lx, y + ly, bg_color);
        }
    }
}

void putchar(char c)
{
    put_literal_char(print_x, print_y, c);
    print_x += FONT_WIDTH;

    if (print_x >= fb_width || c == '\n')
    {
        print_x = 0;
        print_y += FONT_HEIGHT;
    }
    if (print_y >= fb_height)
    {
        for (u64 i = fb_width * FONT_HEIGHT; i < fb_height * fb_width; i++)
            fb[i - fb_width * FONT_HEIGHT] = fb[i];
        for (u64 i = (fb_height - FONT_HEIGHT) * fb_width; i < fb_height * fb_width; i++)
            fb[i] = bg_color;

        print_y = fb_height - FONT_HEIGHT;
    }
}

void putstr(char *s)
{
    for (char *p = s; *p; p++)
        putchar(*p);
}

void init_fb(struct limine_framebuffer *limine_framebuffer)
{
    fb = limine_framebuffer->address;
    fb_width = limine_framebuffer->width;
    fb_height = limine_framebuffer->height;

    for (u64 i = 0; i < (fb_width * fb_height); i++)
        fb[i] = bg_color;
}
