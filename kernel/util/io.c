#include <system.h>
#include <va_list.h>
#include <video.h>

unsigned int max_power(unsigned int num, unsigned int base)
{
    unsigned int rval = 1;
    while (num)
    {
        rval *= base;
        num /= base;
    }
    return rval / base;
}

void put_int(int x)
{
    if (x < 0)
    {
        put_char('-');
        x = abs(x);
    }
    else if (x == 0)
    {
        put_char('0');
        return;
    }
    
    unsigned int pow10 = max_power(x, 10);
    
    while (pow10 > 0)
    {
        unsigned int digit = x / pow10;
        put_char(digit + '0');
        x -= digit * pow10;
        pow10 /= 10;
    }
}

void put_hex(int x)
{
    if (x < 0)
    {
        put_char('-');
        x = abs(x);
    }
    else if (x == 0)
    {
        put_char('0');
        return;
    }
    
    unsigned int pow16 = max_power(x, 16);
    
    while (pow16 > 0) {
        unsigned int digit = x / pow16;
        if (digit < 10) {
            put_char(digit + '0');
        } else {
            put_char(digit - 10 + 'A');
        }
        x -= digit * pow16;
        pow16 /= 16;
    }
}

void put_string(const char *str)
{
    int i = 0;
    int len = strlen(str);
    for (i = 0; i < len; i++)
        put_char(str[i]);
}

extern void kvprintf(const char *format, va_list args)
{
    int i = 0;
    int len = strlen(format);
    for (i = 0; i < len && format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            put_char(format[i]);
            continue;
        }
        
        ++i;
        
        switch(format[i])
        {
            case 's':
                put_string((char*)va_arg(args, char*));
                break;
            case 'c':
                put_char((char)va_arg(args, int));
                break;
            case 'd':
                put_int((int)va_arg(args, int));
                break;
            case 'x':
                put_hex((int)va_arg(args, int));
                break;
            case '%':
                put_char('%');
                break;
            default:
                put_char(format[i]);
                break;
        }
    }
}

extern void kprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    kvprintf(format, args);
    va_end(args);
}
