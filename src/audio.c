/* vim: set sw=2 ts=2 expandtab: */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "audio.h"

#include <stdbool.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include "assert.h"
#include "util.h"

static bool initialized = false;
static char *music_root = NULL;
static Mix_Music *music = NULL;

void
finish_channel (int channel)
{
  ASSERT (0 <= channel);

  Mix_Chunk * const chunk = Mix_GetChunk (channel);
  ASSERT (NULL != chunk);

  if (0 == chunk->allocated)
    {
      free (chunk->abuf);
    }

  Mix_FreeChunk (chunk);
}

void
audio_init (const char *music)
{
  if (true == initialized)
    {
      LOG_FATAL ("already initialized");
    }

  ASSERT (NULL != music_root);

  int ret = SDL_InitSubSystem (SDL_INIT_AUDIO);
  ASSERT (0 == ret);

  ret = Mix_OpenAudio (22000, AUDIO_S16SYS, 2, 4096);
  ASSERT (0 == ret);

  Mix_ChannelFinished (&finish_channel);

  music_root = xmalloc (strlen (music) + 1);
  strcpy (music_root, music);

  music = NULL;

  initialized = true;
}

void
audio_quit ()
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  free (music_root);

  audio_stop ();

  Mix_CloseAudio ();
  SDL_QuitSubSystem (SDL_INIT_AUDIO);

  initialized = false;
}

static void
stop_music ()
{
  Mix_HaltMusic ();

  if (NULL != music)
    {
      Mix_FreeMusic (music);
    }
}

void
audio_stop ()
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  stop_music ();

  Mix_HaltChannel (-1);
}

void
audio_music_play (uint32_t track)
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  ASSERT (0 < track && 19 > track);

  stop_music ();

  /* 1: FILEPATH_SEPARATOR
   * 5: "music"
   * 2: track number
   * 4: ".ogg"
   * 1: '\0'
   */
  const size_t file_path_size = sizeof (char) * 
      (strlen (music_root) + 1 + 5 + 2 + 4 + 1);
  char * const file_path = xmalloc (file_path_size);
  xsnprintf (file_path, file_path_size, "%s%cmusic%02"PRId8".ogg",
             music_root, FILEPATH_SEPARATOR, track);

  music = Mix_LoadMUS (file_path);
  ASSERT (NULL != music);

  const int ret = Mix_PlayMusic (music, -1);
  ASSERT (0 == ret);
}

void
audio_sound_play (const uint8_t *raw, uint32_t len, int32_t loop)
{
  if (false == initialized)
    {
      LOG_FATAL ("not initialized");
    }

  ASSERT (NULL != raw);
  ASSERT (0 < len);

  /* The raw data is assumed as unsigned 8-bit mono. 
   * So it should be converted to signed 16-bit stereo.
   *
   * reference
   * http://www.dsprelated.com/showmessage/52896/1.php
   */
  int16_t * const converted = xmalloc (2 * 2 * len);
  size_t i = 0;
  for (i = 0; len > i; i++)
    {
      converted[2 * i] = ((int16_t) raw[i] - 128) << 8;
      converted[2 * i + 1] = converted[2 * i];
    }

  Mix_Chunk * const sound = Mix_QuickLoad_RAW ((Uint8 *) converted,
                                               2 * 2 * len);
  ASSERT (NULL != sound);

  const int ret = Mix_PlayChannel (-1, sound, (0 == loop) ? 0 : -1);
  ASSERT (-1 != ret);
}
