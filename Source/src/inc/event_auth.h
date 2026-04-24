/*--------------------------------------------------------------------------------------------------------------------------------- 
 Id                 Date                Author                          Comment                                                     
----------------------------------------------------------------------------------------------------------------------------------- 
 0001MK             18-02-2002          Mahjoub KARROUM         Initial Version                                                     
 0002YO             03-09-2002          Younes OUADI            Gestion de piste III non valide                                     
---------------------------------------------------------------------------------------------------------------------------------*/ 
/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      event_auth.h                                                           */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Define of the different autorisation events                         */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Add events defined in the PLSQL package event_global_vars            */
/*******************************************************************************/
#ifndef HPS_EVENT_H
#define HPS_EVENT_H

 /* =======================================  */
 /* Les Dinvers Events a retourner lors      */
 /* des erreurs de verif HSM                 */
 /* =======================================  */
#define ERROR_CVV1    				"021"
#define ERROR_CVV2   				"022"
#define ERROR_PIN    				"023"
#define ERROR_CRYPTO    			"024"
#define ERROR_SYCHRONISATION_KEY    "028"
#define PIN_NB_EXCEEDED			    "003"

/* 0002YO */
#define EVENT_INVALID_TRACK_III                 "031"
/* 0002YO */
#define APPROVED_EVENT       			"000" /*NAB06042020 PLUTONL-1127*/
#define ISSUER_NOT_AVAILABLE_EVENT      "801"
#define MALFUNCTION_EVENT         		"902"
#define NATIONAL_ROUTING_EVENT    		"998"
#define INTERNATIONAL_ROUTING_EVENT    	"999"
#define HSM_UNAVAILABLE_EVENT           "991"
#define ADVICE_ACCEPTED     			"987"  /*HAL25032022 PLUTONL-3952*/

 /* =======================================  */
 /* Les Dinvers Events a retourner lors      */
 /* des erreurs Autorisation                 */
 /* =======================================  */
#define CARD_NOT_DELIVRED                           "001" /* Carte non d�livr�e */
#define ERROR_SEC_FLAG                              "002" /* Error In Getting Security Flags */
#define PIN_NB_EXCEEDED                             "003" /* D�passement du nombre de PIN faux */
#define PIN_NB_ALREADY_EXCEEDED                     "004" /* Nombre de PIN faux d�j� sup�rieur � la limite */
#define EXPIRED_CARD                                "005" /* Carte expir�e */
#define VALIDITY_DATE_UPPER                         "007" /* D�but date validit� sup�rieur � la date du jour */
#define ROTATIVE_MEMORY_FRAUD                       "008" /* M�moire rotative douteuse */
#define CARD_NBR_DIFF_ISO1_ISO2                     "009" /* Diff�rence num�ro entre piste ISO1  ISO2 */
#define CARD_NBR_DIFF_ISO2_ISO3                     "010" /* Diff�rence num�ro carte entre piste ISO2  ISO3 */
#define DATE_VAL_DIFF_ISO1_ISO2                     "011" /* Diff�rence date validit� entre piste ISO1  ISO2 */
#define DATE_VAL_DIFF_ISO2_ISO3                     "012" /* Diff�rence date validit� entre piste ISO2  ISO3 */
#define ACCOUNT_NOT_FOUND                           "013" /* Compte bancaire inexistant dans la table BALANCE */
#define ERROR_DATE_VAL_ON                           "014" /* Date validit� erron�e Carte ON US */
#define ERROR_DATE_VAL_OFF                          "015" /* Date validit� erron�e Carte OFF US */
#define ERROR_SERVICE_CODE                          "016" /* Service code erron� */
#define SERVICE_CODE_NOT_PERMITTED                  "017" /* Service non permis (Code Service) */
#define SERVICE_NOT_SUPPORTED                       "018" /* Service non support� */
#define ON_CARD_OFF                                 "019" /* Carte nationale � l"�tranger */
#define OFF_CARD_ON                                 "020" /* Carte �trang�re national dans le pays */
#define ERROR_ICVV				    "034" /* MYJ231007 Mehdi Elyajizi Emv Control Verif Flags  Faux ICVV */
#define ERROR_CVV1                                  "021" /* Faux CVV1 */
#define ERROR_CVV2                                  "022" /* Faux CVV2 */
#define ERROR_PIN                                   "023" /* PIN/PVV faux */
#define ERROR_CRYPTO                                "024" /* Faux CryptoGram */
#define ERROR_TELECODE                              "025" /* Faux TELECODE */
#define ERROR_CCD                                   "026" /* Faux CCD */
#define ERROR_CVC3                                  "027" /* Faux CVC3  */    /* MJA21092011  CONTACTLESS */
#define PB_KEY_SYCHRONISATION                       "028" /* Probl�me Synchronisation cl�s */
#define INVALID_ISO1                                "029" /* Erreur Piste ISO1 */
#define INVALID_ISO2                                "030" /* Erreur Piste ISO2 */
#define INVALID_ISO3                                "031" /* Erreur Piste ISO3 */
#define INVALID_CHIP                                "032" /* Erreur PUCE */
#define INV_ISSUER_SERVICE                          "033" /* INVALID ISSUER SERVICE */
#define ERROR_CAVV                                  "034" /* Faux CAVV */ /*  MJA20130206 3D-SECURE */
#define DAILY_LIMIT_EXCEEDED                        "100" /* D�passement plafond quotidien */
#define DAILY_NBR_TRX_EXCEEDED                      "101" /* D�passement nombre quotidien de transaction */
#define PERIOD_LIMIT_EXCEEDED                       "102" /* D�passement plafond p�riodique */
#define PERIOD_NBR_TRX_EXCEEDED                     "103" /* D�passement nombre p�riodique de transaction */
#define TRX_INF_MIN                                 "104" /* Transaction inf�rieure au minimum */
#define TRX_INF_MAX                                 "105" /* Transaction sup�rieure au maximum */
#define DAILY_TOT_LIMIT_EXCEEDED                    "106" /* Daily Total Limit Exceeded */
#define DAILY_TOT_NBR_EXCEEDED                      "107" /* Daily Total Nbr Exceeded */
#define PER_TOT_LIMIT_EXCEEDED                      "108" /* Periodique Total Limit Exceeded */
#define PER_TOT_NBR_EXCEEDED                        "109" /* Periodique Total Nbr Exceeded */
#define DAILY_LIMIT_ID_AMNT_EXCEEDED                "110" /* Daily Limit Id Total amount Exceeded */
#define DAILY_LIMIT_ID_NBR_EXCEEDED                 "111" /* Daily Limit Id Total NUMBER Exceeded */
#define PER_LIMIT_ID_AMNT_EXCEEDED                  "112" /* Periodique Limit Id Total amount Exceeded */
#define PER_LIMIT_ID_NBR_EXCEEDED                   "113" /* Periodique Limit Id Total NUMBER Exceeded */
#define INVALID_PERIOD_DATE                         "114" /* Event Invalid Date Of Control Limit */
#define NO_CHECKING_ACC                             "118" /* No Checking Account */
#define NO_SAVING_ACC                               "119" /* No Saving Account */
#define NO_CREDIT_ACC                               "120" /* No Credit Account */
#define PARTIAL_AUTHORIZATION					    "200" /* Partial authorization */
#define EXCEPTION_LOCKED                            "300" /* Carte inexistante */
#define CARD_NOT_FOUND                              "301" /* Carte inexistante */
#define CARD_LOCKED                                 "302" /* Erreur/Lock sur lecture Carte */
#define CARD_PRODUCT_LOCKED                         "300" /* Erreur/Lock sur lecture Carte Product */
#define ACTIVITY_CARD_NOT_FOUND                     "303" /* Activit� carte inexistante */
#define ACTIVITY_CARD_LOCKED                        "304" /* Erreur/Lock sur lecture activit� carte */
#define ERROR_UPDATE_CARD_ACTIVITY                  "305" /* Erreur mise � jour activit� carte */
#define BALANCE_NOT_FOUND                           "306" /* Solde inexistant */
#define BALANCE_LOCKED                              "307" /* Erreur/Lock sur lecture Solde */
#define ERROR_UPDATE_BALANCE                        "308" /* Erreur mise � jour solde */
#define ACCOUNT_ACTIVITY_NOT_FOUND                  "309" /* Activit� cr�dit inexistante */
#define ACCOUNT_ACTIVITY_LOCKED                     "310" /* Erreur/Lock sur lecture activit� cr�dit */
#define ERROR_UPDATE_ACCOUNT_ACTIVIY                "311" /* Erreur mise � jour activit� cr�dit */
#define CLIENT_ACTIVITY_NOT_FOUND                   "312" /* Activit� client inexistante */
#define CLIENT_ACTIVITY_LOCKED                      "313" /* Erreur/Lock sur lecture activit� client */
#define ERROR_UPDATE_CLIENT_ACTIVIY                 "314" /* Erreur mise � jour activit� client */
#define PRODUCT_NOT_FOUND                           "315" /* Produit non d�fini */
#define PRODUCT_LOCKED                              "316" /* Erreur/Lock sur lecture produit */
#define INTERNAT_ONUS_FLAG                          "317" /* Erreur/Lock sur lecture Carte Product */
#define INSUFFICIENT_BALANCE                        "318" /* Balance non suffisante */
#define BALANCE_TYPE_NOT_FOUND                      "319" /* Balance Type not defined */
#define INVALID_MERCHANT                      		"320" /* Invalid Merchant         */
#define ACQ_CENTER_NOT_FOUND                        "331" /* Centre acqu�reur ind�fini */
#define ISS_CENTER_NOT_FOUND                        "332" /* Centre �metteur ind�fini */
#define ACQ_BANK_NOT_FOUND                          "333" /* Banque acqu�reur ind�finie */
#define ISS_BANK_NOT_FOUND                          "334" /* Banque �metteur ind�finie */
#define LOC_BANK_NOT_FOUND                          "338" /* Local Banque  ind�finie */
#define NETWORK_NOT_FOUND                           "335" /* R�seau ind�finie */
#define BANK_NETWORK_NOT_FOUND                      "336" /* Banque R�seau ind�finie */
#define OUTLET_NOT_FOUND                            "337" /* Point de service ind�fini */
#define EXCEPTION_LIMIT_NOT_FOUND                   "351" /* Indexe plafond dans exception ind�fini */
#define EXCEPTION_PERIOD_NOT_FOUND                  "352" /* P�riodicit� plafond dans exception ind�fini */
#define EXCEPTION_SCENARIO_NOT_FOUND                "353" /* Sc�nario autorisation dans exception ind�fini */
#define INDEX_LIMIT_NOT_FOUND                       "354" /* Indexe plafond ind�fini */
#define PERIOD_NOT_FOUND                            "355" /* P�riodicit� plafond ind�fini */
#define SCENARIO_NOT_FOUND                          "356" /* Sc�nario autorisation ind�fini */
#define SERVICE_CODE_NOT_SUPPORTED                  "357" /* Code service carte non support� */
#define EXCEPTION_ACTION_CODE                       "400" /* Code action impos� dans l"exception */
#define SCENARIO_ACTION_CODE                        "401" /* Code action impos� par sc�nario utilisateur */
#define INVALID_SERVICE_SETUP                       "402" /* Service transaction Invalid */
#define TRANS_LIMIT_LOCK                            "403" /* P7_trans_limit record locked */
#define ERR_ORIGNAL_REVERSAL                        "404" /* Error Getting Original Data */
#define NO_ORIGINAL_TRX                             "405" /* Original Transaction for reversal not found. */
#define TRX_ALREADY_REVERSED                        "406" /* Transction already reversed. */
#define ERR_PUT_TAG_REVERSAL_TLV                    "407" /* Error To Put Reversal TLV DATA */
#define ORIGI_TRX_FOUND_INCONSISTENT                "408" /* Original Transaction for reversal found but not consistent. */
#define TRX_DECLINED                                "409" /* Original Transaction is declined/ so No reversal */
#define TRX_LIMIT_NOT_FOUND                         "500" /* P7_TRANS_LIMIT ind�fini */
#define LIMIT_SETUP_NOT_FOUND                       "501" /* P7_LIMITS_SETUP ind�fini */
#define LIMIT_SETUP_TOTAL_NOT_FOUND                 "504" /* P7_LIMITS_SETUP ind�fini */
#define DFLT_TRX_LIMIT_NOT_FOUND                    "502" /* P7_TRANS_LIMIT_DEFAULT ind�fini */
#define EEROR_CALC_GLISS_PERIOD                     "503" /* Erreur durant calcul activit� p�riode glissante */
#define STOP_CARD_BIN                               "551" /* Carte oppos�e dans STOP_LIST_CARD_BIN_RANGE */
#define STOP_CARD_COUNTRY                           "552" /* Carte oppos�e dans STOP_LIST_COUNTRY */
#define STOP_CARD_MCC                               "553" /* Carte oppos�e dans STOP_LIST_MCC */
#define STOP_CARD_MASK                              "554" /* Carte oppos�e dans STOP_LIST_MASK */
#define ERROR_STOP_LIST_CARD_BATCH                  "555" /* -- Carte oppos�e dans STOP_LIST_CARD_BIN_RANGE */
#define STOP_CARD_ACCOUNT                           "547" /* Carte oppos�e dans STOP_LIST_MASK */
#define ERROR_STOP_LIST_CARD_BIN_RANGE              "550" /* -- Carte oppos�e dans STOP_LIST_CARD_BIN_RANGE */
#define ERROR_STOP_LIST_MASK                        "549" /* -- Carte oppos�e dans STOP_LIST_CARD_BIN_RANGE */
#define ERROR_STOP_LIST_CR_ACCOUNT                  "544" /* -- Access error on STOP_LIST_SHADOW_ACCOUNT Table in Select mode (SYSTEM ERROR) */
#define ERROR_STOP_LIST_MCC                         "543" /* -- Access error on STOP_LIST_MCC Table in Select mode (SYSTEM ERROR) */
#define STOP_CARD_BATCH                             "556" /* Carte oppos�e dans STOP_LIST_CARD_BATCH */
#define ERROR_STOP_LIST_ACCOUNT                     "548" /* -- Carte oppos�e dans STOP_LIST_CARD_BIN_RANGE */
#define ERROR_STOP_LIST_NETWORKS                    "546" /* -- Access error on STOP_LIST_NETWORKS Table in Select mode (SYSTEM ERROR) */
#define ERROR_STOP_LIST_COUNTRY                     "545" /* -- Access error on STOP_LIST_COUNTRY Table in Select mode (SYSTEM ERROR) */
#define CANCELLED_CARD                              "557" /* Carte annul�e */
#define REPLACED_CARD                               "558" /* Carte remplac�e */
#define STOP_ACCOUNT                                "559" /* Compte cr�dit oppos� */
#define STOP_BALANCE                                "560" /* Compte bancaire oppos� */
#define STOP_NETWORKS                               "561" /* Card In Stop_LIST_NETWORKS */
#define RESOURCE_SERVICE_NOT_SUPPORTED              "600" /* Service non support� par la ressource */
#define RESOURCE_NOT_DEFINE                         "601" /* Ressource non d�finie */
#define RESOURCE_NOT_DISPONIBLE                     "602" /* Ressource non disponible */
#define NO_SWITCH_RESOURCE                          "603" /* Ressource non destin�e au routage */
#define RESOURCE_OFF                                "604" /* Ressource non d�marr�e */
#define PRODUCT_SERVICE_NOT_SUPPORTED               "605" /* Service non support� par le produit */
#define ERR_DATE_VAL_BD                             "606" /* Date fin validit� carte # fin validit� de la demande */
#define ERR_DATE_VAL_TRACK                          "607" /* Date fin validit� carte # fin validit� de la piste 1/2/3 */
#define ERR_SERVICE_CODE_TRACK                      "608" /* Code service carte # Code service piste 1/2/3 */
#define NO_PRODUCT_ROUTING                          "609" /* Routage Non Definis dans la table p7_product_swith */
#define NO_RANGE_ROUTING                            "610" /* Routage Non Definis dans la table p7_range_swith */
#define ERROR_RANGE_SCENARIO                        "611" /* Error Getting Range On Or OFF card range */
#define USAGE_CARD_2_COUNTRY_ERR                    "700" /* Transaction dans deux pays en moins de X heures par la m�me carte */
#define TIME_OUT_TRX                                "800" /* Time out sur une demande d"autorisation */
#define ISSUER_NOT_AVAILABLE                        "801" /* Emetteur indisponible */
#define ISSUER_UNKNOWN                              "802" /* Emetteur inconnu */
#define TLV_ERROR_DATA                              "803" /* Error In TLV DATA */
#define INVALID_REVERSAL_AMOUNT                     "804" /* Invalid reversal amount */
#define INVALID_REVERSAL_CURRENCY                   "805" /* Invalid reversal currency */
#define TLV_ERR_AMNT_DATA                           "806" /* Error In TLV Amount DATA Voir Package PCRD_P7_AUTH_AMOUNT_MNG */
#define INVALID_ISSUER_DATA                         "900" /* Error Loading Issuer DATA */
#define INVALID_EVENT_CODE                          "909" /* Event Code Invalid */
#define EVENT_SYSTEM_MALFUNCTION                    "902" /* Erreur syst�me */
#define SCRIPT_FAILED                               "903" /* Issuer script a echoue */  /* Mehdi ELyajizi Issuer Script event */
#define EVENT_HSM_UNAVAILABLE                       "991" /* HSM Down */
#define PURCHASE_CASH_BACK_NOT_ALLOWED              "992" /* Mehdi Elyajizi ENH 7.2 Terminal don't support 
							     Partial Approval for purchase with cash Back */
#define INVALID_CASH_BACK_TRANS_61                  "993" /* Mehdi Elyajizi ENH 7.2 Terminal don't support 
							     Partial Approval for purchase with cash Back*/
#define ERR_SYNCHRONISATION_KEY                     "996" /* Erreur synchronisation des cl�s de s�curit� */
#define ISSUER_ROUTING		                        "997" /* Issuer Routing */
#define NATIONAL_ROUTING                            "998" /* Routage national */

#endif  /**  HPS_EVENT_H **/
