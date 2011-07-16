bin_PROGRAMS = wargs wargs-ttl

wargs_SOURCES = assert.h \
								conf.c conf.h \
								filepath.c filepath.h \
								log.c log.h \
								util.c util.h \
								wargs.c

wargs_ttl_SOURCES = asm.h \
										ttl.S \
										wargs-ttl.c \
										wrappers.S

MKWRAPPERS = $(PYTHON) $(top_srcdir)/tool/mkwrappers

wrappers.S: $(top_srcdir)/tool/mkwrappers.conf
	$(MKWRAPPERS) $(MKWRAPPERS_FLAGS) -o $@ $<

CLEANFILES = wrappers.S
