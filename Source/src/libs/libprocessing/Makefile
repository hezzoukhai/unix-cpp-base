include $(HOME)/src/libs/env_precomp.mk

all :   LIBPROCESSING 

OBJPRE = check_resource_services.o \
	     check_resource.o \
	     get_pin_info.o \
	     control_processing.o \
	     pin_chg_mng.o \
	     get_action_code.o \
	     get_auth_number.o \
	     get_issuer_data_info.o \
	     get_original_data.o \
	     get_security_flags.o \
	 	pin_chg_key_mng.o \
         reversal_procedure.o \
	     stand_in.o \
	     maj_autho.o \
	     auth_utils.o \
		 autho_loan_utils.o \
	     autho_processing.o \
		 exception_processing.o \
	     balance_processing.o \
	     conversion_rate_processing.o \
	     conversion_rate_inquiry.o \
	     top_up_processing.o \
	     reversal_processing.o \
	     loan_processing.o \
		 loan_inquiry.o \
	     file_update_processing.o \
	     bill_payment_processing.o \
	     bill_payment_inquiry.o \
	     deposit_inquiry.o \
	     deposit_processing.o \
	     pin_change_processing.o \
	     control_limit.o \
	     maj_card_activity.o \
	     maj_balance.o \
	     update_terminal_pos_watch_act.o \
	     update_ptserv_activity.o \
		 autho_conf_proc.o \
		 authentication_processing.o \
		 cheque_processing.o \
		 cheque_control.o \
		 cheque_inquiry.o \
		 update_loan_account.o \
		 control_installment_amount.o \
		 transfer_processing.o \
		 check_loan_account.o \
		 set_loan_data.o \
		 statement_processing.o \
		 statement_inquiry.o \
		 stop_list_reply_processing.o \
		 balance_inquiry.o \
		 refund_processing.o \
		 autho_saf_processing.o \
		 cardless_processing.o \
		 settlement_mng.o \
		 get_timers.o \
		 get_trackII_record.o \
	     check_security.o \
	         amount_mng.o \
		bank_update_utils.o \
		check_fallback.o \
		check_card_inquiry.o\
                oct_auth_processing.o proc_preauth.o

.pc.o : 
	$(PROC) $(PROCPLSFLAGS) $(PROCPPFLAGS) iname=$* ireclen=132
	$(CC) $*.c

.c.o :
	$(CC) $<

LIBPROCESSING : $(OBJPRE)
	ar $(B64ARC) -q -v libprocessing.a $(OBJPRE)
	mv libprocessing.a $(HPS_LIBPATH)

