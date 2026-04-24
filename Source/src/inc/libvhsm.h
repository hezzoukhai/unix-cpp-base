/*
** <Name>
**  libvhsm.h
** 
** <Descr>
**  general header file for "libvhsm" library
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       11Mar02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef LIBVHSM_H
#define LIBVHSM_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */
#define VHSM_BUFFER_SIZE	256
#define CMD_MAX_NBR		625

#define ANSI_FORMAT		"01"
#define DIEBOLD_FORMAT		"03"
#define BULL_FORMAT		"10"

#define NO_ERROR				"00"
#define HSM_VERIFICATION_FAILED			"01"
#define CVK_A_OR_B_PARITY_ERROR			"10"
#define PARITY_ERROR_IN_1_2_3_COMPONENT		"11"
#define NO_KEYS_LOADED_IN_USER_STORAGE		"12"
#define LMK_ERROR				"13"
#define ERROR_IN_INPUT_DATA			"15"
#define NOT_IN_THE_AUTHORISED_STATE		"17"
#define INVALIDE_USER_STORAGE_INDEX		"21"
#define VHSM_ERROR				"99"

#define VHSM_VERIF				"MTvHSM"

/* - Data Type definition Section ------------------------------------------ */
typedef struct TSHsmCmd {
    unsigned char	*command;
    int			(*v_command_xx)( unsigned char cmd[],  int cmd_len,
				 unsigned char resp[], int *resp_len);
} SHsmCmd;

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */

   /* - Private Functions Declaration */
   #ifdef _FOR_INT_USE_
   int v_command_gg();
   int v_command_fg();
   int v_command_aw();
   int v_command_fc();
   int v_command_cw();
   int v_command_cy();
   int v_command_za();
   int v_command_zc();
   int v_command_dg();
   int v_command_oe();
   int v_command_jc();
   int v_command_jg();
   int v_command_dc();
   int v_command_as();
   int v_command_ee();
   int v_command_ua();
   int v_command_be();
   int v_command_ma();
   int v_command_mc();
   int v_command_kq();
   int v_command_kr();
   int v_command_ku();
   int v_void();
   #endif /* _FOR_INT_USE_ */

/* - Varuiables Definition Section ----------------------------------------- */
   /* - Module Variables Declaration --------------------------------------- */
   #ifdef _FOR_INT_USE_                                                        
   SHsmCmd		sHsmCmd[] = 
   {
   /*----------------------------------------------------------------------------*/
   /*command	v_command_xx	commentaire				      */
   /*----------------------------------------------------------------------------*/
   {"GG",		v_command_gg},	/* CREATION ET IMPRESSION DE LA ZMK */
   {"AW",		v_command_aw}, /* INTEGRATION DE LA PAIRE CVK */
   {"FC",		v_command_fc}, /* INTEGRATION DE LA PAIRE PVK */
   {"CW",		v_command_cw},	/* CALCUL DU CVV */
   {"CY",		v_command_cy},	/* VERIFICATION DU CVV */
   {"ZA",		v_command_za},	/* CRYPTAGE D'UNE CLE  */
   {"ZC",		v_command_zc},	/* DECRYPTAGE D'UNE CLE  */
   {"DG",		v_command_dg},	/* GENERATION D'UN PVV */
   {"OE",		v_command_oe},	/* GENERATION ET IMPRESSION D'UNE TMK */
   {"JC",		v_command_jc},	/* EXTRACTION DU PIN */
   {"JG",		v_command_jg},	/* PIN FROM LMK TO ZPK*/
   {"DC",		v_command_dc},	/* VERIFICATION DU PVV */
   {"AS",		v_command_as},	/* GENERATION PAIRE CVK */
   {"FG",		v_command_fg},	/* GENERATION PAIRE PVK */
   {"EE",		v_command_ee},	/* CALCUL PIN METHOD IBM */
   {"UA",		v_command_ua},	/* CALCUL PIN METHOD PRIVEE */
   {"BE",		v_command_be},	/* VERIFICATION PIN METHODE DE COMPARAISON */
   {"MA",		v_command_ma},	/* CALCUL DU MAC */
   {"MC",		v_command_mc},	/* VERIFICATION DU MAC */
   {"KQ",		v_command_kq},	/* VERIFICATION DU ADQC */
   {"KU",		v_command_ku},	/* ISSUER SCRIPT MAC */
   {"EOT",		v_void},       /* END OF TABLE */
   /*----------------------------------------------------------------------------*/
   };
   #endif /* _FOR_INT_USE_ */                                                  
                                                                               
   /* - Module Variables Publication --------------------------------------- */
   #ifdef _FOR_EXT_USE_                                                        
   extern SHsmCmd sHsmCmd;
   #endif /* _FOR_INT_USE_ */                                                  

#endif /* LIBVHSM_H */

