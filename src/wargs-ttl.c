/* vim: set sw=2 ts=2 expandtab: */

#include "game.h"
#include "conf.h"

int
main ()
{
  game_init ();

  if (conf_get_fast ())
    {
      game_set_fps_high ();
    }
  else
    {
      game_set_fps_low ();
    }

  int ret = 0;
  asm volatile ("call ttl" : "=a" (ret));

  game_quit ();

  return ret;
}
