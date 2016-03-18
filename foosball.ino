#include <WS2812.h>
int game_point[3] = {5, 0, 0};
int match_point[3] = {3, 0, 0};
cRGB color[20];


void setup() 
{
  Serial.begin(115200);
  led_init();
  input_init();

}

void loop() 
{
  
  set_game();
  set_match();
  set_color();
  match_point[1] = match_point[2] = 0;
  while (game() == 0)
    exchange();
  
}

