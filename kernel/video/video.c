#include <system.h>

#define VIDEO_RAM_LOCATION 0xB8000

const int ROW_WIDTH = 80;
int video_row = 0;
int video_col = 0;

void next_line()
{
    video_row++;
    video_col = 0;
}

void next_char()
{
    video_col++;
    if (video_col % ROW_WIDTH == 0) {
        video_col = 0;
        video_row++;
    }
}

void put_char(char c)
{
    volatile unsigned char *videoram = (unsigned char*)VIDEO_RAM_LOCATION;
    
    if (c == '\n') {
        next_line();
        return;
    }
    
    videoram[(video_row * ROW_WIDTH * 2) + video_col * 2] = c;
    videoram[(video_row * ROW_WIDTH * 2) + video_col * 2 + 1] = 0x07;
    
    next_char();
}

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

void clear()
{
    volatile unsigned char *videoram = (unsigned char*)VIDEO_RAM_LOCATION;
    int i = 0;
    for (i = 0; i < ROW_WIDTH * 50; i++)
        videoram[i] = 0;
}

void put_string(const char *str)
{
    int len = strlen(str);
    int i = 0;
    for (i = 0; i < len; i++)
        put_char(str[i]);
}
