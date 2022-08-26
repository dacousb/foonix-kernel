#include <stdarg.h>

#include <lib/fb.h>
#include <lib/types.h>

static void itoa(char base, u64 i)
{
    if (i == 0)
    {
        putchar('0');
        return;
    }

    char tmp[21] = {0}, *tmpr = &tmp[0];
    base = (base == 'd') ? 10 : 16;

    for (; i > 0; i /= base)
        *(++tmpr) = "0123456789ABCDEF"[i % base];
    while (*tmpr != 0)
        putchar(*(tmpr--));
}

void printf(const char *fmt, ...)
{
    va_list lst;
    va_start(lst, fmt);

    for (u64 i = 0; fmt[i]; i++)
    {
        if (fmt[i] == '%')
        {
            i++;
            switch (fmt[i])
            {
            case 's':
                putstr(va_arg(lst, char *));
                break;
            case 'l':
                i++;
                if (fmt[i] == 'd' || fmt[i] == 'x')
                    itoa(fmt[i], va_arg(lst, u64));
                break;
            case 'd':
            case 'x':
                itoa(fmt[i], va_arg(lst, u32));
                break;
            case '%':
                putchar(fmt[i]);
                break;
            }
        }
        else
            putchar(fmt[i]);
    }

    va_end(lst);
}