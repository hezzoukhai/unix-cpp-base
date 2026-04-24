include $(HOME)/src/libs/env_precomp.mk

all :   LIBSTATEMENT 

OBJPRE = statement_req_processing.o \
	statement_req_inquiry.o

.pc.o : 
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CC) $*.c

.c.o :
	$(CC) $<

LIBSTATEMENT : $(OBJPRE)
	ar $(B64ARC) -q -v libstatement.a $(OBJPRE) 
	mv libstatement.a $(HPS_LIBPATH)

