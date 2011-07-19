/* vim: set sw=2 ts=2 expandtab: */

#include "keyboard.h"

#include <SDL.h>

#include "assert.h"

static bool initialized = false;

void
keyboard_init ()
{
  if (true == initialized)
    {
      LOG_FATAL ("already initialized");
    }

  initialized = true;
}

void
keyboard_quit ()
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  initialized = false;
}

int
keyboard_esc_is_pressed ()
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  SDL_PumpEvents ();

  Uint8 *key_state = SDL_GetKeyState (NULL);
  ASSERT (NULL != key_state);

  return key_state[SDLK_ESCAPE];
}
