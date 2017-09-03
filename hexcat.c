#!/bin/tcc -run

/**
 * hex cat: dump escape sequences received as ASCII and numbers
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

  while(1) {
    char c = getchar();
    int n = c == 27 ? 33 : 36;
    if(c==27)printf("\n");
    printf("%2d \e[%dm%c\e[m ", c, n, c); // <- change d to X or x if you want hex
  }
}

void signal_handler(int sig)
{
  fputs("\n", stdout);
  tcsetattr(0, TCSANOW, &old);
  exit(0);
}
