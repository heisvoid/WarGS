/* vim: set sw=2 ts=2 expandtab: */

int
main ()
{
  int ret = 0;
  asm volatile ("call ttl" : "=a" (ret));

  return ret;
}
