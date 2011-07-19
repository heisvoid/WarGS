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
static uint8_t palette_index = 0;

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
  palette_index = 0;

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
video_set_palette_index (uint32_t index)
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  ASSERT (0 <= index && PALETTE_LEN > index);

  palette_index = index;
}

void
video_set_palette_color (uint32_t r, uint32_t g, uint32_t b)
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  ASSERT (64 > r);
  ASSERT (64 > g);
  ASSERT (64 > b);

  SDL_Color *color = xmalloc (sizeof (SDL_Color));
  color->r = r << 2;
  color->g = g << 2;
  color->b = b << 2;

  ASSERT (PALETTE_LEN > palette_index);

  const int ret = SDL_SetPalette (surface, SDL_LOGPAL, color,
                                  palette_index, 1);
  ASSERT (1 == ret);

  palette[palette_index].r = color->r;
  palette[palette_index].g = color->g;
  palette[palette_index].b = color->b;

  palette_index++;

  free (color);
}
