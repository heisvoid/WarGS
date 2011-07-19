/* vim: set sw=2 ts=2 expandtab: */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <stdlib.h>

#include "conf.h"
#include "filepath.h"
#include "video.h"
#include "keyboard.h"
#include "audio.h"
#include "util.h"
#include "mouse.h"

int
main ()
{
  conf_init ();

  const char * const root = conf_get_root ();

  filepath_init (root);
  video_init (conf_get_ratio ());
  keyboard_init ();

  const size_t music_root_size = sizeof (char) * (strlen (root) + 1 + 5 + 1);
  char * const music_root = xmalloc (music_root_size);
  xsnprintf (music_root, music_root_size,
             "%s%cmusic", root, FILEPATH_SEPARATOR);
  audio_init (music_root);
  free (music_root);

  mouse_init ();

  int ret = 0;
  asm volatile ("call ttl" : "=a" (ret));

  mouse_quit ();
  audio_quit ();
  keyboard_quit ();
  video_quit ();
  filepath_quit ();
  conf_quit ();

  return ret;
}
