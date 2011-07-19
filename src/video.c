/* vim: set sw=2 ts=2 expandtab: */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "video.h"

#include <stdbool.h>

#include <SDL.h>

#include "assert.h"
#include "util.h"

enum
{
  WIDTH = 320,
  HEIGHT = 200,

  PALETTE_LEN = 256
};

static bool initialized = false;
static uint8_t ratio = 1;
static SDL_Surface *surface = NULL;
static SDL_Color palette[PALETTE_LEN];

void *video_buffer = NULL;

void
video_init (uint8_t r)
{
  if (true == initialized)
    {
      LOG_ERROR ("already initialized");

      return;
    }

  ratio = (2 > r) ? 1 : r;

  int ret = SDL_Init (SDL_INIT_VIDEO);
  ASSERT (0 == ret);

  surface = SDL_SetVideoMode (WIDTH * ratio, HEIGHT * ratio, 8, SDL_SWSURFACE);
  ASSERT (NULL != surface);

  SDL_WM_SetCaption (PACKAGE_NAME, NULL);

  if (1 < ratio)
    {
      video_buffer = xmalloc (WIDTH * HEIGHT);
    }
  else
    {
      video_buffer = surface->pixels;
    }

  memset (palette, 0, sizeof (SDL_Color) * PALETTE_LEN);

  initialized = true;
}

void
video_quit ()
{
  if (false == initialized)
    {
      LOG_ERROR ("not initialized");

      return;
    }

  if (1 < ratio)
    {
      free (video_buffer);
    }

  SDL_Quit ();

  initialized = false;
}

void
video_set_palette_color (uint32_t index, uint32_t r, uint32_t g, uint32_t b)
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  ASSERT (PALETTE_LEN > index);
  ASSERT (64 > r);
  ASSERT (64 > g);
  ASSERT (64 > b);

  SDL_Color *color = xmalloc (sizeof (SDL_Color));
  color->r = r << 2;
  color->g = g << 2;
  color->b = b << 2;

  const int ret = SDL_SetPalette (surface, SDL_LOGPAL, color, index, 1);
  ASSERT (1 == ret);

  palette[index].r = color->r;
  palette[index].g = color->g;
  palette[index].b = color->b;

  free (color);
}

void
video_update ()
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  if (1 < ratio)
    {
      int y = 0;
      for (y = 0; HEIGHT > y; y++)
        {
          int x = 0;
          for (x = 0; WIDTH > x; x++)
            {
              const uint8_t color =
                  ((uint8_t *) video_buffer)[WIDTH * y + x];

              int j = 0;
              for (j = 0; ratio > j; j++)
                {
                  int i = 0;
                  for (i = 0; ratio > i; i++)
                    {
                      const int offset = WIDTH * ratio
                        * (ratio * y + j) + ratio * x + i;
                      ((uint8_t *) surface->pixels)[offset] = color;
                    }
                }
            }
        }
    }

  SDL_Flip (surface);
}
