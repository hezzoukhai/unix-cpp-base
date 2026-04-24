##################################################################################
#
#  Descr    : Generic Makefile For LIBPRIVTLVEMV 
#
##################################################################################
#
#   Variables Definition
#
include $(HOME_ENV_MAKEFILE)/env_precomp.mk
TARGET  =    LIBPRIVTLVEMV
DEST		=-o $(HPS_LIBPATH)/$(TARGET)
#
#   Dependancies Generation Rule
#
all :  $(TARGET)
OBJPRIVEMV  =	private_emv_ini.o \
		private_emv_analyse.o \
		private_emv_util.o
#
# Precompilation de Por*C
#
.pc.o:
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CGC) $*.c
#
# Generation des objets
#
.c.o :
	$(CGC) $<
#
#   Target Generation Rule
#

LIBPRIVTLVEMV : $(OBJPRIVEMV)
	ar -q -v libprivtlvemv.a $(OBJPRIVEMV)
	mv libprivtlvemv.a $(HPS_LIBPATH)

