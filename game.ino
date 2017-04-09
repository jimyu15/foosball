#define COLOR_NUM 8
uint32_t color_bar[COLOR_NUM] = {0xFF0000, 0xFF9900, 0xFFD700, 0x66FF00, 0xFFFF, 0x66FF, 0x9900CC, 0xFF00FF};

void set_game()
{
  for (int i = 0; i < 7; i++)
  {
    input_scan();
    input_read(i);
  }
  do
  {
    for (int i = 0; i < 20; i++)
    {
      if (i < game_point[0])
        color[i].g = 255;
      else
        color[i].g = 0;
      color[i].r = 0;
      color[i].b = 0;
    }
    led_refresh(1);
    input_scan();
    if (input_read(0) > 0)
    {
      switch (game_point[0])
      {
        case 5:
          game_point[0] = 7;
          break;
        case 7:
          game_point[0] = 10;
          break;
        default:
          game_point[0] = 5;
          break;
      }
      Serial.print(input_read(0));
      Serial.print("Game point selected: ");
      Serial.println(game_point[0]);
    }
  } while (input_read(1) < 1);
  Serial.print("Game point confirmed: ");
  Serial.println(game_point[0]);

}

void set_match()
{
  for (int i = 0; i < 7; i++)
  {
    input_scan();
    input_read(i);
  }
  do
  {
    for (int i = 0; i < 20; i++)
    {
      if (9 - i < match_point[0] && i < 10)
      {
        color[i].r = 255;
        color[i].g = 255;
        color[i].b = 255;
      }
      else
      {
        color[i].r = 0;
        color[i].g = 0;
        color[i].b = 0;
      }
    }
    led_refresh(1);
    input_scan();
    if (input_read(0) > 0)
    {
      match_point[0]--;
      if (match_point[0] == 0)
        match_point[0] = 5;
      Serial.print("Match point selected: ");
      Serial.println(match_point[0]);
    }
  } while (input_read(1) < 1);
  Serial.print("Match point confirmed: ");
  Serial.println(match_point[0]);

}

void set_color()
{
  for (int i = 0; i < 7; i++)
  {
    input_scan();
    input_read(i);
  }
  uint8_t flag = 3;
  do
  {
    for (int i = 0; i < 20; i++)
    {
      coloring(i, i / 10 + 1, 2);
    }
    led_refresh(flag);
    input_scan();

    if (input_read(0) > 0 && (flag & 1))
    {
      team_color[1] = (team_color[1] + 1) % COLOR_NUM;
      Serial.print("Team 1 color selected: ");
      Serial.println(team_color[1]);
    }

    if (input_read(2) > 0 && (flag & 2))
    {
      team_color[2] = (team_color[2] + 1) % COLOR_NUM;
      Serial.print("Team 2 color selected: ");
      Serial.println(team_color[2]);
    }


    if (input_read(1) > 0)
    {
      if (flag == 1)
      {
        if (team_color[1] != team_color[2])
          flag = 0;
      }
      else
        flag = 2;
    }

    if (input_read(3) > 0)
    {
      if (flag == 2)
      {
        if (team_color[1] != team_color[2])
          flag = 0;
      }
      else
        flag = 1;
    }
  } while (flag);
  Serial.println("Team colors confirmed.");


}

bool game()
{
  Serial.print("Now matching ");
  Serial.print(match_point[1]);
  Serial.print(" : ");
  Serial.println(match_point[2]);

  for (int i = 0; i < 10; i++)
  {
    int dim;
    if (i % 10 < match_point[1])
      dim = 2;
    else if (i % 10 < match_point[0])
      dim = 1;
    else dim = 0;
    coloring(9 - i, 0, dim);
  }
  for (int i = 0; i < 10; i++)
  {
    int dim;
    if (i % 10 < match_point[2])
      dim = 2;
    else if (i % 10 < match_point[0])
      dim = 1;
    else dim = 0;
    coloring(19 - i, 0, dim);
  }
  for (int i = 0; i < 200; i++)
  {
    led_refresh(0);
    delay(10);
  }

  for (int i = 0; i < 7; i++)
    input_read(i);
  for (int i = 0; i < 10; i++)
  {
    int dim;
    if (i % 10 < game_point[1])
      dim = 2;
    else if (i % 10 < game_point[0])
      dim = 1;
    else dim = 0;
    coloring(i, 1, dim);
  }
  for (int i = 10; i < 20; i++)
  {
    int dim;
    if (i % 10 < game_point[2])
      dim = 2;
    else if (i % 10 < game_point[0])
      dim = 1;
    else dim = 0;
    coloring(i, 2, dim);
  }

  for (int i = 0; i < match_point[1]; i++)
  {
    coloring(9 - i, 0, 2);
  }
  for (int i = 0; i < match_point[2]; i++)
  {
    coloring(19 - i, 0, 2);
  }
  led_refresh(0);

  int blink_num = 0, blink_count = 0, flag = 0;
  do
  {
    input_scan();
    if (input_read(6) == 2)
    {
      Serial.println("Match aborted");
      for (int i = 10; i < 20; i++)
      {
        coloring(i, 2, 0);
      }
      led_refresh(0);
      delay(500);
      return 2;
    }
    if (input_read(1) == 2 || input_read(5) == 2)
    {
      game_point[1]++;
      if (blink_count > 0)
      {
        coloring(blink_num, blink_num / 10 + 1, 2);
      }
      blink_num = game_point[1] - 1;
      blink_count = 1000;
      eeprom_write();
      Serial.print("GOAL! ");
      Serial.print(game_point[1]);
      Serial.print(" : ");
      Serial.println(game_point[2]);


    }
    if (input_read(3) == 2 || input_read(4) == 2)
    {
      game_point[2]++;
      if (blink_count > 0)
      {
        coloring(blink_num, blink_num / 10 + 1, 2);
      }
      blink_num = game_point[2] - 1 + 10;
      blink_count = 1000;
      eeprom_write();
      Serial.print("GOAL! ");
      Serial.print(game_point[1]);
      Serial.print(" : ");
      Serial.println(game_point[2]);
    }
    if (input_read(0) == 2 && game_point[1] > 0)
    {
      game_point[1]--;
      coloring(game_point[1], 1, 1);
      eeprom_write();
      Serial.print("GOAL! ");
      Serial.print(game_point[1]);
      Serial.print(" : ");
      Serial.println(game_point[2]);
    }
    if (input_read(2) == 2 && game_point[2] > 0)
    {
      game_point[2]--;
      coloring(game_point[2] + 10, 2, 1);
      eeprom_write();
      Serial.print("GOAL! ");
      Serial.print(game_point[1]);
      Serial.print(" : ");
      Serial.println(game_point[2]);
    }

    if (game_point[1] >= game_point[0])
      flag = 1;
    if (game_point[2] >= game_point[0])
      flag = 2;
    if (blink_count > 0)
    {

      blink_count--;
      if (blink_count / 100 % 2)
      {
        coloring(blink_num, blink_num / 10 + 1, 1);
      }
      else
      {
        coloring(blink_num, blink_num / 10 + 1, 2);
      }
    }
    else if (flag)
    {
      if (abs(int(game_point[1]) - game_point[2]) <= 1 && match_point[0] - 1 == match_point[1] && match_point[1] == match_point[2])
      {
        Serial.println("Duece");
      }
      else
      {
        Serial.print("Team ");
        Serial.print(flag);
        Serial.println(" takes this game!");
        match_point[flag]++;
        if (match_point[flag] >= match_point[0])
        {
          Serial.print("Team ");
          Serial.print(flag);
          Serial.println(" wins!");
          for (int i = flag * 10 - 10; i < flag * 10; i++)
          {
            coloring(i, flag, 2);
          }
          for (int i = 0; i < 300; i++)
          {
            led_refresh(flag);
            delay(10);
          }
          return flag;
        }
        else
          return 0;
      }
    }


    led_refresh(0);
  } while (1);
}


void coloring(int num, int team, int dim)
{
  if (team == 0)
  {
    color[num].r = 255 / (dim == 1 ? 3 : 1) * (dim == 0 ? 0 : 1);
    color[num].g = 255 / (dim == 1 ? 3 : 1) * (dim == 0 ? 0 : 1);
    color[num].b = 255 / (dim == 1 ? 3 : 1) * (dim == 0 ? 0 : 1);
  }
  else
  {
    color[num].r = (color_bar[team_color[team]] >> 16) / (dim == 1 ? 3 : 1) * (dim == 0 ? 0 : 1);
    color[num].g = ((color_bar[team_color[team]] >> 8) % 256) / (dim == 1 ? 3 : 1) * (dim == 0 ? 0 : 1);
    color[num].b = (color_bar[team_color[team]] % 256) / (dim == 1 ? 3 : 1) * (dim == 0 ? 0 : 1);
  }
  /*
  Serial.print(color[num].r, HEX);
  Serial.print("\t");
  Serial.print(color[num].g, HEX);
  Serial.print("\t");
  Serial.print(color[num].b, HEX);
  Serial.print("\t");
  Serial.print((color_bar[team_color[team]] >> 8) % 256, HEX);
  */
}

