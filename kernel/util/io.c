#include <system.h>
#include <va_list.h>
#include <video.h>

unsigned int max_power_of_ten(unsigned int num)
{
    unsigned int rval = 1;
    while (num)
    {
        rval *= 10;
        num /= 10;
    }
    return rval;
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
    
    unsigned int pow10 = max_power_of_ten(x) / 10;
    
    while (pow10 > 0)
    {
        unsigned int digit = x / pow10;
        put_char(digit + '0');
        x -= digit * pow10;
        pow10 /= 10;
    }
}

void put_string(const char *str)
{
    int i = 0;
    int len = strlen(str);
    for (i = 0; i < len; i++)
        put_char(str[i]);
}

extern void kprintf(const char *format, ...)
{
    int i = 0;
    int len = strlen(format);
    va_list args;
    va_start(args, format);
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
            case '%':
                put_char('%');
                break;
            default:
                put_char(format[i]);
                break;
        }
    }
    va_end(args);
}
