/* vim: set sw=2 ts=2 expandtab: */

#include "game.h"

int
main (int argc, char *argv[])
{
  game_init ();

  int ret = 0;
  asm volatile ("call chp" : "=a" (ret) : "a" (argc), "d" (argv));

  game_quit ();

  return ret;
}
