#include <EEPROM.h>

uint16_t addr;

bool eeprom_init1()
{
  addr = 2;
  eeprom_set();
  return 0;
}

bool eeprom_init()
{
  addr = (EEPROM.read(0) << 8) + EEPROM.read(1);
  Serial.print("addr: ");
  Serial.println(addr, HEX);
  while (EEPROM.read(addr & 0x0FFF) % 4 != 3)
  {
    addr = (addr & 0xF000) + (((addr & 0x0FFF) + 4092) % 4094) + 3;
  }
  Serial.print("addr: ");
  Serial.println(addr, HEX);
  match_point[0] = (addr >> 12) / 3 + 1;
  switch ((addr >> 12) % 3)
  {
    case 0:
      game_point[0] = 5;
      break;
    case 1:
      game_point[0] = 7;
      break;
    default:
      game_point[0] = 10;
      break;
  }
  
  int col = EEPROM.read((((addr & 0x0FFF) + 4092) % 4094)) >> 2;
  team_color[1] = col & 0x07;
  team_color[2] = col >> 3;
  Serial.print("byte0: ");
  Serial.println(EEPROM.read((((addr & 0x0FFF) + 4092) % 4094)), HEX);

  int stat = (EEPROM.read((((addr & 0x0FFF) + 4093) % 4094)) >> 2 << 8) + EEPROM.read(addr & 0x0FFF) >> 2;
  if (stat == 3000)
    return 0;
  game_point[1] = stat % 10;
  game_point[2] = stat / 10 % 10;
  match_point[1] = stat / 100 % 5;
  match_point[2] = stat / 500;
  Serial.print("byte1: ");
  Serial.println(EEPROM.read((((addr & 0x0FFF) + 4093) % 4094)), HEX);
  Serial.print("byte2: ");
  Serial.println(EEPROM.read(addr & 0x0FFF), HEX);

  Serial.print("stat: ");
  Serial.println(stat, HEX);

  Serial.print("match_point[0]: ");
  Serial.println(match_point[0], HEX);
  Serial.print("match_point[1]: ");
  Serial.println(match_point[1], HEX);
  Serial.print("match_point[2]: ");
  Serial.println(match_point[2], HEX);
  Serial.print("game_point[0]: ");
  Serial.println(game_point[0], HEX);
  Serial.print("game_point[1]: ");
  Serial.println(game_point[1], HEX);
  Serial.print("game_point[2]: ");
  Serial.println(game_point[2], HEX);
  Serial.print("team_color[1]: ");
  Serial.println(team_color[1], HEX);
  Serial.print("team_color[2]: ");
  Serial.println(team_color[2], HEX);
  return 1;
}

void eeprom_write()
{
  int col = (team_color[2] << 3) + team_color[1];
  int stat = match_point[2] * 500 + match_point[1] * 100 + game_point[2] * 10 + game_point[1];
  EEPROM.write(addr, col << 2);
  addr = (addr & 0xF000) + (((addr & 0x0FFF) + 4092) % 4094) + 3;
  EEPROM.write(addr, stat >> 6 << 2);
  addr = (addr & 0xF000) + (((addr & 0x0FFF) + 4092) % 4094) + 3;
  EEPROM.write(addr, ((stat & 0x3F) << 2) + 3);

  /*
  Serial.print("byte0: ");
  Serial.println(col << 2, HEX);
  Serial.print("byte1: ");
  Serial.println(stat >> 8 << 2, HEX);
  Serial.print("byte2: ");
  Serial.println(((stat & 0x3F) << 2) + 3, HEX);
  Serial.print("addr: ");
  Serial.println(addr, HEX);
  Serial.print("col: ");
  Serial.println(col, HEX);
  Serial.print("stat: ");
  Serial.println(stat, HEX);
  Serial.print("csm1: ");
  Serial.println(((stat << 2) + 3) % 4, HEX);
  */
}

void eeprom_set()
{
  int col = (team_color[2] << 3) + team_color[1];
  int stat = 3000;
  EEPROM.write(addr, col << 2);
  addr = (addr & 0xF000) + (((addr & 0x0FFF) + 4092) % 4094) + 3;
  EEPROM.write(addr, stat >> 6 << 2);
  addr = (addr & 0xF000) + (((addr & 0x0FFF) + 4092) % 4094) + 3;
  EEPROM.write(addr, ((stat & 0x3F) << 2) + 3);

  switch (game_point[0])
  {
    case 5:
      addr = (((match_point[0] - 1) * 3 + 0) << 12) + (addr & 0x0FFF);
      break;
    case 7:
      addr = (((match_point[0] - 1) * 3 + 1) << 12) + (addr & 0x0FFF);
      break;
    default:
      addr = (((match_point[0] - 1) * 3 + 2) << 12) + (addr & 0x0FFF);
      break;
  }
  
  
}



