bin_PROGRAMS = wargs wargs-ttl wargs-combatii

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
										game.c game.h \
										keyboard.c keyboard.h \
										mouse.c mouse.h \
										ttl.S \
										video.c video.h \
										wargs-ttl.c \
										wrappers.S

wargs_combatii_SOURCES = $(COMMON_SRCS) \
												 asm.h \
												 combatii.S \
												 wargs-combatii.c

MKWRAPPERS = $(PYTHON) $(top_srcdir)/tool/mkwrappers

wrappers.S: $(top_srcdir)/tool/mkwrappers.conf
	$(MKWRAPPERS) $(MKWRAPPERS_FLAGS) -o $@ $<

CLEANFILES = wrappers.S
