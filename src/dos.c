/* vim: set sw=2 ts=2 expandtab: */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

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

void
dos_free (void *p)
{
  ASSERT (NULL != p);

  free (p);
}

struct dos_registers
{
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
  uint32_t esi;
  uint32_t edi;
  uint32_t eflags;
} __attribute__ ((packed));

static void
print_interrupt_info (int n, const struct dos_registers *regs)
{
  printf ("dos interrupt via int386/int386x ()\n"
          "  int: 0x%02"PRIx8"\n"
          "  eax: 0x%08"PRIx32"\n"
          "  ebx: 0x%08"PRIx32"\n"
          "  ecx: 0x%08"PRIx32"\n"
          "  edx: 0x%08"PRIx32"\n"
          "  esi: 0x%08"PRIx32"\n"
          "  edi: 0x%08"PRIx32"\n"
          "  eflags: 0x%08"PRIx32"\n",
          n, regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi,
          regs->edi, regs->eflags);

  if (0x31 == n && 0x0300 == (0xffff & regs->eax))
    {
      /* simulate real mode interrupt */

      struct dpmi_real_mode_call_structure
      {
        uint32_t edi;
        uint32_t esi;
        uint32_t ebp;
        uint32_t reserved;
        uint32_t ebx;
        uint32_t edx;
        uint32_t ecx;
        uint32_t eax;
        uint16_t flags;
        uint16_t es;
        uint16_t ds;
        uint16_t fs;
        uint16_t gs;
        uint16_t ip;
        uint16_t cs;
        uint16_t sp;
        uint16_t ss;
      } __attribute__ ((packed));

      const struct dpmi_real_mode_call_structure * const s =
          (const struct dpmi_real_mode_call_structure * const) regs->edi;

      printf ("\n");
      printf ("  dpmi real mode call structure\n"
              "    edi: 0x%08"PRIx32"\n"
              "    esi: 0x%08"PRIx32"\n"
              "    ebp: 0x%08"PRIx32"\n"
              "    reserved: 0x%08"PRIx32"\n"
              "    ebx: 0x%08"PRIx32"\n"
              "    edx: 0x%08"PRIx32"\n"
              "    ecx: 0x%08"PRIx32"\n"
              "    eax: 0x%08"PRIx32"\n"
              "    flags: 0x%04"PRIx16"\n"
              "    es: 0x%04"PRIx16"\n"
              "    ds: 0x%04"PRIx16"\n"
              "    fs: 0x%04"PRIx16"\n"
              "    gs: 0x%04"PRIx16"\n"
              "    ip: 0x%04"PRIx16"\n"
              "    cs: 0x%04"PRIx16"\n"
              "    sp: 0x%04"PRIx16"\n"
              "    ss: 0x%04"PRIx16"\n",
              s->edi, s->esi, s->ebp, s->reserved, s->ebx, s->edx,
              s->ecx, s->eax, s->flags, s->es, s->ds, s->fs, s->gs,
              s->ip, s->cs, s->sp, s->ss);
    }
}

int __attribute__ ((noreturn))
dos_int386 (int n, const struct dos_registers *in, struct dos_registers *out)
{
  print_interrupt_info (n, in);

  abort ();
}

int __attribute__ ((noreturn))
dos_int386x (int n, const struct dos_registers *in, struct dos_registers *out,
             void *s)
{
  print_interrupt_info (n, in);

  abort ();
}

void *
dos_memset (void *s, int c, size_t n)
{
  ASSERT (NULL != s);
  ASSERT (0 < n);

  return memset (s, c, n);
}

long int
dos_ftell (FILE *f)
{
  ASSERT (NULL != f);

  const long int offset = ftell (f);
  ASSERT (-1 != offset);

  return offset;
}

int
dos_fseek (FILE *f, long int offset, int whence)
{
  ASSERT (NULL != f);

  const int ret = fseek (f, offset, whence);
  ASSERT (0 == ret);

  return ret;
}

size_t
dos_fread (void *buf, size_t size, size_t nmemb, FILE *f)
{
  ASSERT (NULL != buf);
  ASSERT (0 < size);
  ASSERT (0 < nmemb);
  ASSERT (NULL != f);

  size_t total_read = 0;
  while (nmemb > total_read)
    {
      const size_t r = fread (buf + total_read * size, size,
                              nmemb - total_read, f);
      if (nmemb - total_read > r)
        {
          if (0 != ferror (f))
            {
              LOG_FATAL ("fread error");
            }

          if (0 != feof (f))
            {
              total_read += r;           

              break;
            }
        }

      total_read += r;
    }

  return total_read;
}

int
dos_fgetc (FILE *f)
{
  ASSERT (NULL != f);

  return fgetc (f);
}

int
dos_unlink (const char *path)
{
  ASSERT (NULL != path);

  const char * const native_path = filepath_transform (path);
  const int ret = unlink (native_path);
  free ((void *) native_path);

  return ret;
}

off_t
dos_lseek (int fd, off_t offset, int whence)
{
  ASSERT (2 < fd);

  const off_t ret = lseek (fd, offset, whence);
  ASSERT ((off_t) -1 != ret);

  return ret;
}

int
dos_creat (const char *path, int mode)
{
  ASSERT (NULL != path);

  const char * const native_path = filepath_transform (path);

  const int fd = creat (native_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  ASSERT (-1 != fd);

  free ((void *) native_path);

  return fd;
}

int
dos_write (int fd, const void *buf, unsigned int len)
{
  ASSERT (2 < fd);
  ASSERT (NULL != buf);
  ASSERT (0 < len);

  int total_written = 0;
  while (total_written < len)
    {
      const ssize_t written = write (fd, ((int8_t *) buf) + total_written,
                                     len - total_written);
      if (0 > written)
        {
          LOG_FATAL ("failed to write");
        }

      total_written += written;
    }

  return total_written;
}

char *
dos_strstr (const char *haystack, const char *needle)
{
  ASSERT (NULL != haystack);
  ASSERT (NULL != needle);

  return strstr (haystack, needle);
}

int
dos_vsprintf (char *buf, const char *format, va_list ap)
{
  ASSERT (NULL != buf);
  ASSERT (NULL != format);

  const int ret = vsprintf (buf, format, ap);
  ASSERT (0 <= ret);

  return ret;
}
