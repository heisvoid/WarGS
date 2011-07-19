/* vim: set sw=2 ts=2 expandtab: */

#include <SDL.h>

#include "conf.h"
#include "assert.h"
#include "video.h"
#include "mouse.h"

enum
{
  DEFAULT_FPS = 30
};

static uint32_t fps = DEFAULT_FPS;

void
game_cfg_setup ()
{
  extern int8_t game_cfg;

  (&game_cfg)[0] = (true == conf_get_fast () ? 1 : 0);
  (&game_cfg)[1] = 1; /* force music on */
  (&game_cfg)[2] = 1; /* force sound on */
  (&game_cfg)[3] = 5; /* unknown */
  ((int16_t *) (&game_cfg))[2] = 220; /* force sound-blaster ADR 220 */
  ((int16_t *) (&game_cfg))[3] = 7;   /* force sound-blaster IRQ 7 */
  ((int16_t *) (&game_cfg))[4] = 1;   /* force sound-blaster DMA 1 */
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
