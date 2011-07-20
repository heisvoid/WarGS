/* vim: set sw=2 ts=2 expandtab: */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "game.h"

#include <SDL.h>

#include "conf.h"
#include "assert.h"
#include "video.h"
#include "mouse.h"
#include "filepath.h"
#include "keyboard.h"
#include "util.h"
#include "audio.h"

enum
{
  DEFAULT_FPS = 30
};

static bool initialized = false;
static uint32_t fps = DEFAULT_FPS;

void
game_cfg_setup ()
{
  extern int8_t game_cfg_speed;

  game_cfg_speed = (true == conf_get_fast () ? 1 : 0);
}

void
game_set_fps (uint32_t n)
{
  ASSERT (0 < n);

  fps = n;
}

void
game_set_fps_default ()
{
  game_set_fps (DEFAULT_FPS);
}

static void
handle_events ()
{
  SDL_Event event;

  while (1 == SDL_PollEvent (&event))
    {
      switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION:
          mouse_handle_event (&event);
          break;
        default:
          break;
        }
    }
}

void
game_update ()
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  static uint32_t last_frame_ticks = 0;

  handle_events ();

  if (0 < last_frame_ticks)
    {
      const int32_t delay = 1000 / fps - (SDL_GetTicks () - last_frame_ticks);
      if (0 < delay)
        {
          SDL_Delay (delay);
        }
    }

  video_update ();

  last_frame_ticks = SDL_GetTicks ();
}

void
game_init ()
{
  if (true == initialized)
    {
      LOG_FATAL ("already initialized");
    }

  conf_init ();
  filepath_init (conf_get_root ());
  video_init (conf_get_ratio ());
  keyboard_init ();

  char * const music_root = xmalloc (PATH_MAX);
  xsnprintf (music_root, PATH_MAX, "%s%cmusic", conf_get_root (),
             FILEPATH_SEPARATOR);
  audio_init (music_root);
  free (music_root);

  if (false == conf_get_music ())
    {
      audio_music_mute ();
    }
  if (false == conf_get_sound ())
    {
      audio_sound_mute ();
    }

  mouse_init ();

  initialized = true;
}

void
game_quit ()
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  mouse_quit ();
  audio_quit ();
  keyboard_quit ();
  video_quit ();
  filepath_quit ();
  conf_quit ();

  initialized = false;
}
