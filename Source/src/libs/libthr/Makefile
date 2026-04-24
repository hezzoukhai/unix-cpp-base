include $(HOME)/src/libs/env_precomp.mk

all :   LIBPTHR

OBJETS = list_thr.o \
         list_ini.o \
         alarm_thr.o \
         saf_manage.o \
         sign_manage.o \
         cut_over_manage.o \
         bal_prod_conso.o \
         line_prod_conso.o \
         shutdown_common.o \
         time_prod_conso.o

.c.o :
	$(CC) $<

LIBPTHR : $(OBJETS)
	ar $(B64ARC) -q -v libpthr.a $(OBJETS) 
	mv libpthr.a $(HPS_LIBPATH)

