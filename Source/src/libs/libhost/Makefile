include $(HOME)/src/libs/env_precomp.mk

all :   LIBPHOST 

OBJHOST =host_analyse.o\
	host_ini.o\
	host_util.o\
	host_dump.o
.pc.o:
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CC) $*.c

.c.o :
	$(CC) $<

LIBPHOST : $(OBJHOST)
	rm -f libphost.a
#	touch *c
	ar -q -v libphost.a $(OBJHOST) 
	mv libphost.a $(HPS_LIBPATH)
