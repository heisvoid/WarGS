/* vim: set sw=2 ts=2 expandtab: */

#include "conf.h"

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
