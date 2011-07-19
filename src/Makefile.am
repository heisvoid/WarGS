bin_PROGRAMS = wargs wargs-ttl

COMMON_SRCS = assert.h \
					 	  conf.c conf.h \
							filepath.c filepath.h \
							log.c log.h \
							util.c util.h

wargs_SOURCES = $(COMMON_SRCS) \
								wargs.c

wargs_ttl_SOURCES = $(COMMON_SRCS) \
										asm.h \
										audio.c audio.h \
										dos.c \
										game.c \
										keyboard.c keyboard.h \
										mouse.c mouse.h \
										ttl.S \
										video.c video.h \
										wargs-ttl.c \
										wrappers.S

MKWRAPPERS = $(PYTHON) $(top_srcdir)/tool/mkwrappers

wrappers.S: $(top_srcdir)/tool/mkwrappers.conf
	$(MKWRAPPERS) $(MKWRAPPERS_FLAGS) -o $@ $<

CLEANFILES = wrappers.S
