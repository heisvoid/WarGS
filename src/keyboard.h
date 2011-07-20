/* vim: set sw=2 ts=2 expandtab: */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

#include <SDL.h>

extern void keyboard_init ();
extern void keyboard_quit ();

extern int keyboard_esc_is_pressed ();
extern void keyboard_handle_event (const SDL_KeyboardEvent *event);

#endif
