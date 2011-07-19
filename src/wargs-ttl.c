/* vim: set sw=2 ts=2 expandtab: */

#include "game.h"

int
main ()
{
  game_init ();

  int ret = 0;
  asm volatile ("call ttl" : "=a" (ret));

  game_quit ();

  return ret;
}
