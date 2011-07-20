/* vim: set sw=2 ts=2 expandtab: */

int
main (int argc, char *argv[])
{
  int ret = 0;
  asm volatile ("call combatii" : "=a" (ret) : "a" (argc), "d" (argv));

  return ret;
}
