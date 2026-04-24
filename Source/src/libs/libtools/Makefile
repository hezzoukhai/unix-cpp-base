include $(HOME)/src/libs/env_precomp.mk

all :   LIBTOOLS

OBJTOOL = tools.o \
          x_or.o \
          des.o \
          b64.o \
          gen_maa.o

.c.o :
	$(CC) $<

LIBTOOLS : $(OBJTOOL)
	ar $(B64ARC) -q -v libtools.a $(OBJTOOL) 
	mv libtools.a $(HPS_LIBPATH)
