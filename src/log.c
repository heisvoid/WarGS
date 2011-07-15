/* vim: set sw=2 ts=2 expandtab: */

#include "log.h"

#include <string.h>
#include <libgen.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "util.h"
#include "filepath.h"

static bool
get_process_name (char *buf, size_t size)
{
  if (!__linux__)
    {
      return false;
    }

  if (NULL == buf)
    {
      return false;
    }

  char * const process_path = xcalloc (PATH_MAX, 1);
  const ssize_t ret = readlink ("/proc/self/exe", process_path, PATH_MAX);
  if (1 > ret)
    {
      return false;
    }

  const char * const process_name = basename (process_path);
  if (strlen (process_name) >= size)
    {
      return false;
    }

  strcpy (buf, process_name);

  return true;
}

#define LOG_COMMON()                                          \
    {                                                         \
       if (NULL == file || 1 > line || NULL == format)        \
        {                                                     \
          return;                                             \
        }                                                     \
                                                              \
      char *f = strdup (file);                                \
      if (NULL == f)                                          \
        {                                                     \
          return;                                             \
        }                                                     \
                                                              \
      const char * const file_name = basename (f);            \
      if (NULL == file_name)                                  \
        {                                                     \
          free (f);                                           \
                                                              \
          return;                                             \
        }                                                     \
                                                              \
      char * const process_name = xmalloc (PATH_MAX);         \
      if (true == get_process_name (process_name, PATH_MAX))  \
        {                                                     \
          fprintf (stderr, "%s: ", process_name);             \
        }                                                     \
      free (process_name);                                    \
                                                              \
      fprintf (stderr, "%s: %d: ", file_name, line);          \
                                                              \
      free (f);                                               \
                                                              \
      va_list ap;                                             \
      va_start (ap, format);                                  \
      vfprintf (stderr, format, ap);                          \
      va_end (ap);                                            \
                                                              \
      fprintf (stderr, "\n");                                 \
    }

void
log_error (const char *file, int line, const char *format, ...)
{
  LOG_COMMON ();
}

void
log_fatal (const char *file, int line, const char *format, ...)
{
  LOG_COMMON ();

  abort ();
}
