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

void clear()
{
    volatile unsigned char *videoram = (unsigned char*)VIDEO_RAM_LOCATION;
    int i = 0;
    for (i = 0; i < ROW_WIDTH * 50; i++)
        videoram[i] = 0;
}

void reset()
{
    clear();
    video_row = 0;
    video_col = 0;
}
