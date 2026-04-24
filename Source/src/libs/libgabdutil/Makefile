include $(HOME)/src/libs/env_precomp.mk
TARGET      =libgabdutil.a

all :   LIBGABDUTIL

OBJETS = gabd_get_print.o \
		 gabd_seq_mng.o \
		 gabd_arret.o \
		 insert_text.o \
		 gabd_atm_mng.o \
		 gabd_key_mng.o \
		 gabd_get_flag.o \
		 gabd_status_mng.o \
		 gabd_format_field.o \
		 gabd_trans_resp_code.o \
		 gabd_terminal_config.o
.pc.o:
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CGC) $*.c

.c.o :
	$(CGC) $<

LIBGABDUTIL : $(OBJETS)
	ar -q -v $(TARGET) $(OBJETS) 
	mv $(TARGET) $(HPS_LIBPATH)

