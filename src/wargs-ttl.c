/* vim: set sw=2 ts=2 expandtab: */

#include "conf.h"
#include "filepath.h"

int
main ()
{
  conf_init ();
  filepath_init (conf_get_root ());

  int ret = 0;
  asm volatile ("call ttl" : "=a" (ret));

  filepath_quit ();
  conf_quit ();

  return ret;
}
