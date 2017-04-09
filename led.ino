#include <WS2812.h>

WS2812 LED(20);

#define brightness 200
const char gammatable[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

#define DATA_PIN 10

void led_refresh(uint8_t dimming)
{
  long dim[2];
  dim[0] = brightness;
  dim[1] = brightness;
  if (dimming & 1)
    dim[0] = dim[0] * abs((long)millis() % 1000 - 500) / 500;
  if (dimming & 2)
    dim[1] = dim[1] * abs((long)millis() % 1000 - 500) / 500;
  for (int i = 0; i < 10; i++)
  {
    cRGB temp;
    temp.r = gammatable[(long)color[9 - i].r * dim[i / 10] / 255];
    temp.g = gammatable[(long)color[9 - i].g * dim[i / 10] / 255];
    temp.b = gammatable[(long)color[9 - i].b * dim[i / 10] / 255];
    LED.set_crgb_at(i, temp);
  }
  for (int i = 10; i < 20; i++)
  {
    cRGB temp;
    temp.r = gammatable[(long)color[i].r * dim[i / 10] / 255];
    temp.g = gammatable[(long)color[i].g * dim[i / 10] / 255];
    temp.b = gammatable[(long)color[i].b * dim[i / 10] / 255];
    LED.set_crgb_at(i, temp);
  }
  for (int i = 0; i < 4; i++)
  {
    cRGB w;
    w.r = w.g = w.b = 255;
    if (input[i] < -3)
    {
      switch(i)
      {
        case 0:
        LED.set_crgb_at(0, w);
        break;
        case 1:
        LED.set_crgb_at(9, w);
        break;
        case 3:
        LED.set_crgb_at(10, w);
        break;
        case 2:
        LED.set_crgb_at(19, w);
        break;
      }
    }
    if (input[i] < -1 * LONG_PRESS)
    {
      switch(i)
      {
        case 0:
        LED.set_crgb_at(1, w);
        break;
        case 1:
        LED.set_crgb_at(8, w);
        break;
        case 3:
        LED.set_crgb_at(11, w);
        break;
        case 2:
        LED.set_crgb_at(18, w);
        break;
      }
    }
  }
  LED.sync();
}

void led_init()
{
  for (int i = 0; i < 20; i++)
  {
    color[i].r = 0;
    color[i].g = 0;
    color[i].b = 0;
  }
  LED.setOutput(DATA_PIN);
  led_refresh(0);
}


void exchange()
{
  for (int i = 0; i < 10; i++)
  {
    cRGB temp = color[0];
    for (int j = 0; j < 20; j++)
      color[j] = color[j + 1];
    color[19] = temp;
    led_refresh(0);
    delay(200);
    
  }
  int temp;
  temp = team_color[1];
  team_color[1] = team_color[2];
  team_color[2] = temp;

  temp = game_point[1];
  game_point[1] = game_point[2];
  game_point[2] = temp;

  temp = match_point[1];
  match_point[1] = match_point[2];
  match_point[2] = temp;

  
  delay(500);
}

