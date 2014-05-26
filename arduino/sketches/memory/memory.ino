#include "andsoft.h"
#include <rtttl.h>

AndSoft::Button buttonBlue(12);
AndSoft::Button buttonGreen(11);
AndSoft::Button buttonRed(10);
AndSoft::Button buttonOrange(9);

AndSoft::LED ledBlue(A5);
AndSoft::LED ledGreen(A4);
AndSoft::LED ledRed(A3);
AndSoft::LED ledOrange(13);

AndSoft::Timer timer;

ProgmemPlayer tonePlayer(A2);
bool sound = false;

const uint8_t numberOfButtons = 4;
AndSoft::Button * buttons[] = {&buttonBlue, &buttonGreen, &buttonRed, &buttonOrange};
AndSoft::LED * leds[] = {&ledBlue, &ledGreen, &ledRed, &ledOrange};

const char greetSong[] PROGMEM = "PacMan:b=160:32b,32p,32b6,32p,32f#6,32p,32d#6,32p,32b6,32f#6,16p,16d#6,16p,32c6,32p,32c7,32p,32g6,32p,32e6,32p,32c7,32g6,16p,16e6,16p,32b,32p,32b6,32p,32f#6,32p,32d#6,32p,32b6,32f#6,16p,16d#6,16p,32d#6,32e6,32f6,32p,32f6,32f#6,32g6,32p,32g6,32g#6,32a6,32p,32b.6";
const char levelSong[] PROGMEM = "DonkeyKong:d=4,o=5,b=300:c6,32p,8d6,8p,f6,8p,8d6,16p,8c6,16p,8d6,16p,a#";
const char failSong[]  PROGMEM = "Fail:b=80:16d,8p,16c#,8p,16c,8p,4b4";
const char winSong[]   PROGMEM = "Hymne:d=4,o=4,b=84:8c.,16d,e.,8f,8g.,16a,2g,8a.,16b,c.5,8b,8a.,16g,2a,8g.,16f,e.,8d,8e.,16g,2d";

#define MAXSEQUENCE 10
uint8_t sequence[MAXSEQUENCE];
uint8_t length;
uint8_t correct;
bool easy = false;

enum State
{
  STATE_IDLE,
  STATE_OUTPUT,
  STATE_INPUT,
  STATE_NEXT,
  STATE_ERROR,
  STATE_WIN
};

State state = STATE_IDLE;

void randomize()
{
  const unsigned long m = millis();
  randomSeed(m);
}

bool isPressed()
{
  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    if (buttons[i]->pressed())
    {
      return true;
    }
  }
  return false;
}

void playSong(const char * song)
{
  tonePlayer.setSong(song);
  sound = true;
}

void stopSong()
{
  tonePlayer.silence();
  sound = false;
}

void enter_idle();
void state_idle();
void enter_output();
void state_output();
void enter_input();
void state_input();
void enter_next();
void state_next();
void enter_error();
void state_error();
void enter_win();
void state_win();

const unsigned long intervals[4] = {400, 400};

void enter_idle()
{
  //  Serial.println("idle");

  playSong(greetSong);

  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    leds[i]->flash(intervals, 2, HIGH);
  }

  state = STATE_IDLE;
}

void state_idle()
{
  // as soon as a buton is pressed, start
  bool start = false;
  if (buttonBlue.pressed())
  {
    easy = true;
    start = true;
  }
  else
  {
    easy = false;
    start = isPressed();
  }

  if (start)
  {
    randomize();
    length = 1;
    return enter_output();
  }
}

void enter_output()
{
  //  Serial.println("output");

  stopSong();

  // all LEDs off
  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    leds[i]->off();
  }

  // wait a sec
  delay(1000);

  // generate random path
  int last = -1;
  for (uint8_t i = 0; i < length;)
  {
    int next = random(0, 4);
    if (next != last)
    {
      // avoid same button twice
      sequence[i] = next;
      //      Serial.println(next);
      last = next;
      ++i;
    }
  }
  state = STATE_OUTPUT;
}

void state_output()
{
  // display once the correct sequence
  for (uint8_t i = 0; i < length; ++i)
  {
    leds[sequence[i]]->on();
    delay(500);
    leds[sequence[i]]->off();
    delay(500);
  }
  return enter_input();
}

void enter_input()
{
  //  Serial.println("input");

  stopSong();

  // reset player correct results
  correct = 0;

  // reset buttons
  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    buttons[i]->pressed();
    leds[i]->on();
  }

  delay(200);

  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    leds[i]->off();
  }

  delay(200);

  if (easy)
  {
    leds[sequence[correct]]->on();
  }

  state = STATE_INPUT;
}

void state_input()
{
  bool ok = false;
  // we always check all buttons to enforce symmetry
  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    if (buttons[i]->pressed())
    {
      if (i != sequence[correct])
      {
        // wrong button. gone
        return enter_error();
      }
      else
      {
        ok = true;
      }
    }
  }

  if (ok)
  {
    // ok, good one
    if (easy)
    {
      leds[sequence[correct]]->off();
    }
    ++correct;
    if (easy)
    {
      leds[sequence[correct]]->on();
    }
    if (correct == length)
    {
      if (length == MAXSEQUENCE)
      {
        // won the game
        return enter_win();
      }
      else
      {
        // won this leg
        return enter_next();
      }
    }
  }
}

void enter_error()
{
  //  Serial.println("error");

  playSong(failSong);
  timer.start(2000);

  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    leds[i]->flash(intervals, 2, i < 2 ? HIGH : LOW);
  }
  state = STATE_ERROR;
}

void state_error()
{
  if (timer.done())
  {
    length = 1;
    return enter_idle();
  }
}

void enter_next()
{
  //  Serial.println("next");

  playSong(levelSong);
  timer.start(2000);

  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    leds[i]->flash(intervals, 2, i & 1 ? HIGH : LOW);
  }
  state = STATE_NEXT;
}

void state_next()
{
  if (timer.done())
  {
    ++length;
    return enter_output();
  }
}

void enter_win()
{
  //  Serial.println("win");

  playSong(winSong);

  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    leds[i]->flash(intervals, 2, (i == 0 || i == 3) ? HIGH : LOW);
  }
  state = STATE_WIN;
}

void state_win()
{
  // as soon as a buton is pressed, restart
  if (isPressed())
  {
    return enter_idle();
  }
}

void setup()
{
  //  Serial.begin(9600);

  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    leds[i]->setup();
    buttons[i]->setup();
  }
  timer.setup();

  enter_idle();
}

void loop()
{

  // this is cooperative multitasking
  for (uint8_t i = 0; i < numberOfButtons; ++i)
  {
    buttons[i]->loop();
    leds[i]->loop();
  }
  timer.loop();
  if (sound)
    tonePlayer.pollSong();

  // action
  switch (state)
  {
    case STATE_IDLE:
      state_idle();
      break;
    case STATE_OUTPUT:
      state_output();
      break;
    case STATE_INPUT:
      state_input();
      break;
    case STATE_NEXT:
      state_next();
      break;
    case STATE_ERROR:
      state_error();
      break;
    case STATE_WIN:
      state_win();
      break;
  }

}

// Local Variables:
// mode: c++
// End:
