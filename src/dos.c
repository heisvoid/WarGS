/* vim: set sw=2 ts=2 expandtab: */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "assert.h"
#include "filepath.h"

int
dos_open (const char *path, int flags)
{
  ASSERT (NULL != path);

  enum
  {
    DOS_O_RDONLY      = 0x000,
    DOS_O_WRONLY      = 0x001,
    DOS_O_RDWR        = 0x002,
    DOS_O_APPEND      = 0x010,
    DOS_O_CREAT       = 0x020,
    DOS_O_TRUNC       = 0x040,
    DOS_O_NOINHERIT   = 0x080,
    DOS_O_TEXT        = 0x100,
    DOS_O_BINARY      = 0x200,
    DOS_O_EXCL        = 0x400
  };

#ifndef O_BINARY
#define O_BINARY 0
#endif

  static const int flag_mappings[][2] = {
    {DOS_O_RDONLY,      O_RDONLY},
    {DOS_O_WRONLY,      O_WRONLY},
    {DOS_O_RDWR,        O_RDWR},
    {DOS_O_APPEND,      O_APPEND},
    {DOS_O_CREAT,       O_CREAT},
    {DOS_O_TRUNC,       O_TRUNC},
    {DOS_O_NOINHERIT,   0},
    {DOS_O_TEXT,        0},
    {DOS_O_BINARY,      O_BINARY},
    {DOS_O_EXCL,        O_EXCL},
    {-1,                -1}
  };

  int native_flags = 0;
  size_t i = 0;
  for (i = 0; -1 != flag_mappings[i][0]; i++)
    {
      if (0 != (flag_mappings[i][0] & flags))
        {
          native_flags |= flag_mappings[i][1];
        }
    }

  const char * const native_path = filepath_transform (path);

  const int ret = open (native_path, native_flags);

  free ((void *) native_path);

  return ret;
}

int
dos_read (int fd, void *buf, unsigned int len)
{
  ASSERT (2 < fd);
  ASSERT (NULL != buf);
  ASSERT (0 < len);

  ssize_t read_len = 0;
  while (read_len < len)
    {
      const ssize_t ret = read (fd, buf + read_len, len - read_len);
      ASSERT (0 <= ret);

      if (0 == ret)
        {
          /* EOF */
          break;
        }

      read_len += ret;
    }

  return read_len;
}

int
dos_close (int fd)
{
  ASSERT (2 < fd);

  const int ret = close (fd);
  ASSERT (0 == ret);

  return 0;
}

void *
dos_malloc (size_t size)
{
  ASSERT (0 < size);

  void *p = malloc (size);
  ASSERT (NULL != p);

  return p;
}

FILE *
dos_fopen (const char *path, const char *mode)
{
  ASSERT (NULL != path);
  ASSERT (NULL != mode);

  const char * const native_path = filepath_transform (path);

  FILE * const file = fopen (native_path, mode);

  free ((void *) native_path);

  return file;
}
