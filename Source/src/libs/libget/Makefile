include $(HOME)/src/libs/env_precomp.mk

all :   LIBGET LIBG 

OBJS = get_resources.o get_pwc_globals.o

OBJG = get_res.o

.pc.o : 
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CC) $*.c

.c.o :
	$(CC) $<

LIBGET : $(OBJS)
	ar $(B64ARC) -q -v libget.a $(OBJS) 
	mv libget.a $(HPS_LIBPATH)

LIBG   : $(OBJG)
	ar $(B64ARC) -q -v libg.a $(OBJG) 
	mv libg.a $(HPS_LIBPATH)
