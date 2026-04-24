include $(HOME)/src/libs/env_precomp.mk
TARGET          =libasyn.a

all :   LIBNETAHSM 

OBJHSM = async_com.o \
         com.o

.c.o :
	$(CGC) $<

LIBNETAHSM : $(OBJHSM)
	ar -q -v $(TARGET) $(OBJHSM) 
	mv $(TARGET) $(HPS_LIBPATH)

