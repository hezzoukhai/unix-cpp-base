# Modif Hist
# Author            Date           Ref            Description
# M.A.WAHBI         26/06/2003     MAW20030626    Remove files pin_chg_mng.o & pin_chg_key_mng.o they are already on libprocessing

include $(HOME_ENV_MAKEFILE)/env_precomp.mk
TARGET          =libdblutil.a

all :   LIBDBLUTIL

OBJETS = dbl_down_load_field.o \
         dbl_watch_config.o \
         get_fit.o \
         dbl_get_profile.o \
         dbl_get_screen.o \
         dbl_get_state.o \
         dbl_util.o \
	 dbl_get_cassette_info.o \
	 hps_lib.o \
	 util_varchar.o \
	 dbl_atm_mng.o \
	 dbl_tools_convert.o \
	 hps_atm_cutoff.o \
	 dbl_get_reply_data.o \
	 dbl_format_field.o \
	 maj_watch_activity.o \
	 get_msg_indiv_group.o \
	 dbl_get_prints.o \
	 dbl_atm_mng.o \
	 dbl_insert_text.o \
	 dbl_pcrd_accounts.o \
	 dbl_tools_advice.o \
	 dbl_update_cassettes.o \
	 dbl_hps_atm_status_mng.o \
	 dbl_atm_seq_mng.o \
         get_action_info.o \
	 update_log_arrette.o \
	 get_statement_label.o \
         dbl_emv_data.o \
         pcrd_remittance_accounts.o \
         get_last_transaction.o \
	 dbl_emv_init_data.o



.pc.o:
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CC) $*.c

.c.o :
	$(CC) $<

LIBDBLUTIL : $(OBJETS)
	ar -q -v $(TARGET) $(OBJETS) 
	mv $(TARGET) $(HPS_LIBPATH)

clean: 
	rm -f *.o
