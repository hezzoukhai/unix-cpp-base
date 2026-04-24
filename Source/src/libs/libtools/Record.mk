include $(HOME)/src/libs/env_precomp.mk

all :   Record

OBJECTS = Record.o

.c.o :
	$(CC) $<

Record : $(OBJECTS)
	$(GCC) $(LIBS) $(OBJECTS) -o Record
	mv Record $(BIN)/Record

