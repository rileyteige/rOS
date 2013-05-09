#include <system.h>

#define VIDEO_RAM_LOCATION 0xB8000;

uint8_t* videoram = (uint8_t*)VIDEO_RAM_LOCATION;

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
    if (c == '\n') {
        next_line();
        return;
    }
    
    uint8_t* videoptr = NULL;
    videoptr = videoram + (video_row * ROW_WIDTH * 2) + video_col * 2;
    videoptr[0] = c;
    videoptr[1] = 0x07;
    
    next_char();
}

void clear()
{
    int i = 0;
    for (i = 0; i < ROW_WIDTH * 50; i++)
        put_char(' ');
}

void reset()
{
    clear();
    video_row = 0;
    video_col = 0;
}

void graphics_init()
{
    /*
     * Disable that crazy cursor! To do this, we tell
     * VGA to set the 6th bit (bit 5) in its index
     * register 0x0A. That is the "Cursor Disabled" bit.
     */
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, 0x20);
    
    reset();
}
