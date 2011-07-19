/* vim: set sw=2 ts=2 expandtab: */

#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

extern void video_init (uint8_t r);
extern void video_quit ();
extern void video_update ();

extern uint8_t video_get_ratio ();

#endif
