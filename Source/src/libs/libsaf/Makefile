include $(HOME)/src/libs/env_precomp.mk

all   : SAF_LIB

SafLib = saf_manage.o 

.pc.o :
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CC) $*.c

.c.o :
	$(CC) -c $(CFLAGS) $(CCINCLUDES)  $<

SAF_LIB : $(SafLib)
	ar $(B64ARC) -q -v libsaf.a $(SafLib)
	mv libsaf.a $(HPS_LIBPATH)
	touch *c
