include $(HOME)/src/libs/env_precomp.mk

all :   LIBPICCPROD

OBJICCPROD = iccprod_ini.o \
             iccprod_analyse.o \
             iccprod_util.o \
             iccprod_dump.o \
             iccbuild_buf_header.o 

.c.o :
	$(CC) $<

LIBPICCPROD : $(OBJICCPROD)
	ar $(B64ARC)  -q -v libpiccprod.a $(OBJICCPROD)
	mv libpiccprod.a $(HPS_LIBPATH)















