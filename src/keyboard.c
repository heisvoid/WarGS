/* vim: set sw=2 ts=2 expandtab: */

#include "keyboard.h"

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

static uint8_t
get_scan_code (SDLKey sym)
{
  return 0x00;
}

void
keyboard_handle_event (const SDL_KeyboardEvent *event)
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  ASSERT (NULL != event);

  uint8_t scan_code = get_scan_code (event->keysym.sym);
  if (SDL_KEYUP == event->type)
    {
      scan_code += 0x80;
    }

#ifdef CHP
  asm volatile ("call keyboard_isr" : : "a" (scan_code));
#endif
}
