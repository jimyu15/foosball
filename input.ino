#define SHORT_PRESS 15
#define LONG_PRESS 300

const int input_pin[6] = {2, 3, 4, 5, 6, 7};
const bool input_logic[6] = {1, 1, 1, 1, 0, 0};
volatile int input[7] = {0};

void input_init()
{

  for (int i = 0; i < 6; i++)
    pinMode(input_pin[i], INPUT);

}

void input_scan()
{
  for (int i = 0; i < 6; i++)
  {
    if (digitalRead(input_pin[i]) == input_logic[i] && input[i] <= 0)
    {
      if (input[i] < 10000)
        input[i]--;
    }
    else if (input[i] < 0)
    {
      int timeout = 0;
      if (i == 4 || i == 5)
      {
        while (digitalRead(input_pin[i]) != input_logic[i])
        {
          timeout++;
          delay(1);
        }
      }
      else
        timeout = 5;
      if (timeout > 4)
      {
        input[i] *= -1;
        Serial.print("Input ");
        Serial.print(i);
        Serial.print(" triggered: ");
        Serial.println(input[i]);
      }
    }
  }
  if ((input[0] < -1 * LONG_PRESS && input[3] < -1 * LONG_PRESS) || (input[1] < -1 * LONG_PRESS && input[2] < -1 * LONG_PRESS))
    input[6] = LONG_PRESS + 1;
  /*
  if(Serial.available())
  {
    int in = Serial.parseInt();
    if (in < 7 && in >= 0)
      input[in] = LONG_PRESS + 1;
    Serial.read();
  }
  */
}

int input_read(const int pin)
{
  int state;
  if (input[pin] > LONG_PRESS)
    state = 2;
  else if (input[pin] > SHORT_PRESS)
    state = 1;
  else if (input[pin] < 0)
    state = -1;
  else
    state = 0;
  if (input[pin] > 0)
    input[pin] = 0;
  return state;
}

