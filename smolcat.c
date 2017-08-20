#!/bin/tcc -run

/**
 * smol cat: unbuffered echo for terminal testing
 * Ondřej Hruška, Aug 2017
 * MIT license
 */

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <signal.h>

void signal_handler(int sig);

struct termios old;

int main(void)
{
  struct termios new;
  char c;
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  tcgetattr(TCSANOW, &old);
  new = old;
  new.c_lflag &= (~ECHO & ~ICANON);
  tcsetattr(0, TCSANOW, &new);
  tcsetattr(0, TCSANOW, &new);

  // buffer swap with fallback reset after
  fputs("\e[?1049h\ec\e[r", stdout);

  while(1) putchar(getchar());
}

void signal_handler(int sig)
{
  // buffer unswap with fallback reset before
  fputs("\ec\e[r\e[?1049l", stdout);
  tcsetattr(0, TCSANOW, &old);
  exit(0);
}
