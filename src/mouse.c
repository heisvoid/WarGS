/* vim: set sw=2 ts=2 expandtab: */

#include "mouse.h"

#include <stdbool.h>

#include "log.h"
#include "assert.h"
#include "video.h"

static bool initialized = false;
static bool enable_event_handling = false;

void
mouse_init ()
{
  if (true == initialized)
    {
      LOG_FATAL ("already initialized");
    }

  SDL_ShowCursor (SDL_DISABLE);

  enable_event_handling = false;

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

void
mouse_handle_event (const SDL_Event *event)
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  if (false == enable_event_handling)
    {
      return;
    }

  ASSERT (NULL != event);

  uint16_t mouse_isr_ax = 0; /* condition mask */
  uint16_t mouse_isr_bx = 0; /* button state */
  uint16_t mouse_isr_cx = 0; /* cursor column: 2 * x */
  uint16_t mouse_isr_dx = 0; /* cursor row */

  uint16_t x = 0;
  uint16_t y = 0;

  switch (event->type)
    {
    case SDL_MOUSEBUTTONDOWN:
      {
        const SDL_MouseButtonEvent * const ev = &event->button;
        switch (ev->button)
          {
          case SDL_BUTTON_LEFT:
            mouse_isr_ax |= (1 << 1);
            mouse_isr_bx |= (1 << 0);
            break;
          case SDL_BUTTON_RIGHT:
            mouse_isr_ax |= (1 << 3);
            mouse_isr_bx |= (1 << 1);
            break;
          default:
            break;
          }

        x = ev->x;
        y = ev->y;       
      }
      break;
    case SDL_MOUSEBUTTONUP:
      {
        const SDL_MouseButtonEvent * const ev = &event->button;
        switch (ev->button)
          {
          case SDL_BUTTON_LEFT:
            mouse_isr_ax |= (1 << 2);
            break;
          case SDL_BUTTON_RIGHT:
            mouse_isr_ax |= (1 << 4);
            break;
          default:
            break;
          }

        x = ev->x;
        y = ev->y;
      }
      break;
    case SDL_MOUSEMOTION:
      mouse_isr_ax |= (1 << 0);
      x = event->motion.x;
      y = event->motion.y;

      break;
    default:
      LOG_FATAL ("invalid event type");
    }

  mouse_isr_cx = x / video_get_ratio () * 2;
  mouse_isr_dx = y / video_get_ratio ();

  asm volatile ("movw %0,%%ax\n"
                "movw %1,%%bx\n"
                "movw %2,%%cx\n"
                "movw %3,%%dx\n"
                "call mouse_isr"
                : /* no output */
                : "g" (mouse_isr_ax), "g" (mouse_isr_bx),
                  "g" (mouse_isr_cx), "g" (mouse_isr_dx)
                : "eax", "ebx", "ecx", "edx");
}

void
mouse_enable_event_handling ()
{
  enable_event_handling = true;
}
