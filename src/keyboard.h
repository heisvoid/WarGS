/* vim: set sw=2 ts=2 expandtab: */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

extern void keyboard_init ();
extern void keyboard_quit ();

extern int keyboard_esc_is_pressed ();

#endif
