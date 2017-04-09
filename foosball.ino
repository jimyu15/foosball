#include <WS2812.h>
int game_point[3] = {5, 0, 0};
int match_point[3] = {2, 0, 0};
int team_color[3] = {0, 0, 2};
cRGB color[20];
bool starting = 1;

void setup() 
{
  Serial.begin(115200);
  led_init();
  input_init();

}

void loop() 
{
  if (starting)
  {
    set_game();
    set_match();
    set_color();
    match_point[1] = match_point[2] = 0;
    game_point[1] = game_point[2] = 0;
    eeprom_set();
  }
  else 
    starting = 1;
  
  while (game() == 0)
  {
    exchange();
    game_point[1] = game_point[2] = 0;
  }
  eeprom_set();
  
}

