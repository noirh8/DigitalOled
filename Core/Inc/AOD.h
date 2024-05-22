#ifndef AOD_H
#define AOD_H

#include "main.h"
#include "ssd1306.h"
#include <stdint.h>

#define FRAME_DELAY 42
#define FRAME_WIDTH 32
#define FRAME_HEIGHT 32
#define FRAME_COUNT (sizeof(frames) / sizeof(frames[0]))

uint8_t frame = 0;
const uint8_t frames[][128] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,192,0,31,240,64,0,48,0,192,96,31,255,128,248,7,255,0,204,2,1,131,68,2,1,243,67,255,6,31,99,129,252,7,32,0,0,3,32,0,0,3,48,0,0,3,14,0,0,15,3,224,0,59,0,63,255,227,0,0,252,0,0,0,7,0,0,0,7,128,0,0,6,192,0,0,7,128,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,192,0,31,252,64,0,48,0,192,0,30,3,128,248,7,255,0,204,2,1,0,68,2,1,243,66,127,7,63,99,255,252,7,96,0,96,3,32,0,0,3,48,0,0,3,30,0,0,15,7,224,0,59,0,127,255,224,0,7,255,0,0,0,6,0,0,0,7,128,0,0,6,192,0,0,6,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,192,0,15,252,64,0,48,0,64,0,28,3,128,248,15,255,0,204,2,1,0,68,2,1,224,70,31,3,59,99,255,252,7,96,0,240,3,32,0,0,3,48,0,0,3,28,0,0,7,7,128,0,27,0,255,11,224,0,7,255,0,0,0,6,0,0,0,7,128,0,0,6,192,0,0,6,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,15,255,192,0,62,0,64,0,56,1,128,248,15,255,0,204,2,1,0,68,2,1,227,70,15,3,251,67,255,252,15,96,1,240,3,32,0,0,3,48,0,0,3,24,0,0,7,7,0,0,27,0,248,3,226,0,7,255,0,0,0,4,0,0,0,7,128,0,0,6,192,0,0,6,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,15,255,192,0,62,0,64,0,56,1,128,240,15,255,0,248,3,65,2,68,2,1,227,70,6,3,251,67,255,28,14,96,1,240,3,32,0,0,3,48,0,0,3,24,0,0,7,15,0,0,27,1,248,1,243,0,31,255,128,0,0,4,0,0,0,7,128,0,0,6,192,0,0,6,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,3,255,192,0,31,0,64,0,48,1,128,112,31,255,0,248,3,193,0,196,2,1,227,70,2,3,251,67,255,14,15,96,1,248,3,32,0,0,3,48,0,0,3,24,0,0,7,14,0,0,27,1,224,1,243,0,31,255,128,0,0,12,0,0,0,7,128,0,0,6,128,0,0,6,192,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,252,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,3,255,192,0,31,0,64,0,48,1,128,96,31,255,0,248,3,193,0,204,2,1,192,70,2,3,251,67,255,14,15,97,129,248,3,32,0,0,3,48,0,0,3,24,0,0,7,14,0,0,27,1,224,0,240,0,31,255,128,0,0,12,0,0,0,7,128,0,0,6,128,0,0,6,192,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,3,252,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,3,255,192,0,31,0,64,0,48,1,128,96,31,255,0,248,3,193,0,204,2,1,195,70,2,3,251,67,255,14,15,97,129,248,3,32,0,0,3,48,0,0,3,24,0,0,7,14,0,0,27,1,224,0,243,0,31,255,128,0,0,12,0,0,0,7,128,0,0,6,128,0,0,6,192,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,7,252,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,3,255,192,0,31,0,64,0,48,1,128,112,31,255,0,248,3,193,2,196,2,1,195,70,2,3,251,67,255,14,14,96,1,248,3,32,0,0,3,48,0,0,3,24,0,0,7,14,0,0,27,1,224,1,243,0,31,255,128,0,0,12,0,0,0,7,128,0,0,134,128,0,0,134,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,15,252,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,3,255,192,0,31,0,64,0,48,1,128,240,31,255,0,248,3,65,0,196,2,1,227,70,2,3,251,67,255,14,15,96,1,248,3,32,0,0,3,48,0,0,3,24,0,0,7,15,0,0,27,1,248,1,243,0,31,255,128,0,0,12,0,0,0,7,128,0,3,134,128,0,3,134,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,15,248,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,15,255,192,0,62,0,64,0,56,1,128,240,15,255,0,248,3,65,0,68,2,1,224,70,6,3,251,67,255,28,15,96,1,240,3,32,0,0,3,48,0,0,3,28,0,0,7,15,0,0,59,1,248,1,240,0,31,255,128,0,0,12,0,0,0,7,128,0,7,134,128,0,7,134,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,15,240,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,15,255,192,0,62,0,64,0,56,1,128,240,15,255,0,252,2,1,0,68,2,1,227,70,6,3,251,67,255,28,15,32,1,240,3,32,0,0,3,48,0,0,3,28,0,0,7,7,0,0,27,1,248,3,226,0,31,255,0,0,0,4,0,0,0,7,128,0,31,134,192,0,31,134,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,15,192,0,0,0,0,0,0,0,0,0,0,0,0,3,128,0,15,255,192,0,62,0,192,0,56,1,128,248,15,255,0,204,2,1,3,68,2,1,227,70,15,3,59,67,255,252,14,96,0,240,3,32,0,0,3,48,0,0,3,28,0,0,7,7,128,0,27,0,255,3,227,0,7,255,0,0,0,6,0,0,0,7,128,0,63,134,192,0,63,134,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,0,0,0,0,15,128,0,15,252,64,0,48,0,192,0,28,3,128,248,15,255,0,204,2,1,0,68,2,1,227,70,15,3,59,99,255,252,7,32,0,240,3,32,0,0,3,48,0,0,3,28,0,0,15,7,128,0,59,0,255,15,224,0,7,255,0,0,0,6,0,0,0,7,128,0,127,134,192,0,127,134,128,0,0,3,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,15,128,0,31,252,64,0,48,0,192,0,28,3,128,248,15,255,0,204,2,1,0,68,2,1,242,66,127,7,63,99,255,252,7,32,0,192,3,32,0,0,3,48,0,0,3,28,0,0,15,3,128,0,58,0,127,255,224,0,0,255,0,0,0,7,0,0,0,7,128,0,127,134,192,0,127,134,128,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,192,0,31,252,64,0,48,0,192,0,30,3,128,248,7,255,0,204,2,1,128,70,2,1,243,66,127,6,63,99,255,252,7,32,0,0,3,32,0,0,3,48,0,0,3,30,0,0,15,7,224,0,59,0,127,255,224,0,0,252,0,0,0,7,0,0,0,7,128,0,126,4,192,0,126,7,128,0,0,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,192,0,31,240,64,0,48,0,192,96,30,7,128,248,7,255,0,76,2,1,131,70,2,1,243,67,255,6,31,99,193,252,7,32,0,0,3,32,0,0,3,16,0,0,3,14,0,0,15,3,224,0,59,0,63,255,227,0,0,252,0,0,0,7,0,0,0,7,128,0,124,4,192,0,124,7,128,0,0,1,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,192,0,31,240,64,0,48,0,192,96,31,255,128,248,3,255,0,68,2,1,128,70,2,3,243,67,255,6,31,99,193,252,7,32,0,0,3,48,0,0,3,24,0,0,7,14,0,0,15,3,240,0,123,0,63,255,224,0,0,28,0,0,0,7,0,0,0,7,128,0,112,4,128,0,112,7,128,0,0,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,192,0,31,240,64,0,48,0,192,112,31,255,128,248,3,255,0,68,2,1,192,70,2,3,243,99,255,6,31,96,1,252,7,32,0,0,3,48,0,0,3,24,0,0,7,15,0,0,15,1,240,0,120,0,31,255,224,0,0,28,0,0,0,7,0,0,0,7,128,0,96,4,128,0,96,7,128,0,0,1,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,255,192,0,31,192,64,0,16,0,192,248,15,255,128,252,3,57,0,68,2,1,194,70,2,3,243,99,255,6,31,32,1,252,7,32,0,0,3,48,0,0,3,24,0,0,7,7,0,0,15,1,240,0,115,0,31,255,128,0,0,12,0,0,0,7,0,0,0,6,128,0,0,4,128,0,0,7,128,0,0,0,0,0,0,0,0,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,255,192,0,31,192,64,0,24,0,192,248,15,255,128,204,3,57,0,68,2,1,195,70,2,3,243,99,255,30,15,32,1,248,3,32,0,0,3,48,0,0,3,28,0,0,7,7,128,0,15,0,248,0,115,0,15,255,131,0,0,12,0,0,0,7,128,0,0,6,128,0,0,4,128,0,0,7,128,0,0,0,0,0,0,0,0,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,255,192,0,31,192,64,0,24,0,192,248,15,255,128,76,3,57,0,68,2,1,194,66,15,3,243,99,255,30,15,32,1,248,3,32,0,0,3,48,0,0,3,28,0,0,7,7,128,0,31,0,248,0,243,0,15,255,128,0,0,12,0,0,0,7,128,0,0,6,128,0,0,4,128,0,0,7,128,0,0,0,0,0,0,0,0,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,128,0,31,255,192,0,24,0,64,0,28,0,192,248,15,255,128,76,3,1,0,70,2,1,224,66,31,3,251,99,255,254,15,32,0,248,3,32,0,0,3,16,0,0,3,14,0,0,7,3,192,0,31,0,127,0,240,0,7,255,128,0,0,12,0,0,0,7,128,0,0,6,128,0,0,4,128,0,0,7,128,0,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,255,192,0,31,0,64,0,24,0,192,248,15,255,128,76,3,33,0,68,2,1,194,66,15,3,243,99,255,14,31,32,1,248,7,32,0,0,3,48,0,0,3,28,0,0,7,7,128,0,15,0,120,0,115,0,7,255,128,0,0,12,0,0,0,7,128,0,0,6,128,0,0,4,128,0,0,7,128,0,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,255,192,0,31,0,64,0,24,0,192,248,15,255,128,76,3,225,0,68,2,1,195,70,15,3,251,99,255,14,31,32,0,252,7,32,0,0,3,48,0,0,3,28,0,0,7,7,128,0,15,0,248,0,115,0,15,255,131,0,0,12,0,0,0,7,0,0,0,7,128,0,0,4,128,0,0,7,128,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,255,192,0,31,248,64,0,24,0,192,248,15,255,128,204,3,249,0,68,2,1,192,70,2,1,251,99,255,6,31,96,1,252,3,32,0,0,3,48,0,0,3,24,0,0,3,7,0,0,15,0,240,0,123,0,15,255,192,0,0,12,0,0,0,7,0,0,0,7,128,0,0,4,128,0,0,7,128,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,192,0,31,240,64,0,48,0,192,240,31,255,128,252,3,255,0,68,2,1,128,70,2,3,243,67,255,6,31,96,1,252,3,32,0,0,3,48,0,0,3,24,0,0,3,15,0,0,15,1,240,0,122,0,31,255,192,0,0,28,0,0,0,7,0,0,0,7,128,0,0,4,128,0,0,7,128,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,192,0,31,240,64,0,48,0,192,240,31,255,128,248,3,255,0,76,2,1,128,70,2,3,243,67,255,6,31,99,193,252,3,32,0,0,3,32,0,0,3,24,0,0,3,14,0,0,15,3,224,0,59,0,63,255,224,0,0,252,0,0,0,7,0,0,0,7,128,0,0,4,192,0,0,7,128,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};


void AOD(void)
{
    ssd1306_DrawRectangle(18, 1, 108, 11, White);
    ssd1306_SetCursor(25, 3);
    ssd1306_WriteString("DO AN MON HOC", Font_6x8, White);
    ssd1306_SetCursor(26, 17);
    ssd1306_WriteString("NHOM 16", Font_11x18, White);

    ssd1306_FillRectangle(1, 35, 86, 95, White);
    ssd1306_SetCursor(5, 39);
    ssd1306_WriteString("DIGITAL", Font_7x10, Black);
    ssd1306_SetCursor(47, 51);
    ssd1306_WriteString("CLOCK", Font_7x10, Black);

    ssd1306_DrawBitmap(90, 35, frames[frame], FRAME_WIDTH, FRAME_HEIGHT, White);
    frame = (frame + 1) % FRAME_COUNT;
    HAL_Delay(FRAME_DELAY);
    ssd1306_UpdateScreen();
}

#endif // AOD_H
