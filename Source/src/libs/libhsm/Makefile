include $(HOME)/src/libs/env_precomp.mk
TARGET          =libhsmtdes.a


all   : HSM_LIB_TDES


OBJECTS = hsm_commands.o \
	hsm_utils.o \
	hsm_pin_chg.o \
	hsm_thr.o \
	translation_lib.o \
	verification_lib.o \
	Production.o\
	trans_key.o\
	hps_key_mng.o\
	hps_gen_key.o\
	mac_mng.o\
	get_hsm_keys.o

.pc.o :
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CGC) $*.c

.c.o :
	$(CGC) $<

HSM_LIB_TDES : $(OBJECTS)
	ar $(B64ARC) -q -v $(TARGET) $(OBJECTS)
	mv $(TARGET) $(HPS_LIBPATH)
