/* vim: set sw=2 ts=2 expandtab: */

#include "mouse.h"

#include <stdbool.h>

#include <SDL.h>

#include "log.h"

static bool initialized = false;

void
mouse_init ()
{
  if (true == initialized)
    {
      LOG_FATAL ("already initialized");
    }

  SDL_ShowCursor (SDL_DISABLE);

  initialized = true;
}

void
mouse_quit ()
{
  if (false == initialized)
  {
    LOG_FATAL ("not initialized");
  }

  initialized = false;
}
