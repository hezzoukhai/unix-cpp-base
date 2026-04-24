include $(HOME)/src/libs/env_precomp.mk
TARGET          =libvhsm.a

all :   LIBVHSM 

OBJVHSM = vhsm_commands.o \
	  vhsm_mac_mng.o \
	  vhsm_cry_key.o \
	  vhsm_dec_key.o \
	  vhsm_gen_cvk.o \
	  vhsm_gen_cvv.o \
	  vhsm_gen_pin.o \
	  vhsm_gen_pvk.o \
	  vhsm_gen_pvv.o \
	  vhsm_gen_tmk.o \
	  vhsm_gen_zmk.o \
	  vhsm_get_lmk.o \
	  vhsm_int_cvk.o \
	  vhsm_int_pin.o \
	  vhsm_int_pvk.o \
	  vhsm_ver_pin.o \
	  vhsm_ver_cvv.o \
	  vhsm_ver_pvv.o \
	  vhsm_gen_mac.o \
	  vhsm_ver_mac.o \
	  vhsm_compute_arqc.o \
	  vhsm_util.o \
	  vhsm.o \
	  des_tools.o
		

.pc.o :
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CGC) $*.c

.c.o :
	$(CGC) $<

LIBVHSM : $(OBJVHSM)
	ar $(B64ARC) -q -v $(TARGET) $(OBJVHSM) 
	mv $(TARGET) $(HPS_LIBPATH)
