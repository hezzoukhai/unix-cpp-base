include $(HOME)/src/libs/env_precomp.mk
#include $(HOME)/src/libs/env_precomp_cc.mk
TARGET1          =libcomora.a
TARGET2          =libqueue.a

all :   LIBQUEUE LIBCOMORA

OBJBAL = queue.o

OBJORA = connect.o \
		 signal_mng.o \
		 dump.o \
		 date.o \
		 des_tools.o \
		 get_cryptogram.o \
		 cipher.o
		
.pc.o:
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CGC) $*.c

.c.o :
	$(CGC) $<

LIBCOMORA : $(OBJORA)
	ar $(B64ARC) -q -v $(TARGET1) $(OBJORA) 
	mv $(TARGET1) $(HPS_LIBPATH)

LIBQUEUE : $(OBJBAL)
	ar $(B64ARC) -q -v $(TARGET2) $(OBJBAL) 
	mv $(TARGET2) $(HPS_LIBPATH)

