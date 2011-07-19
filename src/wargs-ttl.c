/* vim: set sw=2 ts=2 expandtab: */

#include "conf.h"
#include "filepath.h"
#include "video.h"

int
main ()
{
  conf_init ();
  filepath_init (conf_get_root ());
  video_init (conf_get_ratio ());

  int ret = 0;
  asm volatile ("call ttl" : "=a" (ret));

  video_quit ();
  filepath_quit ();
  conf_quit ();

  return ret;
}
