#ifndef PWC_HSM_C2P__DEFINE_H
#define PWC_HSM_C2P__DEFINE_H





/* Commands requests */
#define C2P_CMD_REQ_VERIFY_AC_E2 				"E2"		/*Commande de vérification d'AC (Application Cryptogram)*/
#define C2P_CMD_REQ_GEN_ARPC_E3 				"E3"		/*Commande de calcul d'ARPC (Authorisation ResPonse Cryptogram)*/
#define C2P_CMD_REQ_GEN_MAC_E4 					"E4"		/*Commande de calcul de MAC*/
#define C2P_CMD_REQ_ENC_DATA_E5 				"E5"		/*Commande de chiffrement des données*/
#define C2P_CMD_REQ_VERIFY_DAC_E6 				"E6"		/*Commande de vérification de DAC (Data Authentication Code)*/
#define C2P_CMD_REQ_VERIFY_IDN_E7 				"E7"		/*Commande de vérification d’IDN (ICC Dynamic Number)*/
#define C2P_CMD_REQ_PIN_CHG_E8 					"E8"		/*Commande de changement de PIN*/
#define C2P_CMD_REQ_GEN_KEY_E9 					"E9"		/*Commande de tirage de clé symétrique*/
#define C2P_CMD_REQ_SAVE_KEY_EA 				"EA"		/*Commande d’imposition de clé symétrique*/
#define C2P_CMD_REQ_IMPORT_KEY_EB 				"EB"		/*Commande d’importation d'une clé symétrique*/
#define C2P_CMD_REQ_DEL_KEY_EC 					"EC"		/*Commande de suppression d'une clé symétrique*/
#define C2P_CMD_REQ_VERIFY_KEY_ED 				"ED"		/*Commande de vérification de clé*/
#define C2P_CMD_REQ_GEN_RSA_F0 					"F0"		/*Commande de tirage d'un bi-clé RSA*/
#define C2P_CMD_REQ_EXT_PUBLIC_KEY_F1 					"F1"		/*Commande d’externalisation d’une clé publique*/
#define C2P_CMD_REQ_AUTO_TEST_F2 				"F2"		/*Commande d’autotest*/
#define C2P_CMD_REQ_STAT_REPORT_F3 				"F3"		/*Commande de relevés statistiques*/
#define C2P_CMD_REQ_RFU_F4_F4 					"F4"		/*Commande d'imposition d'une clé asymétrique*/
#define C2P_CMD_REQ_GEN_SIG_F5 					"F5"		/*Commande de calcul de signature*/
#define C2P_CMD_REQ_VERIFY_SIG_F6 				"F6"		/*Commande de vérification de signature*/
#define C2P_CMD_REQ_GEN_FINGERPRINT_SHA_1_F7 	"F7"		/*Commande de calcul d’empreinte par l’algorithme SHA-1*/
#define C2P_CMD_REQ_GEN_HASH_F8 				"F8"		/*Commande de calcul de sceau*/
#define C2P_CMD_REQ_VERIFY_HASH_F9 				"F9"		/*Commande de vérification de sceau*/
#define C2P_CMD_REQ_GEN_AUTH_NO_FA 				"FA"		/*Commande de calcul du numéro d’autorisation*/
#define C2P_CMD_REQ_PIN_TRSLT_FB 				"FB"		/*Commande de transchiffrement de code confidentiel*/
#define C2P_CMD_REQ_VERIFY_PIN_FC 				"FC"		/*Commande de contrôle de code*/
#define C2P_CMD_REQ_VERIFY_CVX_FD 				"FD"		/*Commande de vérification de CVX*/
#define C2P_CMD_REQ_VERIFY_CVX2_FE 				"FE"		/*Commande de vérification de CVX2*/
#define C2P_CMD_REQ_EXPORT_KEY_FF20 			"FF20"		/*Commande d’exportation d’une clé symétrique*/
#define C2P_CMD_REQ_RFU_FF21_FF21 				"FF21"		/*Commande de transchiffrement d’un jeton de clé symétrique*/
#define C2P_CMD_REQ_RFU_FF22_FF22 				"FF22"		/*Commande de sortie d’une clé symétrique*/
#define C2P_CMD_REQ_RFU_FF23_FF23 				"FF23"		/*Commande d’introduction d’une clé symétrique*/
#define C2P_CMD_REQ_RFU_FF24_FF24 				"FF24"		/*Commande de vérification de l’élément d’authentification B0’V3 – Supprimée*/
#define C2P_CMD_REQ_RFU_FF25_FF25 				"FF25"		/*Commande de vérification de CAI B0’V3– Supprimée*/
#define C2P_CMD_REQ_RFU_FF26_FF26 				"FF26"		/*Commande de chiffrement des données d’authentification ANSI2*/
#define C2P_CMD_REQ_RFU_FF27_FF27 				"FF27"		/*Commande de contrôle des données d’authentification ANSI2*/
#define C2P_CMD_REQ_RFU_FF28_FF28 				"FF28"		/*Commande de génération de code confidentiel*/
#define C2P_CMD_REQ_GEN_PVV_FF29 				"FF29"		/*Commande de calcul de la valeur de contrôle du code confidentiel*/
#define C2P_CMD_REQ_GEN_CVX_FF2A 				"FF2A"		/*Commande de calcul du CVX*/
#define C2P_CMD_REQ_GEN_CVX2_FF2B 				"FF2B"		/*Commande de calcul du CVX2*/
#define C2P_CMD_REQ_RFU_FF2C_FF2C 				"FF2C"		/*Commande de sortie du code confidentiel en clair*/
#define C2P_CMD_REQ_BUILD_AUTH_TICKET_FF2D 		"FF2D"		/*Commande de constitution de ticket d’authentification*/
#define C2P_CMD_REQ_VERIFY_AUTH_TICKET_FF2E 	"FF2E"		/*Commande de vérification de ticket d’authentification*/
#define C2P_CMD_REQ_GEN_SPA_HMAC_FF2F 			"FF2F"		/*Commande de calcul de HMAC SPA*/
#define C2P_CMD_REQ_VERIFY_SPA_HMAC_FF42 		"FF42"		/*Commande de vérification de HMAC SPA*/
#define C2P_CMD_REQ_VERIFY_CSC_FF43 			"FF43"		/*Commande de vérification de CSC*/
#define C2P_CMD_REQ_GEN_CSC_FF44 				"FF44"		/*Commande de calcul de CSC*/
#define C2P_CMD_REQ_GEN_CARD_KEY_KCV_FF45 		"FF45"		/*Commande de calcul de KCV d’une clé carte*/
#define C2P_CMD_REQ_RFU_FF46_FF46 				"FF46"		/*Commande de chiffrement des données de sortie*/
#define C2P_CMD_REQ_RFU_FF47_FF47 				"FF47"		/*Commande de dérivation de clés cartes*/
#define C2P_CMD_REQ_RFU_FF48_FF48 				"FF48"		/*Commande de calcul de DAC*/
#define C2P_CMD_REQ_RFU_FF49_FF49 				"FF49"		/*Commande de création de clé*/
#define C2P_CMD_REQ_RFU_FF4A_FF4A 				"FF4A"		/*Commande de Chiffrement avec clé symétrique*/
#define C2P_CMD_REQ_RFU_FF4B_FF4B 				"FF4B"		/*Commande de Déchiffrement avec clé symétrique*/
#define C2P_CMD_REQ_ENCRYPT_RSA_FF4C 				"FF4C"		/*Commande de Chiffrement RSA*/
#define C2P_CMD_REQ_RFU_FF4D_FF4D 				"FF4D"		/*Commande de tirage de bi-clé RSA carte*/
#define C2P_CMD_REQ_RFU_FF4E_FF4E 				"FF4E"		/*Commande de dérivation de clé MONEO*/
#define C2P_CMD_REQ_RFU_FF4F_FF4F 				"FF4F"		/*Commande d'importation de clé MONEO*/
#define C2P_CMD_REQ_RFU_FF60_FF60 				"FF60"		/*Commande de calcul du message d'authentification FISC*/
#define C2P_CMD_REQ_RFU_FF61_FF61 				"FF61"		/*Commande de vérification de TAC (Transaction Authentication Code)*/
#define C2P_CMD_REQ_RFU_FF62_FF62 				"FF62"		/*Commande d'externalisation de clé CB (RFU)*/
#define C2P_CMD_REQ_RFU_FF63_FF63 				"FF63"		/*Commande de dérivation de clés MPCOS*/
#define C2P_CMD_REQ_RFU_FF64_FF64 				"FF64"		/*Commande d'impression du code confidentiel*/
#define C2P_CMD_REQ_RFU_FF65_FF65 				"FF65"		/*Commande de vérification de MAC MPCOS*/
#define C2P_CMD_REQ_RFU_FF66_FF66 				"FF66"		/*Commande de génération d'award MPCOS*/
#define C2P_CMD_REQ_RFU_FF67_FF67 				"FF67"		/*Commande de chiffrement pour Secure Messaging MPCOS*/
#define C2P_CMD_REQ_RFU_FF68_FF68 				"FF68"		/*Commande de dérivation de clé UKPT*/
#define C2P_CMD_REQ_RFU_FF69_FF69 				"FF69"		/*Commande de contrôle de code COFINOGA*/
#define C2P_CMD_REQ_RFU_FF6A_FF6A 				"FF6A"		/*Commande de génération de quantité de contrôle COFINOGA*/
#define C2P_CMD_REQ_DECRYPT_ASYM_KEY_FF6B 				"FF6B"		/*Commande de transchiffrement de clé asymétrique*/
#define C2P_CMD_REQ_RFU_FF6C_FF6C 				"FF6C"		/*Commande d'importation de clé d'une configuration émetteur CardInk (test uniquement)*/
#define C2P_CMD_REQ_RFU_FF6D_FF6D 				"FF6D"		/*Commande d’importation de code confidentiel*/
#define C2P_CMD_REQ_RFU_FF6E_FF6E 				"FF6E"		/*Commande d’exportation de clé initiale du terminal*/
#define C2P_CMD_REQ_RFU_FF6F_FF6F 				"FF6F"		/*Commande de vérification d’OTP OATH*/
#define C2P_CMD_REQ_RFU_FF8100_FF8100 			"FF8100"	/*Commande de vérification de code TOTAL*/
#define C2P_CMD_REQ_RFU_FF8101_FF8101 			"FF8101"	/*Commande de vérification du cryptogramme visuel TOTAL*/
#define C2P_CMD_REQ_RFU_FF8102_FF8102 			"FF8102"	/*Commande de calcul de cryptogramme visuel TOTAL*/
#define C2P_CMD_REQ_RFU_FF8103_FF8103 			"FF8103"	/*Commande de génération des données carte TOTAL*/
#define C2P_CMD_REQ_RFU_FF8104_FF8104 			"FF8104"	/*Commande de génération des données conducteur TOTAL*/
#define C2P_CMD_REQ_RFU_FF8105_FF8105 			"FF8105"	/*Commande de calcul ou contrôle de numéro d’autorisation TOTAL*/
#define C2P_CMD_REQ_RFU_FF8106_FF8106 			"FF8106"	/*Commande de verification des Working Key FISC*/
#define C2P_CMD_REQ_RFU_FF8107_FF8107 			"FF8107"	/*Commande de sécurisation des informations du compte*/
#define C2P_CMD_REQ_VERIFY_CVC3_FF8108 			"FF8108"	/*Commande de vérification du CVC3*/
#define C2P_CMD_REQ_GEN_IV_CVC3_FF8109 			"FF8109"	/*Commande de calcul de vecteur initial pour le CVC3*/
#define C2P_CMD_REQ_RFU_FF810A_FF810A 			"FF810A"	/*Commande de génération des clés de session APACS*/
#define C2P_CMD_REQ_RFU_FF810B_FF810B 			"FF810B"	/*Commande de mise à jour de la clé de base APACS*/
#define C2P_CMD_REQ_RFU_FF810C_FF810C 			"FF810C"	/*Commande de génération des clés de session ZKA*/
#define C2P_CMD_REQ_RFU_FF810D_FF810D 			"FF810D"	/*Commande de sortie de jeton de clé symétrique*/
#define C2P_CMD_REQ_RFU_FF810E_FF810E 			"FF810E"	/*Commande d’introduction de jeton de clé symétrique*/
#define C2P_CMD_REQ_RFU_FF810F_FF810F 			"FF810F"	/*Commande d'administration de carte porte clés*/
#define C2P_CMD_REQ_RFU_FF8120_FF8120 			"FF8120"	/*Commande de vérification de PIN propriétaire*/
#define C2P_CMD_REQ_RFU_FF8121_FF8121 			"FF8121"	/*Commande de création d’un jeton d’utilisateur*/
#define C2P_CMD_REQ_RFU_FF8122_FF8122 			"FF8122"	/*Commande de transchiffrement des données vers la carte*/
#define C2P_CMD_REQ_IMP_SEC_KEY_RSA_FF8123      	"FF8123"	/*Commande d’importation de clé secrète sous une clé RSA*/
#define C2P_CMD_REQ_EXP_SEC_KEY_RSA_FF8124 		"FF8124"	/*Commande d’exportation de clé secrète sous une clé RSA*/
#define C2P_CMD_REQ_RFU_FF8125_FF8125 			"FF8125"	/*Commande de création de jeton de clé publique certifiée*/
#define C2P_CMD_REQ_RFU_FF8126_FF8126 			"FF8126"	/*Commande d’imposition de jeton de clé publique certifiée*/
#define C2P_CMD_REQ_RFU_FF8127_FF8127 			"FF8127"	/*Commande de génération de clé ECDSA*/
#define C2P_CMD_REQ_RFU_FF8128_FF8128 			"FF8128"	/*Commande d’Établissement d'un secure channel – EMV/Global Platform – SCP 02’*/
#define C2P_CMD_REQ_RFU_FF8129_FF8129 			"FF8129"	/*Commande d’importation d'une clé symétrique PKCS#11*/
#define C2P_CMD_REQ_RFU_FF812A_FF812A 			"FF812A"	/*Commande d’exportation d’une clé PKCS#11 sous une clé symétrique PKCS#11*/
#define C2P_CMD_REQ_RFU_FF812B_FF812B 			"FF812B"	/*Commande de dérivation d’une clé symétrique PKCS#11*/
#define C2P_CMD_REQ_RFU_FF812C_FF812C 			"FF812C"	/*Commande d’Établissement d'un secure channel – IAS*/
#define C2P_CMD_REQ_RFU_FF812D_FF812D 			"FF812D"	/*Commande de génération de nombre aléatoire*/
#define C2P_CMD_REQ_RFU_FF812E_FF812E 			"FF812E"	/*Commande de scellement de clé exportée*/
#define C2P_CMD_REQ_RFU_FF812F_FF812F 			"FF812F"	/*Commande d'exportation de code confidentiel*/
#define C2P_CMD_REQ_RFU_FF8160_FF8160 			"FF8160"	/*Commande de tirage de clé ECDSA carte*/
#define C2P_CMD_REQ_RFU_FF8161_FF8161 			"FF8161"	/*Commande d'ajout / retrait du numéro de conducteur*/
#define C2P_CMD_REQ_RFU_FF8162_FF8162 			"FF8162"	/*Commande de transchiffrement d'utilisateur*/
#define C2P_CMD_REQ_RFU_FF8163_FF8163 			"FF8163"	/*Commande d'authentification externe*/
#define C2P_CMD_REQ_RFU_FF8164_FF8164 			"FF8164"	/*Commande d'import de bloc de clé TR31*/
#define C2P_CMD_REQ_RFU_FF8165_FF8165 			"FF8165"	/*Commande d'export de bloc de clé TR31*/
#define C2P_CMD_REQ_RFU_FF8166_FF8166 			"FF8166"	/*Commande de déchiffrement d'une réponse carte*/
#define C2P_CMD_REQ_RFU_FF8167_FF8167 			"FF8167"	/*Commande de chiffrement de la commande ChangeKey en mode 1 pour les cartes MIFARE AES*/
#define C2P_CMD_REQ_RFU_FF8168_FF8168 			"FF8168"	/*Commande d'exportation de clé maitre terminal*/
#define C2P_CMD_REQ_RFU_FF8169_FF8169 			"FF8169"	/*Commande d'établissement d'un secure channel MIFARE DESFire*/
#define C2P_CMD_REQ_RFU_FF816A_FF816A 			"FF816A"	/*Commande de chiffrement/déchiffrement de PAN*/
#define C2P_CMD_REQ_RFU_FF816B_FF816B 			"FF816B"	/*Commande de déchiffrement de piste reçue du terminal*/
#define C2P_CMD_REQ_RFU_FF816C_FF816C 			"FF816C"	/*Commande d'établissement de secure channel Global Platform SCP03*/
#define C2P_CMD_REQ_RFU_FF816D_FF816D 			"FF816D"	/*Commande d’Établissement d'un secure channel – SCP 80’*/
#define C2P_CMD_REQ_RFU_FF816E_FF816E 			"FF816E"	/*Commande de transchiffrement des données vers le domaine de sécurité*/
#define C2P_CMD_REQ_RFU_FF816F_FF816F 			"FF816F"	/*Commande d'importation de la clé publique du domaine de sécurité*/
#define C2P_CMD_REQ_RFU_FF8200_FF8200 			"FF8200"	/*Commande de vérification de sceau du log de sécurité*/
#define C2P_CMD_REQ_DECRYPT_RSA_FF8201 			"FF8201"	/*Commande de déchiffrement RSA*/
#define C2P_CMD_REQ_RFU_FF8202_FF8202 			"FF8202"	/*Commande de calcul de HMAC*/
#define C2P_CMD_REQ_RFU_FF8203_FF8203 			"FF8203"	/*Commande de vérification de HMAC*/
#define C2P_CMD_REQ_RFU_FF8204_FF8204 			"FF8204"	/*Commande d'exportation de clé d'une configuration émetteur CardInk (test uniquement)*/
#define C2P_CMD_REQ_RFU_FF8205_FF8205 			"FF8205"	/*Commande de changement de PIN Piste*/
#define C2P_CMD_REQ_RFU_FF8206_FF8206 			"FF8206"	/*Commande de changement de clé de stockage (RFU)*/
#define C2P_CMD_REQ_RFU_FF8207_FF8207 			"FF8207"	/*Commande de génération de clé de session INGENICO Espagne*/
#define C2P_CMD_REQ_RFU_FF8208_FF8208 			"FF8208"	/*Commande de transchiffrement de secret BANCOMAT*/
#define C2P_CMD_REQ_RFU_FF8209_FF8209 			"FF8209"	/*Commande de génération des secrets BANCOMAT*/
#define C2P_CMD_REQ_RFU_FF820A_FF820A 			"FF820A"	/*Commande de tirage de clé DSA*/
#define C2P_CMD_REQ_RFU_FF820B_FF820B 			"FF820B"	/*Commande de chiffrement /déchiffrement authentifié*/
#define C2P_CMD_REQ_RFU_FF820C_FF820C 			"FF820C"	/*Commande de diversification de clé carte*/
#define C2P_CMD_REQ_RFU_FF820D_FF820D 			"FF820D"	/*Commande de génération de PUK et mot de passe*/
#define C2P_CMD_REQ_RFU_FF820E_FF820E 			"FF820E"	/*Commande d’activation de clé privée*/
#define C2P_CMD_REQ_RFU_FF820F_FF820F 			"FF820F"	/*Commande d’établissement de secure channel CPS2Ter*/
#define C2P_CMD_REQ_RFU_FF8220_FF8220 			"FF8220"	/*Commande de vérification de MAC et déchiffrement avec clé dérivée GrDF*/
#define C2P_CMD_REQ_RFU_FF8221_FF8221 			"FF8221"	/*Commande chiffrement et MAC avec clé dérivée GrDF*/
#define C2P_CMD_REQ_RFU_FF8222_FF8222 			"FF8222"	/*Commande chiffrement et MAC avec clé non dérivée GrDF*/
#define C2P_CMD_REQ_RFU_FF8223_FF8223 			"FF8223"	/*Commande exportation des clés du DdC*/
#define C2P_CMD_REQ_RFU_FF8224_FF8224 			"FF8224"	/*Commande de génération d’AC*/
#define C2P_CMD_REQ_RFU_FF8225_FF8225 			"FF8225"	/*Commande de transchiffrement d’une donnée de profil de carte*/
#define C2P_CMD_REQ_RFU_FF8226_FF8226 			"FF8226"	/*Commande de génération des clés de mobile*/
#define C2P_CMD_REQ_RFU_FF8227_FF8227 			"FF8227"	/*Commande de génération des clés de session de mobile*/
#define C2P_CMD_REQ_RFU_FF8228_FF8228 			"FF8228"	/*Commande de génération des clés à usage limité*/
#define C2P_CMD_REQ_RFU_FF8229_FF8229 			"FF8229"	/*Commande de vérification de CVV dynamique temporel*/


/* Commands responses */
#define C2P_CMD_RESP_UNKOWN_FF01 				"FF01"		/*Réponse à Commande inconnue*/
#define C2P_CMD_RESP_VERIFY_AC_FF02 			"FF02"		/*Réponse à Commande E2 de vérification d'AC (Application Cryptogram)*/
#define C2P_CMD_RESP_GEN_ARPC_FF03 				"FF03"		/*Réponse à Commande E3 de calcul d'ARPC (Authorisation ResPonse Cryptogram)*/
#define C2P_CMD_RESP_GEN_MAC_FF04 				"FF04"		/*Réponse à Commande E4 de calcul de MAC*/
#define C2P_CMD_RESP_ENC_DATA_FF05 				"FF05"		/*Réponse à Commande E5 de chiffrement des données*/
#define C2P_CMD_RESP_VERIFY_DAC_FF06 			"FF06"		/*Réponse à Commande E6 de vérification de DAC (Data Authentication Code)*/
#define C2P_CMD_RESP_VERIFY_IDN_FF07 			"FF07"		/*Réponse à Commande E7 de vérification d’IDN (ICC Dynamic Number)*/
#define C2P_CMD_RESP_PIN_CHG_FF08 				"FF08"		/*Réponse à Commande E8 de changement de PIN*/
#define C2P_CMD_RESP_GEN_KEY_FF09 				"FF09"		/*Réponse à Commande E9 de tirage de clé symétrique*/
#define C2P_CMD_RESP_SAVE_KEY_FF0A 				"FF0A"		/*Réponse à Commande EA d’imposition de clé symétrique*/
#define C2P_CMD_RESP_IMPORT_KEY_FF0B 			"FF0B"		/*Réponse à Commande EB d’importation d'une clé symétrique*/
#define C2P_CMD_RESP_DEL_KEY_FF0C 				"FF0C"		/*Réponse à Commande EC de suppression d'une clé symétrique*/
#define C2P_CMD_RESP_VERIFY_KEY_FF0D 			"FF0D"		/*Réponse à Commande ED de vérification de clé*/
#define C2P_CMD_RESP_GEN_RSA_FF10 				"FF10"		/*Réponse à Commande F0 de tirage d'un bi-clé RSA*/
#define C2P_CMD_RESP_EXT_PUBLIC_KEY_FF11 				"FF11"		/*Réponse à Commande F1 d’externalisation d’une clé publique*/
#define C2P_CMD_RESP_AUTO_TEST_FF12 			"FF12"		/*Réponse à Commande F2 d’autotest*/
#define C2P_CMD_RESP_STAT_REPORT_FF13 			"FF13"		/*Réponse à Commande F3 de relevés statistiques*/
#define C2P_CMD_RESP_RFU_FF14_FF14 				"FF14"		/*Réponse à Commande F4 d'imposition d'une clé asymétrique*/
#define C2P_CMD_RESP_GEN_SIG_FF15 				"FF15"		/*Réponse à Commande F5 de calcul de signature*/
#define C2P_CMD_RESP_VERIFY_SIG_FF16 			"FF16"		/*Réponse à Commande F6 de vérification de signature*/
#define C2P_CMD_RESP_GEN_FINGERPRINT_SHA_1_FF17 	"FF17"	/*Réponse à Commande F7 de calcul d’empreinte par l’algorithme SHA-1*/
#define C2P_CMD_RESP_GEN_HASH_FF18 				"FF18"		/*Réponse à Commande F8 de calcul de sceau*/
#define C2P_CMD_RESP_VERIFY_HASH_FF19 			"FF19"		/*Réponse à Commande F9 de vérification de sceau*/
#define C2P_CMD_RESP_GEN_AUTH_NO_FF1A 			"FF1A"		/*Réponse à Commande FA de calcul du numéro d’autorisation*/
#define C2P_CMD_RESP_PIN_TRSLT_FF1B 			"FF1B"		/*Réponse à Commande FB de transchiffrement de code confidentiel*/
#define C2P_CMD_RESP_VERIFY_PIN_FF1C 			"FF1C"		/*Réponse à Commande FC de contrôle de code.*/
#define C2P_CMD_RESP_VERIFY_CVX_FF1D 			"FF1D"		/*Réponse à Commande FD de vérification de CVX*/
#define C2P_CMD_RESP_VERIFY_CVX2_FF1E 			"FF1E"		/*Réponse à Commande FE de vérification de CVX2*/
#define C2P_CMD_RESP_EXPORT_KEY_FF30 			"FF30"		/*Réponse à Commande d’exportation d’une clé symétrique*/
#define C2P_CMD_RESP_RFU_FF31_FF31 				"FF31"		/*Réponse à Commande de transchiffrement d’un jeton de clé symétrique*/
#define C2P_CMD_RESP_RFU_FF32_FF32 				"FF32"		/*Réponse à Commande de sortie d’une clé symétrique*/
#define C2P_CMD_RESP_RFU_FF33_FF33 				"FF33"		/*Réponse à Commande d’introduction d’une clé symétrique*/
#define C2P_CMD_RESP_RFU_FF34_FF34 				"FF34"		/*Réponse à Commande de vérification de l’élément d’authentification B0’V3– Supprimée*/
#define C2P_CMD_RESP_RFU_FF35_FF35 				"FF35"		/*Réponse à Commande de vérification de CAI B0’V3 – Supprimée*/
#define C2P_CMD_RESP_RFU_FF36_FF36 				"FF36"		/*Réponse à Commande de chiffrement des données d’authentification ANSI2*/
#define C2P_CMD_RESP_RFU_FF37_FF37 				"FF37"		/*Réponse à Commande de contrôle des données d’authentification ANSI2*/
#define C2P_CMD_RESP_RFU_FF38_FF38 				"FF38"		/*Réponse à Commande de génération de code confidentiel*/
#define C2P_CMD_RESP_GEN_PVV_FF39 				"FF39"		/*Réponse à Commande de calcul de la valeur de contrôle du code confidentiel*/
#define C2P_CMD_RESP_GEN_CVX_FF3A 				"FF3A"		/*Réponse à Commande de calcul du CVX*/
#define C2P_CMD_RESP_GEN_CVX2_FF3B 				"FF3B"		/*Réponse à Commande de calcul du CVX2*/
#define C2P_CMD_RESP_RFU_FF3C_FF3C 				"FF3C"		/*Réponse à Commande de sortie du code confidentiel en clair*/
#define C2P_CMD_RESP_BUILD_AUTH_TICKET_FF3D 	"FF3D"		/*Réponse à Commande de constitution de ticket d’authentification*/
#define C2P_CMD_RESP_VERIFY_AUTH_TICKET_FF3E 	"FF3E"		/*Réponse à Commande de vérification de ticket d’authentification*/
#define C2P_CMD_RESP_GEN_SPA_HMAC_FF3F 			"FF3F"		/*Réponse de calcul de HMAC SPA*/
#define C2P_CMD_RESP_VERIFY_SPA_HMAC_FF52 		"FF52"		/*Réponse de vérification de HMAC SPA*/
#define C2P_CMD_RESP_VERIFY_CSC_FF53 			"FF53"		/*Réponse de vérification de CSC*/
#define C2P_CMD_RESP_GEN_CSC_FF54 				"FF54"		/*Réponse de calcul de CSC*/
#define C2P_CMD_RESP_GEN_CARD_KEY_KCV_FF55 		"FF55"		/*Réponse de calcul de KCV d’une clé carte*/
#define C2P_CMD_RESP_RFU_FF56_FF56 				"FF56"		/*Réponse de chiffrement des données de sortie*/
#define C2P_CMD_RESP_RFU_FF57_FF57 				"FF57"		/*Réponse de dérivation des clés cartes EMV*/
#define C2P_CMD_RESP_RFU_FF58_FF58 				"FF58"		/*Réponse de calcul de DAC*/
#define C2P_CMD_RESP_RFU_FF59_FF59 				"FF59"		/*Réponse de création de clé*/
#define C2P_CMD_RESP_RFU_FF5A_FF5A 				"FF5A"		/*Réponse de Chiffrement avec clé symétrique*/
#define C2P_CMD_RESP_RFU_FF5B_FF5B 				"FF5B"		/*Réponse de Déchiffrement avec clé symétrique*/
#define C2P_CMD_RESP_ENCRYPT_RSA_FF5C 				"FF5C"		/*Réponse de Chiffrement RSA*/
#define C2P_CMD_RESP_RFU_FF5D_FF5D 				"FF5D"		/*Réponse de tirage de bi-clé RSA carte*/
#define C2P_CMD_RESP_RFU_FF5E_FF5E 				"FF5E"		/*Réponse de dérivation de clé MONEO*/
#define C2P_CMD_RESP_RFU_FF5F_FF5F 				"FF5F"		/*Réponse d'importation de clé MONEO*/
#define C2P_CMD_RESP_RFU_FF70_FF70 				"FF70"		/*Réponse de calcul du message d'authentification FISC*/
#define C2P_CMD_RESP_RFU_FF71_FF71 				"FF71"		/*Réponse de vérification de TAC (Transaction Authentication Code)*/
#define C2P_CMD_RESP_RFU_FF72_FF72 				"FF72"		/*Réponse d'externalisation de clé CB (RFU)*/
#define C2P_CMD_RESP_RFU_FF73_FF73 				"FF73"		/*Réponse de dérivation de clé MPCOS*/
#define C2P_CMD_RESP_RFU_FF74_FF74 				"FF74"		/*Réponse d'impression du code confidentiel*/
#define C2P_CMD_RESP_RFU_FF75_FF75 				"FF75"		/*Réponse de vérification de MAC MPCOS*/
#define C2P_CMD_RESP_RFU_FF76_FF76 				"FF76"		/*Réponse de génération d'award MPCOS*/
#define C2P_CMD_RESP_RFU_FF77_FF77 				"FF77"		/*Réponse de chiffrement pour Secure Messaging MPCOS*/
#define C2P_CMD_RESP_RFU_FF78_FF78 				"FF78"		/*Réponse de dérivation de clé UKPT*/
#define C2P_CMD_RESP_RFU_FF79_FF79 				"FF79"		/*Réponse de contrôle de code COFINOGA*/
#define C2P_CMD_RESP_RFU_FF7A_FF7A 				"FF7A"		/*Réponse de génération de quantité de contrôle COFINOGA*/
#define C2P_CMD_RESP_DECRYPT_ASYM_KEY_FF7B 				"FF7B"		/*Réponse de transchiffrement de clé asymétrique*/
#define C2P_CMD_RESP_RFU_FF7C_FF7C 				"FF7C"		/*Réponse d'importation de clé d'une configuration émetteur CardInk*/
#define C2P_CMD_RESP_RFU_FF7D_FF7D 				"FF7D"		/*Réponse d’importation de code confidentiel*/
#define C2P_CMD_RESP_RFU_FF7E_FF7E 				"FF7E"		/*Réponse d’exportation de clé initiale du terminal*/
#define C2P_CMD_RESP_RFU_FF7F_FF7F 				"FF7F"		/*Réponse de vérification d’OTP OATH*/
#define C2P_CMD_RESP_RFU_FF8110_FF8110 			"FF8110"	/*Réponse de vérification de code TOTAL*/
#define C2P_CMD_RESP_RFU_FF8111_FF8111 			"FF8111"	/*Réponse de vérification du cryptogramme visuel TOTAL*/
#define C2P_CMD_RESP_RFU_FF8112_FF8112 			"FF8112"	/*Réponse au calcul de cryptogramme visuel TOTAL*/
#define C2P_CMD_RESP_RFU_FF8113_FF8113 			"FF8113"	/*Réponse de génération des données carte TOTAL*/
#define C2P_CMD_RESP_RFU_FF8114_FF8114 			"FF8114"	/*Réponse de génération des données conducteur TOTAL*/
#define C2P_CMD_RESP_RFU_FF8115_FF8115 			"FF8115"	/*Réponse de calcul ou contrôle de numéro d’autorisation TOTAL*/
#define C2P_CMD_RESP_RFU_FF8116_FF8116 			"FF8116"	/*Réponse de verification des Working Key FISC*/
#define C2P_CMD_RESP_RFU_FF8117_FF8117 			"FF8117"	/*Réponse de sécurisation des informations du compte*/
#define C2P_CMD_RESP_VERIFY_CVC3_FF8118 		"FF8118"	/*Réponse de vérification du CVC3*/
#define C2P_CMD_RESP_GEN_IV_CVC3_FF8119 		"FF8119"	/*Réponse de calcul de vecteur initial pour le CVC3*/
#define C2P_CMD_RESP_RFU_FF811A_FF811A 			"FF811A"	/*Réponse de génération des clés de session APACS*/
#define C2P_CMD_RESP_RFU_FF811B_FF811B 			"FF811B"	/*Réponse de mise à jour de la clé de base APACS*/
#define C2P_CMD_RESP_RFU_FF811C_FF811C 			"FF811C"	/*Réponse de génération des clés de session ZKA*/
#define C2P_CMD_RESP_RFU_FF811D_FF811D 			"FF811D"	/*Réponse de sortie de jeton de clé symétrique*/
#define C2P_CMD_RESP_RFU_FF811E_FF811E 			"FF811E"	/*Réponse d’introduction de jeton de clé symétrique*/
#define C2P_CMD_RESP_RFU_FF811F_FF811F 			"FF811F"	/*Réponse d'administration de carte porte clés*/
#define C2P_CMD_RESP_RFU_FF8130_FF8130 			"FF8130"	/*Réponse de vérification de PIN propriétaire*/
#define C2P_CMD_RESP_RFU_FF8131_FF8131 			"FF8131"	/*Réponse de création d’un jeton d’utilisateur*/
#define C2P_CMD_RESP_RFU_FF8132_FF8132 			"FF8132"	/*Réponse de transchiffrement des données vers la carte*/
#define C2P_CMD_RESP_IMP_SEC_KEY_RSA_FF8133 		"FF8133"	/*Réponse d’importation de clé secrète sous une clé RSA*/
#define C2P_CMD_RESP_EXP_SEC_KEY_RSA_FF8134		"FF8134"	/*Réponse d’exportation de clé secrète sous une clé RSA*/
#define C2P_CMD_RESP_RFU_FF8135_FF8135 			"FF8135"	/*Réponse de création de jeton de clé publique certifiée*/
#define C2P_CMD_RESP_RFU_FF8136_FF8136 			"FF8136"	/*Réponse d’imposition de jeton de clé publique certifiée*/
#define C2P_CMD_RESP_RFU_FF8137_FF8137 			"FF8137"	/*Réponse de génération de clé ECDSA*/
#define C2P_CMD_RESP_RFU_FF8138_FF8138 			"FF8138"	/*Réponse d’Établissement d'un secure channel – EMV/Global Platform – SCP 02’*/
#define C2P_CMD_RESP_RFU_FF8139_FF8139 			"FF8139"	/*Réponse d’importation d'une clé symétrique PKCS#11*/
#define C2P_CMD_RESP_RFU_FF813A_FF813A 			"FF813A"	/*Réponse d’exportation d’une clé PKCS#11 sous une clé symétrique PKCS#11*/
#define C2P_CMD_RESP_RFU_FF813B_FF813B 			"FF813B"	/*Réponse de dérivation d’une clé symétrique PKCS#11*/
#define C2P_CMD_RESP_RFU_FF813C_FF813C 			"FF813C"	/*Réponse d’Établissement d'un secure channel – IAS*/
#define C2P_CMD_RESP_RFU_FF813D_FF813D 			"FF813D"	/*Réponse de génération de nombre aléatoire*/
#define C2P_CMD_RESP_RFU_FF813E_FF813E 			"FF813E"	/*Réponse de scellement de clé exportée*/
#define C2P_CMD_RESP_RFU_FF813F_FF813F 			"FF813F"	/*Réponse d'exportation de code confidentiel*/
#define C2P_CMD_RESP_RFU_FF8170_FF8170 			"FF8170"	/*Réponse de tirage de clé ECDSA carte*/
#define C2P_CMD_RESP_RFU_FF8171_FF8171 			"FF8171"	/*Réponse d'ajout / retrait du numéro de conducteur*/
#define C2P_CMD_RESP_RFU_FF8172_FF8172 			"FF8172"	/*Réponse de transchiffrement d'utilisateur*/
#define C2P_CMD_RESP_RFU_FF8173_FF8173 			"FF8173"	/*Réponse d'authentification externe*/
#define C2P_CMD_RESP_RFU_FF8174_FF8174 			"FF8174"	/*Réponse d'import de bloc de clé TR31*/
#define C2P_CMD_RESP_RFU_FF8175_FF8175 			"FF8175"	/*Réponse d'export de bloc de clé TR31*/
#define C2P_CMD_RESP_RFU_FF8176_FF8176 			"FF8176"	/*Réponse de déchiffrement d'une réponse carte*/
#define C2P_CMD_RESP_RFU_FF8177_FF8177 			"FF8177"	/*Réponse de chiffrement de la commande ChangeKey en mode 1 pour les cartes MIFARE AES*/
#define C2P_CMD_RESP_RFU_FF8178_FF8178 			"FF8178"	/*Réponse d'exportation de clé maitre terminal*/
#define C2P_CMD_RESP_RFU_FF8179_FF8179 			"FF8179"	/*Réponse d'établissement d'un secure channel MIFARE DESFire*/
#define C2P_CMD_RESP_RFU_FF817A_FF817A 			"FF817A"	/*Réponse de chiffrement/déchiffrement de PAN*/
#define C2P_CMD_RESP_RFU_FF817B_FF817B 			"FF817B"	/*Réponse de déchiffrement de piste reçue du terminal*/
#define C2P_CMD_RESP_RFU_FF817C_FF817C 			"FF817C"	/*Réponse d'établissement de secure channel Global Platform SCP03*/
#define C2P_CMD_RESP_RFU_FF817D_FF817D 			"FF817D"	/*Réponse d’Établissement d'un secure channel – SCP 80’*/
#define C2P_CMD_RESP_RFU_FF817E_FF817E 			"FF817E"	/*Réponse de transchiffrement des données vers le domaine de sécurité*/
#define C2P_CMD_RESP_RFU_FF817F_FF817F 			"FF817F"	/*Réponse d'importation de la clé publique du domaine de sécurité*/
#define C2P_CMD_RESP_RFU_FF8210_FF8210 			"FF8210"	/*Réponse de vérification de sceau du log de sécurité*/
#define C2P_CMD_RESP_DECRYPT_RSA_FF8211 			"FF8211"	/*Réponse de déchiffrement RSA*/
#define C2P_CMD_RESP_RFU_FF8212_FF8212 			"FF8212"	/*Réponse de calcul de HMAC*/
#define C2P_CMD_RESP_RFU_FF8213_FF8213 			"FF8213"	/*Réponse de vérification de HMAC*/
#define C2P_CMD_RESP_RFU_FF8214_FF8214 			"FF8214"	/*Réponse d'exportation de clé d'une configuration émetteur CardInk (test uniquement)*/
#define C2P_CMD_RESP_RFU_FF8215_FF8215 			"FF8215"	/*Réponse de changement de PIN Piste*/
#define C2P_CMD_RESP_RFU_FF8216_FF8216 			"FF8216"	/*Réponse de changement de clé de stockage (RFU)*/
#define C2P_CMD_RESP_RFU_FF8217_FF8217 			"FF8217"	/*Réponse de génération de clé de session INGENICO Espagne*/
#define C2P_CMD_RESP_RFU_FF8218_FF8218 			"FF8218"	/*Réponse de transchiffrement de secret BANCOMAT*/
#define C2P_CMD_RESP_RFU_FF8219_FF8219 			"FF8219"	/*Réponse de génération des secrets BANCOMAT*/
#define C2P_CMD_RESP_RFU_FF821A_FF821A 			"FF821A"	/*Réponse de tirge de clé DSA*/
#define C2P_CMD_RESP_RFU_FF821B_FF821B 			"FF821B"	/*Réponse de chiffrement /déchiffrement authentifié*/
#define C2P_CMD_RESP_RFU_FF821C_FF821C 			"FF821C"	/*Réponse de diversification de clé carte*/
#define C2P_CMD_RESP_RFU_FF821D_FF821D 			"FF821D"	/*Réponse de génération de PUK et mot de passe*/
#define C2P_CMD_RESP_RFU_FF821E_FF821E 			"FF821E"	/*Réponse d’activation de clé privée*/
#define C2P_CMD_RESP_RFU_FF821F_FF821F 			"FF821F"	/*Réponse d’établissement de secure channel CPS2Ter*/
#define C2P_CMD_RESP_RFU_FF8230_FF8230 			"FF8230"	/*Réponse de vérification de MAC et déchiffrement avec clé dérivée GrDF*/
#define C2P_CMD_RESP_RFU_FF8231_FF8231 			"FF8231"	/*Réponse chiffrement et MAC avec clé dérivée GrDF*/
#define C2P_CMD_RESP_RFU_FF8232_FF8232 			"FF8232"	/*Réponse chiffrement et MAC avec clé non dérivée GrDF*/
#define C2P_CMD_RESP_RFU_FF8233_FF8233 			"FF8233"	/*Réponse exportation des clés du DdC*/
#define C2P_CMD_RESP_RFU_FF8234_FF8234 			"FF8234"	/*Réponse de génération d’AC*/
#define C2P_CMD_RESP_RFU_FF8235_FF8235 			"FF8235"	/*Réponse de transchiffrement d’une donnée de profil de carte*/
#define C2P_CMD_RESP_RFU_FF8236_FF8236 			"FF8236"	/*Réponse de génération des clés de mobile*/
#define C2P_CMD_RESP_RFU_FF8237_FF8237 			"FF8237"	/*Réponse de génération des clés de session de mobile*/
#define C2P_CMD_RESP_RFU_FF8238_FF8238 			"FF8238"	/*Réponse de génération des clés à usage limité*/
#define C2P_CMD_RESP_RFU_FF8239_FF8239 			"FF8239"	/*Réponse de vérification de CVV dynamique temporel*/

/* below order is as per .... */
typedef enum {

	C2P_MN_UNKOWN_FF01					= 0	,

	C2P_MN_VERIFY_AC_E2						,
	C2P_MN_VERIFY_AC_FF02					,
	C2P_MN_GEN_ARPC_E3						,
	C2P_MN_GEN_ARPC_FF03					,
	C2P_MN_GEN_MAC_E4						,
	C2P_MN_GEN_MAC_FF04						,
	C2P_MN_ENC_DATA_E5						,
	C2P_MN_ENC_DATA_FF05					,
	C2P_MN_VERIFY_DAC_E6					,
	C2P_MN_VERIFY_DAC_FF06					,
	C2P_MN_VERIFY_IDN_E7					,
	C2P_MN_VERIFY_IDN_FF07					,
	C2P_MN_PIN_CHG_E8						,
	C2P_MN_PIN_CHG_FF08						,
	C2P_MN_GEN_KEY_E9						,
	C2P_MN_GEN_KEY_FF09						,
	C2P_MN_SAVE_KEY_EA						,
	C2P_MN_SAVE_KEY_FF0A					,
	C2P_MN_IMPORT_KEY_EB					,
	C2P_MN_IMPORT_KEY_FF0B					,
	C2P_MN_DEL_KEY_EC						,
	C2P_MN_DEL_KEY_FF0C						,
	C2P_MN_VERIFY_KEY_ED					,
	C2P_MN_VERIFY_KEY_FF0D					,
	C2P_MN_GEN_RSA_F0						,
	C2P_MN_GEN_RSA_FF10					,
	C2P_MN_EXT_PUBLIC_KEY_F1						,
	C2P_MN_EXT_PUBLIC_KEY_FF11					,
	C2P_MN_AUTO_TEST_F2						,
	C2P_MN_AUTO_TEST_FF12					,
	C2P_MN_STAT_REPORT_F3					,
	C2P_MN_STAT_REPORT_FF13					,
	C2P_MN_RFU_F4_F4						,
	C2P_MN_RFU_FF14_FF14					,
	C2P_MN_GEN_SIG_F5						,
	C2P_MN_GEN_SIG_FF15						,
	C2P_MN_VERIFY_SIG_F6					,
	C2P_MN_VERIFY_SIG_FF16					,
	C2P_MN_GEN_FINGERPRINT_SHA_1_F7			,
	C2P_MN_GEN_FINGERPRINT_SHA_1_FF17		,
	C2P_MN_GEN_HASH_F8						,
	C2P_MN_GEN_HASH_FF18					,
	C2P_MN_VERIFY_HASH_F9					,
	C2P_MN_VERIFY_HASH_FF19					,
	C2P_MN_GEN_AUTH_NO_FA					,
	C2P_MN_GEN_AUTH_NO_FF1A					,
	C2P_MN_PIN_TRSLT_FB						,
	C2P_MN_PIN_TRSLT_FF1B					,
	C2P_MN_VERIFY_PIN_FC					,
	C2P_MN_VERIFY_PIN_FF1C					,
	C2P_MN_VERIFY_CVX_FD					,
	C2P_MN_VERIFY_CVX_FF1D					,
	C2P_MN_VERIFY_CVX2_FE					,
	C2P_MN_VERIFY_CVX2_FF1E					,
	C2P_MN_EXPORT_KEY_FF20					,
	C2P_MN_EXPORT_KEY_FF30					,
	C2P_MN_RFU_FF21_FF21					,
	C2P_MN_RFU_FF31_FF31					,
	C2P_MN_RFU_FF22_FF22					,
	C2P_MN_RFU_FF32_FF32					,
	C2P_MN_RFU_FF23_FF23					,
	C2P_MN_RFU_FF33_FF33					,
	C2P_MN_RFU_FF24_FF24					,
	C2P_MN_RFU_FF34_FF34					,
	C2P_MN_RFU_FF25_FF25					,
	C2P_MN_RFU_FF35_FF35					,
	C2P_MN_RFU_FF26_FF26					,
	C2P_MN_RFU_FF36_FF36					,
	C2P_MN_RFU_FF27_FF27					,
	C2P_MN_RFU_FF37_FF37					,
	C2P_MN_RFU_FF28_FF28					,
	C2P_MN_RFU_FF38_FF38					,
	C2P_MN_GEN_PVV_FF29						,
	C2P_MN_GEN_PVV_FF39						,
	C2P_MN_GEN_CVX_FF2A						,
	C2P_MN_GEN_CVX_FF3A						,
	C2P_MN_GEN_CVX2_FF2B					,
	C2P_MN_GEN_CVX2_FF3B					,
	C2P_MN_RFU_FF2C_FF2C					,
	C2P_MN_RFU_FF3C_FF3C					,
	C2P_MN_BUILD_AUTH_TICKET_FF2D			,
	C2P_MN_BUILD_AUTH_TICKET_FF3D			,
	C2P_MN_VERIFY_AUTH_TICKET_FF2E			,
	C2P_MN_VERIFY_AUTH_TICKET_FF3E			,
	C2P_MN_GEN_SPA_HMAC_FF2F				,
	C2P_MN_GEN_SPA_HMAC_FF3F				,
	C2P_MN_VERIFY_SPA_HMAC_FF42				,
	C2P_MN_VERIFY_SPA_HMAC_FF52				,
	C2P_MN_VERIFY_CSC_FF43					,
	C2P_MN_VERIFY_CSC_FF53					,
	C2P_MN_GEN_CSC_FF44						,
	C2P_MN_GEN_CSC_FF54						,
	C2P_MN_GEN_CARD_KEY_KCV_FF45			,
	C2P_MN_GEN_CARD_KEY_KCV_FF55			,
	C2P_MN_RFU_FF46_FF46					,
	C2P_MN_RFU_FF56_FF56					,
	C2P_MN_RFU_FF47_FF47					,
	C2P_MN_RFU_FF57_FF57					,
	C2P_MN_RFU_FF48_FF48					,
	C2P_MN_RFU_FF58_FF58					,
	C2P_MN_RFU_FF49_FF49					,
	C2P_MN_RFU_FF59_FF59					,
	C2P_MN_RFU_FF4A_FF4A					,
	C2P_MN_RFU_FF5A_FF5A					,
	C2P_MN_RFU_FF4B_FF4B					,
	C2P_MN_RFU_FF5B_FF5B					,
	C2P_MN_ENCRYPT_RSA_FF4C					,
	C2P_MN_ENCRYPT_RSA_FF5C					,
	C2P_MN_RFU_FF4D_FF4D					,
	C2P_MN_RFU_FF5D_FF5D					,
	C2P_MN_RFU_FF4E_FF4E					,
	C2P_MN_RFU_FF5E_FF5E					,
	C2P_MN_RFU_FF4F_FF4F					,
	C2P_MN_RFU_FF5F_FF5F					,
	C2P_MN_RFU_FF60_FF60					,
	C2P_MN_RFU_FF70_FF70					,
	C2P_MN_RFU_FF61_FF61					,
	C2P_MN_RFU_FF71_FF71					,
	C2P_MN_RFU_FF62_FF62					,
	C2P_MN_RFU_FF72_FF72					,
	C2P_MN_RFU_FF63_FF63					,
	C2P_MN_RFU_FF73_FF73					,
	C2P_MN_RFU_FF64_FF64					,
	C2P_MN_RFU_FF74_FF74					,
	C2P_MN_RFU_FF65_FF65					,
	C2P_MN_RFU_FF75_FF75					,
	C2P_MN_RFU_FF66_FF66					,
	C2P_MN_RFU_FF76_FF76					,
	C2P_MN_RFU_FF67_FF67					,
	C2P_MN_RFU_FF77_FF77					,
	C2P_MN_RFU_FF68_FF68					,
	C2P_MN_RFU_FF78_FF78					,
	C2P_MN_RFU_FF69_FF69					,
	C2P_MN_RFU_FF79_FF79					,
	C2P_MN_RFU_FF6A_FF6A					,
	C2P_MN_RFU_FF7A_FF7A					,
	C2P_MN_DECRYPT_ASYM_KEY_FF6B					,
	C2P_MN_DECRYPT_ASYM_KEY_FF7B					,
	C2P_MN_RFU_FF6C_FF6C					,
	C2P_MN_RFU_FF7C_FF7C					,
	C2P_MN_RFU_FF6D_FF6D					,
	C2P_MN_RFU_FF7D_FF7D					,
	C2P_MN_RFU_FF6E_FF6E					,
	C2P_MN_RFU_FF7E_FF7E					,
	C2P_MN_RFU_FF6F_FF6F					,
	C2P_MN_RFU_FF7F_FF7F					,
	C2P_MN_RFU_FF8100_FF8100				,
	C2P_MN_RFU_FF8110_FF8110				,
	C2P_MN_RFU_FF8101_FF8101				,
	C2P_MN_RFU_FF8111_FF8111				,
	C2P_MN_RFU_FF8102_FF8102				,
	C2P_MN_RFU_FF8112_FF8112				,
	C2P_MN_RFU_FF8103_FF8103				,
	C2P_MN_RFU_FF8113_FF8113				,
	C2P_MN_RFU_FF8104_FF8104				,
	C2P_MN_RFU_FF8114_FF8114				,
	C2P_MN_RFU_FF8105_FF8105				,
	C2P_MN_RFU_FF8115_FF8115				,
	C2P_MN_RFU_FF8106_FF8106				,
	C2P_MN_RFU_FF8116_FF8116				,
	C2P_MN_RFU_FF8107_FF8107				,
	C2P_MN_RFU_FF8117_FF8117				,
	C2P_MN_VERIFY_CVC3_FF8108				,
	C2P_MN_VERIFY_CVC3_FF8118				,
	C2P_MN_GEN_IV_CVC3_FF8109				,
	C2P_MN_GEN_IV_CVC3_FF8119				,
	C2P_MN_RFU_FF810A_FF810A				,
	C2P_MN_RFU_FF811A_FF811A				,
	C2P_MN_RFU_FF810B_FF810B				,
	C2P_MN_RFU_FF811B_FF811B				,
	C2P_MN_RFU_FF810C_FF810C				,
	C2P_MN_RFU_FF811C_FF811C				,
	C2P_MN_RFU_FF810D_FF810D				,
	C2P_MN_RFU_FF811D_FF811D				,
	C2P_MN_RFU_FF810E_FF810E				,
	C2P_MN_RFU_FF811E_FF811E				,
	C2P_MN_RFU_FF810F_FF810F				,
	C2P_MN_RFU_FF811F_FF811F				,
	C2P_MN_RFU_FF8120_FF8120				,
	C2P_MN_RFU_FF8130_FF8130				,
	C2P_MN_RFU_FF8121_FF8121				,
	C2P_MN_RFU_FF8131_FF8131				,
	C2P_MN_RFU_FF8122_FF8122				,
	C2P_MN_RFU_FF8132_FF8132				,
	C2P_MN_IMP_SEC_KEY_RSA_FF8123				,
	C2P_MN_IMP_SEC_KEY_RSA_FF8133				,
	C2P_MN_EXP_SEC_KEY_RSA_FF8124				,
	C2P_MN_EXP_SEC_KEY_RSA_FF8134				,
	C2P_MN_RFU_FF8125_FF8125				,
	C2P_MN_RFU_FF8135_FF8135				,
	C2P_MN_RFU_FF8126_FF8126				,
	C2P_MN_RFU_FF8136_FF8136				,
	C2P_MN_RFU_FF8127_FF8127				,
	C2P_MN_RFU_FF8137_FF8137				,
	C2P_MN_RFU_FF8128_FF8128				,
	C2P_MN_RFU_FF8138_FF8138				,
	C2P_MN_RFU_FF8129_FF8129				,
	C2P_MN_RFU_FF8139_FF8139				,
	C2P_MN_RFU_FF812A_FF812A				,
	C2P_MN_RFU_FF813A_FF813A				,
	C2P_MN_RFU_FF812B_FF812B				,
	C2P_MN_RFU_FF813B_FF813B				,
	C2P_MN_RFU_FF812C_FF812C				,
	C2P_MN_RFU_FF813C_FF813C				,
	C2P_MN_RFU_FF812D_FF812D				,
	C2P_MN_RFU_FF813D_FF813D				,
	C2P_MN_RFU_FF812E_FF812E				,
	C2P_MN_RFU_FF813E_FF813E				,
	C2P_MN_RFU_FF812F_FF812F				,
	C2P_MN_RFU_FF813F_FF813F				,
	C2P_MN_RFU_FF8160_FF8160				,
	C2P_MN_RFU_FF8170_FF8170				,
	C2P_MN_RFU_FF8161_FF8161				,
	C2P_MN_RFU_FF8171_FF8171				,
	C2P_MN_RFU_FF8162_FF8162				,
	C2P_MN_RFU_FF8172_FF8172				,
	C2P_MN_RFU_FF8163_FF8163				,
	C2P_MN_RFU_FF8173_FF8173				,
	C2P_MN_RFU_FF8164_FF8164				,
	C2P_MN_RFU_FF8174_FF8174				,
	C2P_MN_RFU_FF8165_FF8165				,
	C2P_MN_RFU_FF8175_FF8175				,
	C2P_MN_RFU_FF8166_FF8166				,
	C2P_MN_RFU_FF8176_FF8176				,
	C2P_MN_RFU_FF8167_FF8167				,
	C2P_MN_RFU_FF8177_FF8177				,
	C2P_MN_RFU_FF8168_FF8168				,
	C2P_MN_RFU_FF8178_FF8178				,
	C2P_MN_RFU_FF8169_FF8169				,
	C2P_MN_RFU_FF8179_FF8179				,
	C2P_MN_RFU_FF816A_FF816A				,
	C2P_MN_RFU_FF817A_FF817A				,
	C2P_MN_RFU_FF816B_FF816B				,
	C2P_MN_RFU_FF817B_FF817B				,
	C2P_MN_RFU_FF816C_FF816C				,
	C2P_MN_RFU_FF817C_FF817C				,
	C2P_MN_RFU_FF816D_FF816D				,
	C2P_MN_RFU_FF817D_FF817D				,
	C2P_MN_RFU_FF816E_FF816E				,
	C2P_MN_RFU_FF817E_FF817E				,
	C2P_MN_RFU_FF816F_FF816F				,
	C2P_MN_RFU_FF817F_FF817F				,
	C2P_MN_RFU_FF8200_FF8200				,
	C2P_MN_RFU_FF8210_FF8210				,
	C2P_MN_DECRYPT_RSA_FF8201				,
	C2P_MN_DECRYPT_RSA_FF8211				,
	C2P_MN_RFU_FF8202_FF8202				,
	C2P_MN_RFU_FF8212_FF8212				,
	C2P_MN_RFU_FF8203_FF8203				,
	C2P_MN_RFU_FF8213_FF8213				,
	C2P_MN_RFU_FF8204_FF8204				,
	C2P_MN_RFU_FF8214_FF8214				,
	C2P_MN_RFU_FF8205_FF8205				,
	C2P_MN_RFU_FF8215_FF8215				,
	C2P_MN_RFU_FF8206_FF8206				,
	C2P_MN_RFU_FF8216_FF8216				,
	C2P_MN_RFU_FF8207_FF8207				,
	C2P_MN_RFU_FF8217_FF8217				,
	C2P_MN_RFU_FF8208_FF8208				,
	C2P_MN_RFU_FF8218_FF8218				,
	C2P_MN_RFU_FF8209_FF8209				,
	C2P_MN_RFU_FF8219_FF8219				,
	C2P_MN_RFU_FF820A_FF820A				,
	C2P_MN_RFU_FF821A_FF821A				,
	C2P_MN_RFU_FF820B_FF820B				,
	C2P_MN_RFU_FF821B_FF821B				,
	C2P_MN_RFU_FF820C_FF820C				,
	C2P_MN_RFU_FF821C_FF821C				,
	C2P_MN_RFU_FF820D_FF820D				,
	C2P_MN_RFU_FF821D_FF821D				,
	C2P_MN_RFU_FF820E_FF820E				,
	C2P_MN_RFU_FF821E_FF821E				,
	C2P_MN_RFU_FF820F_FF820F				,
	C2P_MN_RFU_FF821F_FF821F				,
	C2P_MN_RFU_FF8220_FF8220				,
	C2P_MN_RFU_FF8230_FF8230				,
	C2P_MN_RFU_FF8221_FF8221				,
	C2P_MN_RFU_FF8231_FF8231				,
	C2P_MN_RFU_FF8222_FF8222				,
	C2P_MN_RFU_FF8232_FF8232				,
	C2P_MN_RFU_FF8223_FF8223				,
	C2P_MN_RFU_FF8233_FF8233				,
	C2P_MN_RFU_FF8224_FF8224				,
	C2P_MN_RFU_FF8234_FF8234				,
	C2P_MN_RFU_FF8225_FF8225				,
	C2P_MN_RFU_FF8235_FF8235				,
	C2P_MN_RFU_FF8226_FF8226				,
	C2P_MN_RFU_FF8236_FF8236				,
	C2P_MN_RFU_FF8227_FF8227				,
	C2P_MN_RFU_FF8237_FF8237				,
	C2P_MN_RFU_FF8228_FF8228				,
	C2P_MN_RFU_FF8238_FF8238				,
	C2P_MN_RFU_FF8229_FF8229				,
	C2P_MN_RFU_FF8239_FF8239				,

	C2P_LAYOUT_TYPE_QTY
} E_C2P_LAYOUT_TYPE;


/* Tags */

#define		C2P_ISO_TRACK_1					"56"
#define		C2P_PAN							"5A"
#define		C2P_PAN_SEQ_NO					"5F34"
#define		C2P_CERT						"7F21"
#define		C2P_ARC							"8A"
#define		C2P_ARQC						"9F26"
#define		C2P_ATC							"9F36"
#define		C2P_DAC							"9F45"
#define		C2P_IDN							"9F4C"
#define		C2P_CVC3_TRACK1_BITMAP			"9F62"
#define		C2P_UN_ATC_TRACK1_BITMAP		"9F63"
#define		C2P_CVC3_TRACK2_BITMAP			"9F65"
#define		C2P_UN_ATC_TRACK2_BITMAP		"9F66"
#define		C2P_SESSION_KEY_GEN_USED_NBR	"C2"
#define		C2P_AC_ALGO_ID					"C3"
#define		C2P_AC_GEN_DATA					"C4"
#define		C2P_ARPC_GEN_ALGOID				"C5"
#define		C2P_ARPC						"C6"
#define		C2P_MAC_ALGO_ID					"C7"
#define		C2P_MAC_DATA					"C8"
#define		C2P_MAC_LEN						"C9"
#define		C2P_MAC							"CA"
#define		C2P_ENC_ALGO_ID					"CB"
#define		C2P_DATA_TO_ENC					"CC"
#define		C2P_ENC_DATA					"CD"
#define		C2P_DAC_GEN_ALGO_ID				"CE"
#define		C2P_TAG_CF						"CF"
#define		C2P_IDN_GEN_ALGO_ID				"D0"
#define		C2P_IDN_GEN_DATA				"D1"
#define		C2P_KEY_ID						"D2"
#define		C2P_KEY_USAGE					"D3"
#define		C2P_KEY_KCV						"D4"
#define		C2P_KEY_KEV						"D5"
#define		C2P_FINGERPRINT_GEN_DATA		"D6"
#define		C2P_FINGERPRINT					"D7"
#define		C2P_RSA_KEY_MOD_LEN				"D8"
#define		C2P_RSA_PUB_KEY_EXT_FMT_ID		"DA"
#define		C2P_RSA_PRIV_KEY_FMT_ID			"DB"
#define		C2P_RSA_PUB_KEY_MOD				"DC"
#define		C2P_RSA_PUB_KEY_EXP				"DD"
#define		C2P_ENC_KEY						"DE"
#define		C2P_SIG_ALGO_ID					"DF00"
#define		C2P_SIG_DATA					"DF01"
#define		C2P_SIGNATURE					"DF02"
#define		C2P_SIG_VERIF_ALGO_ID			"DF03"
#define		C2P_PAD_ALGO_ID					"DF04"
#define		C2P_ENC_PIN_BLK					"DF05"
#define		C2P_PIN_BLK_FMT					"DF06"
#define		C2P_ISO_TRACK_2					"DF07"
#define		C2P_CVX2						"DF08"
#define		C2P_TRACK2_CTRL_VALUE_OFST		"DF09"
#define		C2P_AUTH_DATA					"DF0A"
#define		C2P_AUTH_CODE_LEN				"DF0B"
#define		C2P_PIN_BLK_NA_IN				"DF0C"
#define		C2P_AUTH_NO						"DF0D"
#define		C2P_PIN_BLK_NA_OUT				"DF0E"
#define		C2P_KEY_IDENT_DATA				"DF0F"
#define		C2P_KEY_IDENT_TYPE				"DF10"
#define		C2P_TKN_KEY_ENC_ALGO_ID			"DF11"
#define		C2P_OAEP_ENC_PARAM				"DF12"
#define		C2P_RSA_PUB_KEY_ENC_KEY			"DF13"
#define		C2P_FILE_TYPE					"DF14"
#define		C2P_RECORDS_NBR					"DF15"
#define		C2P_TRACE_MODE					"DF16"
#define		C2P_C2P_NUMBER					"DF17"
#define		C2P_PIN_VERIF_NEG_RESP_NBR		"DF18"
#define		C2P_PIN_VERIF_REQ_NBR			"DF19"
#define		C2P_CVX_VERIF_NEG_RESP_NBR		"DF1A"
#define		C2P_CVX_VERIF_REQ_NBR			"DF1B"
#define		C2P_MODULE_STATE				"DF1C"
#define		C2P_APP_ID						"DF1D"
#define		C2P_VER_NO						"DF1E"
#define		C2P_SERIAL_NO					"DF1F"
#define		C2P_ENC_KEY_ALGO_ID				"DF20"
#define		C2P_KEY_GEN_INDICATOR			"DF21"
#define		C2P_KEY_OUT_SUPPORT				"DF22"
#define		C2P_KEY_DECOMP_MTHD				"DF23"
#define		C2P_KEY_IN_SUPPORT				"DF24"
#define		C2P_EXPIRY_DATE					"DF25"
#define		C2P_EXPORT_ID					"DF26"
#define		C2P_DIVER_VALUE					"DF27"
#define		C2P_TRANS_DATA					"DF28"
#define		C2P_DES_KEY_ENC_RSA_ALGO_ID		"DF29"
#define		C2P_DIVER_DATA					"DF2A"
#define		C2P_EXTERNAL_VAR				"DF2B"
#define		C2P_CARD_WORD_ADDR				"DF2C"
#define		C2P_CARD_WORD					"DF2D"
#define		C2P_AUTH_ITEM					"DF2E"
#define		C2P_CAI							"DF2F"
#define		C2P_VERIF_ALGO_ID				"DF30"
#define		C2P_CVX_GEN_ALGO				"DF31"
#define		C2P_BLOCK_RANGE					"DF32"
#define		C2P_KEY_COMP					"DF33"
#define		C2P_ENC_MTHD_ID					"DF34"
#define		C2P_KEY_DECOMP_MTHD_2			"DF35"
#define		C2P_COMP_NBR					"DF36"
#define		C2P_FORMAT_ID					"DF37"
#define		C2P_KCV_TYPE					"DF38"
#define		C2P_ANSI2_AUTH_KEY				"DF39"
#define		C2P_AUTH_FIELD					"DF3A"
#define		C2P_IMPLICIT_ENFORCEMENT_FLAG	"DF3B"
#define		C2P_DIVER_PIN_BLK_IN			"DF3C"
#define		C2P_DIVER_PIN_BLK_OUT			"DF3D"
#define		C2P_PIN_LEN						"DF3E"
#define		C2P_PIN							"DF3F"
#define		C2P_SERIAL_NO_2					"DF40"
#define		C2P_AUTH_TICKET					"DF41"
#define		C2P_AUTH_TICKET_VERIF_CERT		"DF42"
#define		C2P_KEY_DEL_FILTER				"DF43"
#define		C2P_KEYS_NBR					"DF44"
#define		C2P_TICKETS_COUNTER				"DF45"
#define		C2P_TICKETS_PRIO				"DF46"
#define		C2P_C2P_IDP						"DF47"
#define		C2P_AUTO_TEST_RESULT_FMT		"DF48"
#define		C2P_AVAIL_OPTIONS				"DF49"
#define		C2P_LOADED_OPTIONS				"DF4A"
#define		C2P_SESSION_KEY_ALGO_PARAMS		"DF4B"
#define		C2P_MASTER_KEY_NBR				"DF4C"
#define		C2P_KEY_LEN						"DF4D"
#define		C2P_SERVICE_CODE				"DF4E"
#define		C2P_HMAC_GEN_DATA				"DF4F"
#define		C2P_HMAC						"DF50"
#define		C2P_HMAC_LEN					"DF51"
#define		C2P_USAGE_PROFILE				"DF52"
#define		C2P_APP_STATE					"DF53"
#define		C2P_CSC_3						"DF54"
#define		C2P_CSC_4						"DF55"
#define		C2P_CSC_5						"DF56"
#define		C2P_DISTR_ACC_FLAG				"DF57"
#define		C2P_CARD_KEY_KCV_LEN			"DF58"
#define		C2P_CARD_KEY_KCV				"DF59"
#define		C2P_ENC_STEP					"DF5A"
#define		C2P_ENC_DATA_IN					"DF5B"
#define		C2P_DATA_ENC_MODE				"DF5C"
#define		C2P_ENC_PADDING_ALGO_ID			"DF5D"
#define		C2P_ENC_INIT_VECTOR				"DF5E"
#define		C2P_REPORT_DATA					"DF5F"
#define		C2P_ENC_DATA_OUT				"DF60"
#define		C2P_KEY_VALUE					"DF61"
#define		C2P_DATA_ENC_MODE_2				"DF62"
#define		C2P_DATA_LEN_RETREIVAL_MODE		"DF63"
#define		C2P_RSA_ENC_ALGO_ID				"DF64"
#define		C2P_RSA_ENC_DATA_IN				"DF65"
#define		C2P_RSA_ENC_DATA_OUT			"DF66"
#define		C2P_P							"DF67"
#define		C2P_Q							"DF68"
#define		C2P_DP							"DF69"
#define		C2P_DQ							"DF6A"
#define		C2P_INV_Q						"DF6B"
#define		C2P_CID							"DF6C"
#define		C2P_PUB_KEY_HASH				"DF6D"
#define		C2P_PUB_KEY_FINGERPRINT_REQ		"DF6E"
#define		C2P_AUTH_RC						"DF6F"
#define		C2P_ENC_MODE					"DF70"
#define		C2P_CARD_KEY_OUT_FMT			"DF71"
#define		C2P_CB_KEY						"DF72"
#define		C2P_PRINT_FMT					"DF73"
#define		C2P_AVOID_SEQ					"DF74"
#define		C2P_CONV_CHAIN					"DF75"
#define		C2P_AUTH_DATA_2					"DF76"
#define		C2P_EMV_CARD_MK_ALGO			"DF77"
#define		C2P_CARD_SERIAL_NBR				"DF78"
#define		C2P_CARD_TRN_COUNTER			"DF79"
#define		C2P_CREDIT_CERT_VALUE			"DF7A"
#define		C2P_CREDIT_CERT_ENC				"DF7B"
#define		C2P_DATA_1						"DF7C"
#define		C2P_P1							"DF7D"
#define		C2P_P2							"DF7E"
#define		C2P_ENC_DATA_TYPE				"DF7F"
#define		C2P_VAR_FIELD_00				"DF8100"
#define		C2P_VAR_FIELD_01				"DF8101"
#define		C2P_VAR_FIELD_02				"DF8102"
#define		C2P_VAR_FIELD_03				"DF8103"
#define		C2P_VAR_FIELD_04				"DF8104"
#define		C2P_VAR_FIELD_05				"DF8105"
#define		C2P_VAR_FIELD_06				"DF8106"
#define		C2P_VAR_FIELD_07				"DF8107"
#define		C2P_VAR_FIELD_08				"DF8108"
#define		C2P_VAR_FIELD_09				"DF8109"
#define		C2P_VAR_FIELD_10				"DF810A"
#define		C2P_VAR_FIELD_11				"DF810B"
#define		C2P_VAR_FIELD_12				"DF810C"
#define		C2P_VAR_FIELD_13				"DF810D"
#define		C2P_VAR_FIELD_14				"DF810E"
#define		C2P_VAR_FIELD_15				"DF810F"
#define		C2P_DATA_2_DATA_3				"DF8110"
#define		C2P_CODE_VERIF_ALGO				"DF8111"
#define		C2P_VAL_DATA					"DF8112"
#define		C2P_PIN_OFST_LEN				"DF8113"
#define		C2P_CODE_GEN_ALGO				"DF8114"
#define		C2P_PIN_OFST					"DF8115"
#define		C2P_KSN							"DF8116"
#define		C2P_DEC_MODE					"DF8117"
#define		C2P_ISS_AC_BITMAP				"DF8118"
#define		C2P_CARD_KEY_OUT_FMT_2			"DF8119"
#define		C2P_CARDINK_ISS_CONFIG_KEY		"DF811A"
#define		C2P_ENC_CSC_CHAIN				"DF811B"
#define		C2P_RSA_ENC_PIN_BLK				"DF811C"
#define		C2P_PVV_2						"DF811D"
#define		C2P_DRIVER_NBR					"DF811E"
#define		C2P_PVV_2_ALPHA					"DF811F"
#define		C2P_PIN_TYPE					"DF8120"
#define		C2P_DIVERT_COMPL_DATA			"DF8121"
#define		C2P_PVV_2_PVV					"DF8122"
#define		C2P_AUTH_CODE_PROTO				"DF8123"
#define		C2P_RES2						"DF8124"
#define		C2P_PVV_TOTAL_ALGO				"DF8125"
#define		C2P_PIN_TOTAL_ALGO				"DF8126"
#define		C2P_OFFLINE_IND					"DF8127"
#define		C2P_PIN_IDENT_DATA				"DF8128"
#define		C2P_IAD_COUNTER					"DF8129"
#define		C2P_ENC_IAD_COUNTER				"DF812A"
#define		C2P_TKN_KEY_DERIV_DATA			"DF812B"
#define		C2P_TKN_KEY_DERIV_ALGO			"DF812C"
#define		C2P_HOTP_VAL_COUNTER			"DF812D"
#define		C2P_OTP_DIGIT_NBR				"DF812E"
#define		C2P_LOOK_AHEAD_WINDOW			"DF812F"
#define		C2P_OTP_TO_VERIF				"DF8200"
#define		C2P_ENC_RANDOM_NBR				"DF8201"
#define		C2P_MAC_ALGO_ID_2				"DF8202"
#define		C2P_ACCOUNT_INFO				"DF8203"
#define		C2P_ACCOUNT_INFO_ENC			"DF8204"
#define		C2P_IVCVC3_INIT_VEC_DATA		"DF8205"
#define		C2P_IVCVC3						"DF8206"
#define		C2P_GROUP_ID					"DF8207"
#define		C2P_TOKEN_RANGE					"DF8208"
#define		C2P_X509_CERT					"DF8209"
#define		C2P_DATA_ITEM_A					"DF820A"
#define		C2P_DATA_ITEM_B					"DF820B"
#define		C2P_DATA_ITEM_C					"DF820C"
#define		C2P_DATA_ITEM_D					"DF820D"
#define		C2P_MAC_APACS_RESIDUE			"DF820E"
#define		C2P_RANDOM_NBR					"DF820F"
#define		C2P_DISTINGUISHED_NAME			"DF8210"
#define		C2P_KEY_EXP_DATE				"DF8211"
#define		C2P_KEY_ACTIVATION_DATE			"DF8212"
#define		C2P_PARITY						"DF8213"
#define		C2P_CRC_ALGO					"DF8214"
#define		C2P_KEY_PARTITY					"DF8215"
#define		C2P_PARITY_BITS					"DF8216"
#define		C2P_KEY_DESC					"DF8217"
#define		C2P_KEY_NAME					"DF8218"
#define		C2P_BRAND						"DF8219"
#define		C2P_RID							"DF821A"
#define		C2P_SID							"DF821B"
#define		C2P_PUB_KEY_END_VAL_DATE		"DF821C"
#define		C2P_CERT_ID						"DF821D"
#define		C2P_ECDSA_KEY_PARAMS			"DF821E"
#define		C2P_MK_DERIV_MTHD				"DF821F"
#define		C2P_RMAC_CALC					"DF8220"
#define		C2P_CARD_CHALLENGE				"DF8221"
#define		C2P_TERM_CHALLENGE				"DF8222"
#define		C2P_SEQ_COUNTER					"DF8223"
#define		C2P_CARD_CRYPTO					"DF8224"
#define		C2P_TERM_CRYPTO					"DF8225"
#define		C2P_CLASS						"DF8226"
#define		C2P_PKCS11_KEY_DERIV_ALGO		"DF8227"
#define		C2P_TERM_SERIAL_NO				"DF8228"
#define		C2P_HASH_ALGO					"DF8229"
#define		C2P_RANDOM_NBR_LEN				"DF822A"
#define		C2P_RANDOM_NBR_FMT				"DF822B"
#define		C2P_ECDSA_PUB_KEY				"DF822C"
#define		C2P_ECDSA_ENC_PRIV_KEY			"DF822D"
#define		C2P_TOTAL_PIN_BLK_OPS			"DF822E"
#define		C2P_PKCS11_KEY_DERIV_DATA		"DF822F"
#define		C2P_KEY_OFST					"DF8230"
#define		C2P_USER_PIN					"DF8231"
#define		C2P_AUTHEN_DATA					"DF8232"
#define		C2P_GID_LIST					"DF8233"
#define		C2P_TR31_KEY_BLOCK				"DF8234"
#define		C2P_TR31_KEY_BLOCK_VER			"DF8235"
#define		C2P_CARD_APDU					"DF8236"
#define		C2P_DERIV_DATA_COUNTER_OFST		"DF8237"
#define		C2P_RSA_KEY_INDEX				"DF8238"
#define		C2P_HASHED_MSG_LEN				"DF8239"
#define		C2P_DH_KEY_SHARED_DATA			"DF823A"
#define		C2P_CARD_ADMIN_DATA				"DF823B"
#define		C2P_PAN_PREFIX_LEN				"DF823C"
#define		C2P_KEY_TKN_MAC					"DF823D"
#define		C2P_CRC_COMPL_DATA				"DF823E"
#define		C2P_MIFARE_KEY_VER				"DF823F"
#define		C2P_KEY_DERIV_FUNCTION			"DF8240"
#define		C2P_ISO_TRACK2_TXT_FMT			"DF8241"
#define		C2P_USAGE_MODE					"DF8242"
#define		C2P_SECRET_OWNER_ROLE			"DF8243"
#define		C2P_DIVERT_DATA_LEN				"DF8244"
#define		C2P_MSG_LABEL					"DF8245"
#define		C2P_MGF_GEN_FUNC				"DF8246"
#define		C2P_PAN_ENC_ALGO				"DF8247"
#define		C2P_AUTO_TEST_TYPE				"DF8248"
#define		C2P_OP_MODE						"DF8249"
#define		C2P_SECURITY_POLICY_FLAGS		"DF824A"
#define		C2P_SESSION_KEY_DERIV_RND_NBR	"DF824C"
#define		C2P_ENC_PKC						"DF824D"
#define		C2P_ENC_AP						"DF824E"
#define		C2P_AUTO_ENC_AP					"DF824F"
#define		C2P_ABI_CODE					"DF8250"
#define		C2P_CCS							"DF8251"
#define		C2P_PKC_LEN						"DF8252"
#define		C2P_PRIM_P						"DF8253"
#define		C2P_SUBPRIM_Q					"DF8254"
#define		C2P_G_BASE						"DF8255"
#define		C2P_DSA_PUB_KEY					"DF8256"
#define		C2P_DSA_PRIM_P_LEN				"DF8257"
#define		C2P_AUTH_ENC_MODE				"DF8258"
#define		C2P_DUKPT_KEY_USAGE				"DF8259"
#define		C2P_CARD_KEYS_DIVERT_MTHD		"DF825A"
#define		C2P_SECRET_LEN					"DF825B"
#define		C2P_ENC_PUK						"DF825C"
#define		C2P_AUTHEN_MTHD					"DF825D"
#define		C2P_ACTIV_SIG_NBR				"DF825E"
#define		C2P_INIT_PWD					"DF825F"
#define		C2P_ENC_INIT_PWD				"DF8260"
#define		C2P_DYN_AUTH_DATA				"DF8261"
#define		C2P_VARIANT_NBR					"DF8262"
#define		C2P_DDC_ID						"DF8263"
#define		C2P_DERIV_KEY_LEN				"DF8264"
#define		C2P_DDC_KEY_PACK				"DF8265"
#define		C2P_ENC_COUNTER					"DF8266"
#define		C2P_ENC_DATA_OFST				"DF8267"
#define		C2P_ENC_DATA_LEN				"DF8268"
#define		C2P_EPURSE_DERIV_ALGO			"DF8269"
#define		C2P_GET_PURSE_CHALLENGE			"DF826A"
#define		C2P_SERVER_TRN_DATA				"DF826B"
#define		C2P_TOPUP_CMD_DATA				"DF826C"
#define		C2P_MOBILE_APP_ID				"DF826D"
#define		C2P_MOBILE_APP_FINGERPRINT		"DF826E"
#define		C2P_MOBILE_APP_MNG_INFO			"DF826F"
#define		C2P_AUTH_CODE					"DF8270"
#define		C2P_ENC_MAC_SESSION_ID			"DF8271"
#define		C2P_SESSION_OPT_VERSION			"DF8272"
#define		C2P_SESSION_EXPIRY_DATE			"DF8273"
#define		C2P_LIMITED_USAGE_KEY_ALGO_ID	"DF8274"
#define		C2P_CVT_ALGO					"DF8275"
#define		C2P_CVT_OATH_KEYS_DIVERT_LABEL	"DF8276"
#define		C2P_CURR_COUNTER				"DF8277"
#define		C2P_NB_NEXT_COUNTER				"DF8278"
#define		C2P_NB_PREV_COUNTER				"DF8279"
#define		C2P_TEMPORAL_DYN_CVV			"DF827A"
#define		C2P_CVT_VERIF_NB_RETRIES		"DF827B"


#define		C2P_SYM_KEY_TKN_BLOC			"E1"
#define		C2P_SYM_KEY_REC_BLOC			"FF41"
#define		C2P_SYM_KEY_COMP_BLOC			"FF40"
#define		C2P_RSA_PUB_KEY_BLOC			"EE"
#define		C2P_ECDSA_PUB_KEY_BLOC			"FF8143"
#define		C2P_DSA_KEY_PARAM_BLOC			"FF8146"
#define		C2P_DSA_PUB_KEY_BLOC			"FF8145"
#define		C2P_PRIV_KEY_BLOC				"EF"
#define		C2P_ACTIVATION_DATA_BLOC		"FF8147"
#define		C2P_CERT_TKN_BLOC				"FF8140"
#define		C2P_USER_TKN_BLOC				"FF8141"
#define		C2P_SEC_HOLDER_AUTH_TKN_BLOC	"FF8144"


#define		C2P_RETURN_CODE					"C0"
#define		C2P_COMPL_INFO					"C1"

/* below order is as per C2PFieldsDataProp */
typedef enum {

	C2P_FN_ISO_TRACK_1						=0	,
	C2P_FN_PAN									,
	C2P_FN_PAN_SEQ_NO							,
	C2P_FN_CERT									,
	C2P_FN_ARC									,
	C2P_FN_ARQC									,
	C2P_FN_ATC									,
	C2P_FN_DAC									,
	C2P_FN_IDN									,
	C2P_FN_CVC3_TRACK1_BITMAP					,
	C2P_FN_UN_ATC_TRACK1_BITMAP					,
	C2P_FN_CVC3_TRACK2_BITMAP					,
	C2P_FN_UN_ATC_TRACK2_BITMAP					,
	C2P_FN_SESSION_KEY_GEN_USED_NBR				,
	C2P_FN_AC_ALGO_ID							,
	C2P_FN_AC_GEN_DATA							,
	C2P_FN_ARPC_GEN_ALGOID						,
	C2P_FN_ARPC									,
	C2P_FN_MAC_ALGO_ID							,
	C2P_FN_MAC_DATA								,
	C2P_FN_MAC_LEN								,
	C2P_FN_MAC									,
	C2P_FN_ENC_ALGO_ID							,
	C2P_FN_DATA_TO_ENC							,
	C2P_FN_ENC_DATA								,
	C2P_FN_DAC_GEN_ALGO_ID						,
	C2P_FN_TAG_CF								,
	C2P_FN_IDN_GEN_ALGO_ID						,
	C2P_FN_IDN_GEN_DATA							,
	C2P_FN_KEY_ID								,
	C2P_FN_KEY_USAGE							,
	C2P_FN_KEY_KCV								,
	C2P_FN_KEY_KEV								,
	C2P_FN_FINGERPRINT_GEN_DATA					,
	C2P_FN_FINGERPRINT							,
	C2P_FN_RSA_KEY_MOD_LEN						,
	C2P_FN_RSA_PUB_KEY_EXT_FMT_ID				,
	C2P_FN_RSA_PRIV_KEY_FMT_ID					,
	C2P_FN_RSA_PUB_KEY_MOD						,
	C2P_FN_RSA_PUB_KEY_EXP						,
	C2P_FN_ENC_KEY								,
	C2P_FN_SIG_ALGO_ID							,
	C2P_FN_SIG_DATA								,
	C2P_FN_SIGNATURE							,
	C2P_FN_SIG_VERIF_ALGO_ID					,
	C2P_FN_PAD_ALGO_ID							,
	C2P_FN_ENC_PIN_BLK							,
	C2P_FN_PIN_BLK_FMT							,
	C2P_FN_ISO_TRACK_2							,
	C2P_FN_CVX2									,
	C2P_FN_TRACK2_CTRL_VALUE_OFST				,
	C2P_FN_AUTH_DATA							,
	C2P_FN_AUTH_CODE_LEN						,
	C2P_FN_PIN_BLK_NA_IN						,
	C2P_FN_AUTH_NO								,
	C2P_FN_PIN_BLK_NA_OUT						,
	C2P_FN_KEY_IDENT_DATA						,
	C2P_FN_KEY_IDENT_TYPE						,
	C2P_FN_TKN_KEY_ENC_ALGO_ID					,
	C2P_FN_OAEP_ENC_PARAM						,
	C2P_FN_RSA_PUB_KEY_ENC_KEY					,
	C2P_FN_FILE_TYPE							,
	C2P_FN_RECORDS_NBR							,
	C2P_FN_TRACE_MODE							,
	C2P_FN_C2P_NUMBER							,
	C2P_FN_PIN_VERIF_NEG_RESP_NBR				,
	C2P_FN_PIN_VERIF_REQ_NBR					,
	C2P_FN_CVX_VERIF_NEG_RESP_NBR				,
	C2P_FN_CVX_VERIF_REQ_NBR					,
	C2P_FN_MODULE_STATE							,
	C2P_FN_APP_ID								,
	C2P_FN_VER_NO								,
	C2P_FN_SERIAL_NO							,
	C2P_FN_ENC_KEY_ALGO_ID						,
	C2P_FN_KEY_GEN_INDICATOR					,
	C2P_FN_KEY_OUT_SUPPORT						,
	C2P_FN_KEY_DECOMP_MTHD						,
	C2P_FN_KEY_IN_SUPPORT						,
	C2P_FN_EXPIRY_DATE							,
	C2P_FN_EXPORT_ID							,
	C2P_FN_DIVER_VALUE							,
	C2P_FN_TRANS_DATA							,
	C2P_FN_DES_KEY_ENC_RSA_ALGO_ID				,
	C2P_FN_DIVER_DATA							,
	C2P_FN_EXTERNAL_VAR							,
	C2P_FN_CARD_WORD_ADDR						,
	C2P_FN_CARD_WORD							,
	C2P_FN_AUTH_ITEM							,
	C2P_FN_CAI									,
	C2P_FN_VERIF_ALGO_ID						,
	C2P_FN_CVX_GEN_ALGO							,
	C2P_FN_BLOCK_RANGE							,
	C2P_FN_KEY_COMP								,
	C2P_FN_ENC_MTHD_ID							,
	C2P_FN_KEY_DECOMP_MTHD_2					,
	C2P_FN_COMP_NBR								,
	C2P_FN_FORMAT_ID							,
	C2P_FN_KCV_TYPE								,
	C2P_FN_ANSI2_AUTH_KEY						,
	C2P_FN_AUTH_FIELD							,
	C2P_FN_IMPLICIT_ENFORCEMENT_FLAG			,
	C2P_FN_DIVER_PIN_BLK_IN						,
	C2P_FN_DIVER_PIN_BLK_OUT					,
	C2P_FN_PIN_LEN								,
	C2P_FN_PIN									,
	C2P_FN_SERIAL_NO_2							,
	C2P_FN_AUTH_TICKET							,
	C2P_FN_AUTH_TICKET_VERIF_CERT				,
	C2P_FN_KEY_DEL_FILTER						,
	C2P_FN_KEYS_NBR								,
	C2P_FN_TICKETS_COUNTER						,
	C2P_FN_TICKETS_PRIO							,
	C2P_FN_C2P_IDP								,
	C2P_FN_AUTO_TEST_RESULT_FMT					,
	C2P_FN_AVAIL_OPTIONS						,
	C2P_FN_LOADED_OPTIONS						,
	C2P_FN_SESSION_KEY_ALGO_PARAMS				,
	C2P_FN_MASTER_KEY_NBR						,
	C2P_FN_KEY_LEN								,
	C2P_FN_SERVICE_CODE							,
	C2P_FN_HMAC_GEN_DATA						,
	C2P_FN_HMAC									,
	C2P_FN_HMAC_LEN								,
	C2P_FN_USAGE_PROFILE						,
	C2P_FN_APP_STATE							,
	C2P_FN_CSC_3								,
	C2P_FN_CSC_4								,
	C2P_FN_CSC_5								,
	C2P_FN_DISTR_ACC_FLAG						,
	C2P_FN_CARD_KEY_KCV_LEN						,
	C2P_FN_CARD_KEY_KCV							,
	C2P_FN_ENC_STEP								,
	C2P_FN_ENC_DATA_IN							,
	C2P_FN_DATA_ENC_MODE						,
	C2P_FN_ENC_PADDING_ALGO_ID					,
	C2P_FN_ENC_INIT_VECTOR						,
	C2P_FN_REPORT_DATA							,
	C2P_FN_ENC_DATA_OUT							,
	C2P_FN_KEY_VALUE							,
	C2P_FN_DATA_ENC_MODE_2						,
	C2P_FN_DATA_LEN_RETREIVAL_MODE				,
	C2P_FN_RSA_ENC_ALGO_ID						,
	C2P_FN_RSA_ENC_DATA_IN						,
	C2P_FN_RSA_ENC_DATA_OUT						,
	C2P_FN_P									,
	C2P_FN_Q									,
	C2P_FN_DP									,
	C2P_FN_DQ									,
	C2P_FN_INV_Q								,
	C2P_FN_CID									,
	C2P_FN_PUB_KEY_HASH							,
	C2P_FN_PUB_KEY_FINGERPRINT_REQ				,
	C2P_FN_AUTH_RC								,
	C2P_FN_ENC_MODE								,
	C2P_FN_CARD_KEY_OUT_FMT						,
	C2P_FN_CB_KEY								,
	C2P_FN_PRINT_FMT							,
	C2P_FN_AVOID_SEQ							,
	C2P_FN_CONV_CHAIN							,
	C2P_FN_AUTH_DATA_2							,
	C2P_FN_EMV_CARD_MK_ALGO						,
	C2P_FN_CARD_SERIAL_NBR						,
	C2P_FN_CARD_TRN_COUNTER						,
	C2P_FN_CREDIT_CERT_VALUE					,
	C2P_FN_CREDIT_CERT_ENC						,
	C2P_FN_DATA_1								,
	C2P_FN_P1									,
	C2P_FN_P2									,
	C2P_FN_ENC_DATA_TYPE						,
	C2P_FN_VAR_FIELD_00							,
	C2P_FN_VAR_FIELD_01							,
	C2P_FN_VAR_FIELD_02							,
	C2P_FN_VAR_FIELD_03							,
	C2P_FN_VAR_FIELD_04							,
	C2P_FN_VAR_FIELD_05							,
	C2P_FN_VAR_FIELD_06							,
	C2P_FN_VAR_FIELD_07							,
	C2P_FN_VAR_FIELD_08							,
	C2P_FN_VAR_FIELD_09							,
	C2P_FN_VAR_FIELD_10							,
	C2P_FN_VAR_FIELD_11							,
	C2P_FN_VAR_FIELD_12							,
	C2P_FN_VAR_FIELD_13							,
	C2P_FN_VAR_FIELD_14							,
	C2P_FN_VAR_FIELD_15							,
	C2P_FN_DATA_2_DATA_3						,
	C2P_FN_CODE_VERIF_ALGO						,
	C2P_FN_VAL_DATA								,
	C2P_FN_PIN_OFST_LEN							,
	C2P_FN_CODE_GEN_ALGO						,
	C2P_FN_PIN_OFST								,
	C2P_FN_KSN									,
	C2P_FN_DEC_MODE								,
	C2P_FN_ISS_AC_BITMAP						,
	C2P_FN_CARD_KEY_OUT_FMT_2					,
	C2P_FN_CARDINK_ISS_CONFIG_KEY				,
	C2P_FN_ENC_CSC_CHAIN						,
	C2P_FN_RSA_ENC_PIN_BLK						,
	C2P_FN_PVV_2								,
	C2P_FN_DRIVER_NBR							,
	C2P_FN_PVV_2_ALPHA							,
	C2P_FN_PIN_TYPE								,
	C2P_FN_DIVERT_COMPL_DATA					,
	C2P_FN_PVV_2_PVV							,
	C2P_FN_AUTH_CODE_PROTO						,
	C2P_FN_RES2									,
	C2P_FN_PVV_TOTAL_ALGO						,
	C2P_FN_PIN_TOTAL_ALGO						,
	C2P_FN_OFFLINE_IND							,
	C2P_FN_PIN_IDENT_DATA						,
	C2P_FN_IAD_COUNTER							,
	C2P_FN_ENC_IAD_COUNTER						,
	C2P_FN_TKN_KEY_DERIV_DATA					,
	C2P_FN_TKN_KEY_DERIV_ALGO					,
	C2P_FN_HOTP_VAL_COUNTER						,
	C2P_FN_OTP_DIGIT_NBR						,
	C2P_FN_LOOK_AHEAD_WINDOW					,
	C2P_FN_OTP_TO_VERIF							,
	C2P_FN_ENC_RANDOM_NBR						,
	C2P_FN_MAC_ALGO_ID_2						,
	C2P_FN_ACCOUNT_INFO							,
	C2P_FN_ACCOUNT_INFO_ENC						,
	C2P_FN_IVCVC3_INIT_VEC_DATA					,
	C2P_FN_IVCVC3								,
	C2P_FN_GROUP_ID								,
	C2P_FN_TOKEN_RANGE							,
	C2P_FN_X509_CERT							,
	C2P_FN_DATA_ITEM_A							,
	C2P_FN_DATA_ITEM_B							,
	C2P_FN_DATA_ITEM_C							,
	C2P_FN_DATA_ITEM_D							,
	C2P_FN_MAC_APACS_RESIDUE					,
	C2P_FN_RANDOM_NBR							,
	C2P_FN_DISTINGUISHED_NAME					,
	C2P_FN_KEY_EXP_DATE							,
	C2P_FN_KEY_ACTIVATION_DATE					,
	C2P_FN_PARITY								,
	C2P_FN_CRC_ALGO								,
	C2P_FN_KEY_PARTITY							,
	C2P_FN_PARITY_BITS							,
	C2P_FN_KEY_DESC								,
	C2P_FN_KEY_NAME								,
	C2P_FN_BRAND								,
	C2P_FN_RID									,
	C2P_FN_SID									,
	C2P_FN_PUB_KEY_END_VAL_DATE					,
	C2P_FN_CERT_ID								,
	C2P_FN_ECDSA_KEY_PARAMS						,
	C2P_FN_MK_DERIV_MTHD						,
	C2P_FN_RMAC_CALC							,
	C2P_FN_CARD_CHALLENGE						,
	C2P_FN_TERM_CHALLENGE						,
	C2P_FN_SEQ_COUNTER							,
	C2P_FN_CARD_CRYPTO							,
	C2P_FN_TERM_CRYPTO							,
	C2P_FN_CLASS								,
	C2P_FN_PKCS11_KEY_DERIV_ALGO				,
	C2P_FN_TERM_SERIAL_NO						,
	C2P_FN_HASH_ALGO							,
	C2P_FN_RANDOM_NBR_LEN						,
	C2P_FN_RANDOM_NBR_FMT						,
	C2P_FN_ECDSA_PUB_KEY						,
	C2P_FN_ECDSA_ENC_PRIV_KEY					,
	C2P_FN_TOTAL_PIN_BLK_OPS					,
	C2P_FN_PKCS11_KEY_DERIV_DATA				,
	C2P_FN_KEY_OFST								,
	C2P_FN_USER_PIN								,
	C2P_FN_AUTHEN_DATA							,
	C2P_FN_GID_LIST								,
	C2P_FN_TR31_KEY_BLOCK						,
	C2P_FN_TR31_KEY_BLOCK_VER					,
	C2P_FN_CARD_APDU							,
	C2P_FN_DERIV_DATA_COUNTER_OFST				,
	C2P_FN_RSA_KEY_INDEX						,
	C2P_FN_HASHED_MSG_LEN						,
	C2P_FN_DH_KEY_SHARED_DATA					,
	C2P_FN_CARD_ADMIN_DATA						,
	C2P_FN_PAN_PREFIX_LEN						,
	C2P_FN_KEY_TKN_MAC							,
	C2P_FN_CRC_COMPL_DATA						,
	C2P_FN_MIFARE_KEY_VER						,
	C2P_FN_KEY_DERIV_FUNCTION					,
	C2P_FN_ISO_TRACK2_TXT_FMT					,
	C2P_FN_USAGE_MODE							,
	C2P_FN_SECRET_OWNER_ROLE					,
	C2P_FN_DIVERT_DATA_LEN						,
	C2P_FN_MSG_LABEL							,
	C2P_FN_MGF_GEN_FUNC							,
	C2P_FN_PAN_ENC_ALGO							,
	C2P_FN_AUTO_TEST_TYPE						,
	C2P_FN_OP_MODE								,
	C2P_FN_SECURITY_POLICY_FLAGS				,
	C2P_FN_SESSION_KEY_DERIV_RND_NBR			,
	C2P_FN_ENC_PKC								,
	C2P_FN_ENC_AP								,
	C2P_FN_AUTO_ENC_AP							,
	C2P_FN_ABI_CODE								,
	C2P_FN_CCS									,
	C2P_FN_PKC_LEN								,
	C2P_FN_PRIM_P								,
	C2P_FN_SUBPRIM_Q							,
	C2P_FN_G_BASE								,
	C2P_FN_DSA_PUB_KEY							,
	C2P_FN_DSA_PRIM_P_LEN						,
	C2P_FN_AUTH_ENC_MODE						,
	C2P_FN_DUKPT_KEY_USAGE						,
	C2P_FN_CARD_KEYS_DIVERT_MTHD				,
	C2P_FN_SECRET_LEN							,
	C2P_FN_ENC_PUK								,
	C2P_FN_AUTHEN_MTHD							,
	C2P_FN_ACTIV_SIG_NBR						,
	C2P_FN_INIT_PWD								,
	C2P_FN_ENC_INIT_PWD							,
	C2P_FN_DYN_AUTH_DATA						,
	C2P_FN_VARIANT_NBR							,
	C2P_FN_DDC_ID								,
	C2P_FN_DERIV_KEY_LEN						,
	C2P_FN_DDC_KEY_PACK							,
	C2P_FN_ENC_COUNTER							,
	C2P_FN_ENC_DATA_OFST						,
	C2P_FN_ENC_DATA_LEN							,
	C2P_FN_EPURSE_DERIV_ALGO					,
	C2P_FN_GET_PURSE_CHALLENGE					,
	C2P_FN_SERVER_TRN_DATA						,
	C2P_FN_TOPUP_CMD_DATA						,
	C2P_FN_MOBILE_APP_ID						,
	C2P_FN_MOBILE_APP_FINGERPRINT				,
	C2P_FN_MOBILE_APP_MNG_INFO					,
	C2P_FN_AUTH_CODE							,
	C2P_FN_ENC_MAC_SESSION_ID					,
	C2P_FN_SESSION_OPT_VERSION					,
	C2P_FN_SESSION_EXPIRY_DATE					,
	C2P_FN_LIMITED_USAGE_KEY_ALGO_ID			,
	C2P_FN_CVT_ALGO								,
	C2P_FN_CVT_OATH_KEYS_DIVERT_LABEL			,
	C2P_FN_CURR_COUNTER							,
	C2P_FN_NB_NEXT_COUNTER						,
	C2P_FN_NB_PREV_COUNTER						,
	C2P_FN_TEMPORAL_DYN_CVV						,
	C2P_FN_CVT_VERIF_NB_RETRIES					,


	C2P_FN_SYM_KEY_TKN_BLOC							,
	C2P_FN_SYM_KEY_REC_BLOC							,
	C2P_FN_SYM_KEY_COMP_BLOC						,
	C2P_FN_RSA_PUB_KEY_BLOC							,
	C2P_FN_ECDSA_PUB_KEY_BLOC						,
	C2P_FN_DSA_KEY_PARAM_BLOC						,
	C2P_FN_DSA_PUB_KEY_BLOC							,
	C2P_FN_PRIV_KEY_BLOC							,
	C2P_FN_ACTIVATION_DATA_BLOC						,
	C2P_FN_CERT_TKN_BLOC							,
	C2P_FN_USER_TKN_BLOC							,
	C2P_FN_SEC_HOLDER_AUTH_TKN_BLOC					,

	C2P_FN_RETURN_CODE								,
	C2P_FN_COMPL_INFO								,

	C2P_FN_QTY
} E_C2P_FIELD_ID;

typedef enum {

	C2P_FN_CMD_REQ_VERIFY_AC_E2					= 0	,
	C2P_FN_CMD_REQ_GEN_ARPC_E3						,
	C2P_FN_CMD_REQ_GEN_MAC_E4						,
	C2P_FN_CMD_REQ_ENC_DATA_E5						,
	C2P_FN_CMD_REQ_VERIFY_DAC_E6					,
	C2P_FN_CMD_REQ_VERIFY_IDN_E7					,
	C2P_FN_CMD_REQ_PIN_CHG_E8						,
	C2P_FN_CMD_REQ_GEN_KEY_E9						,
	C2P_FN_CMD_REQ_SAVE_KEY_EA						,
	C2P_FN_CMD_REQ_IMPORT_KEY_EB					,
	C2P_FN_CMD_REQ_DEL_KEY_EC						,
	C2P_FN_CMD_REQ_VERIFY_KEY_ED					,
	C2P_FN_CMD_REQ_GEN_RSA_F0						,
	C2P_FN_CMD_REQ_EXT_PUBLIC_KEY_F1						,
	C2P_FN_CMD_REQ_AUTO_TEST_F2						,
	C2P_FN_CMD_REQ_STAT_REPORT_F3					,
	C2P_FN_CMD_REQ_RFU_F4_F4						,
	C2P_FN_CMD_REQ_GEN_SIG_F5						,
	C2P_FN_CMD_REQ_VERIFY_SIG_F6					,
	C2P_FN_CMD_REQ_GEN_FINGERPRINT_SHA_1_F7			,
	C2P_FN_CMD_REQ_GEN_HASH_F8						,
	C2P_FN_CMD_REQ_VERIFY_HASH_F9					,
	C2P_FN_CMD_REQ_GEN_AUTH_NO_FA					,
	C2P_FN_CMD_REQ_PIN_TRSLT_FB						,
	C2P_FN_CMD_REQ_VERIFY_PIN_FC					,
	C2P_FN_CMD_REQ_VERIFY_CVX_FD					,
	C2P_FN_CMD_REQ_VERIFY_CVX2_FE					,
	C2P_FN_CMD_REQ_EXPORT_KEY_FF20					,
	C2P_FN_CMD_REQ_RFU_FF21_FF21					,
	C2P_FN_CMD_REQ_RFU_FF22_FF22					,
	C2P_FN_CMD_REQ_RFU_FF23_FF23					,
	C2P_FN_CMD_REQ_RFU_FF24_FF24					,
	C2P_FN_CMD_REQ_RFU_FF25_FF25					,
	C2P_FN_CMD_REQ_RFU_FF26_FF26					,
	C2P_FN_CMD_REQ_RFU_FF27_FF27					,
	C2P_FN_CMD_REQ_RFU_FF28_FF28					,
	C2P_FN_CMD_REQ_GEN_PVV_FF29						,
	C2P_FN_CMD_REQ_GEN_CVX_FF2A						,
	C2P_FN_CMD_REQ_GEN_CVX2_FF2B					,
	C2P_FN_CMD_REQ_RFU_FF2C_FF2C					,
	C2P_FN_CMD_REQ_BUILD_AUTH_TICKET_FF2D			,
	C2P_FN_CMD_REQ_VERIFY_AUTH_TICKET_FF2E			,
	C2P_FN_CMD_REQ_GEN_SPA_HMAC_FF2F				,
	C2P_FN_CMD_REQ_VERIFY_SPA_HMAC_FF42				,
	C2P_FN_CMD_REQ_VERIFY_CSC_FF43					,
	C2P_FN_CMD_REQ_GEN_CSC_FF44						,
	C2P_FN_CMD_REQ_GEN_CARD_KEY_KCV_FF45			,
	C2P_FN_CMD_REQ_RFU_FF46_FF46					,
	C2P_FN_CMD_REQ_RFU_FF47_FF47					,
	C2P_FN_CMD_REQ_RFU_FF48_FF48					,
	C2P_FN_CMD_REQ_RFU_FF49_FF49					,
	C2P_FN_CMD_REQ_RFU_FF4A_FF4A					,
	C2P_FN_CMD_REQ_RFU_FF4B_FF4B					,
	C2P_FN_CMD_REQ_ENCRYPT_RSA_FF4C					,
	C2P_FN_CMD_REQ_RFU_FF4D_FF4D					,
	C2P_FN_CMD_REQ_RFU_FF4E_FF4E					,
	C2P_FN_CMD_REQ_RFU_FF4F_FF4F					,
	C2P_FN_CMD_REQ_RFU_FF60_FF60					,
	C2P_FN_CMD_REQ_RFU_FF61_FF61					,
	C2P_FN_CMD_REQ_RFU_FF62_FF62					,
	C2P_FN_CMD_REQ_RFU_FF63_FF63					,
	C2P_FN_CMD_REQ_RFU_FF64_FF64					,
	C2P_FN_CMD_REQ_RFU_FF65_FF65					,
	C2P_FN_CMD_REQ_RFU_FF66_FF66					,
	C2P_FN_CMD_REQ_RFU_FF67_FF67					,
	C2P_FN_CMD_REQ_RFU_FF68_FF68					,
	C2P_FN_CMD_REQ_RFU_FF69_FF69					,
	C2P_FN_CMD_REQ_RFU_FF6A_FF6A					,
	C2P_FN_CMD_REQ_DECRYPT_ASYM_KEY_FF6B					,
	C2P_FN_CMD_REQ_RFU_FF6C_FF6C					,
	C2P_FN_CMD_REQ_RFU_FF6D_FF6D					,
	C2P_FN_CMD_REQ_RFU_FF6E_FF6E					,
	C2P_FN_CMD_REQ_RFU_FF6F_FF6F					,
	C2P_FN_CMD_REQ_RFU_FF8100_FF8100				,
	C2P_FN_CMD_REQ_RFU_FF8101_FF8101				,
	C2P_FN_CMD_REQ_RFU_FF8102_FF8102				,
	C2P_FN_CMD_REQ_RFU_FF8103_FF8103				,
	C2P_FN_CMD_REQ_RFU_FF8104_FF8104				,
	C2P_FN_CMD_REQ_RFU_FF8105_FF8105				,
	C2P_FN_CMD_REQ_RFU_FF8106_FF8106				,
	C2P_FN_CMD_REQ_RFU_FF8107_FF8107				,
	C2P_FN_CMD_REQ_VERIFY_CVC3_FF8108				,
	C2P_FN_CMD_REQ_GEN_IV_CVC3_FF8109				,
	C2P_FN_CMD_REQ_RFU_FF810A_FF810A				,
	C2P_FN_CMD_REQ_RFU_FF810B_FF810B				,
	C2P_FN_CMD_REQ_RFU_FF810C_FF810C				,
	C2P_FN_CMD_REQ_RFU_FF810D_FF810D				,
	C2P_FN_CMD_REQ_RFU_FF810E_FF810E				,
	C2P_FN_CMD_REQ_RFU_FF810F_FF810F				,
	C2P_FN_CMD_REQ_RFU_FF8120_FF8120				,
	C2P_FN_CMD_REQ_RFU_FF8121_FF8121				,
	C2P_FN_CMD_REQ_RFU_FF8122_FF8122				,
	C2P_FN_CMD_REQ_IMP_SEC_KEY_RSA_FF8123				,
	C2P_FN_CMD_REQ_EXP_SEC_KEY_RSA_FF8124				,
	C2P_FN_CMD_REQ_RFU_FF8125_FF8125				,
	C2P_FN_CMD_REQ_RFU_FF8126_FF8126				,
	C2P_FN_CMD_REQ_RFU_FF8127_FF8127				,
	C2P_FN_CMD_REQ_RFU_FF8128_FF8128				,
	C2P_FN_CMD_REQ_RFU_FF8129_FF8129				,
	C2P_FN_CMD_REQ_RFU_FF812A_FF812A				,
	C2P_FN_CMD_REQ_RFU_FF812B_FF812B				,
	C2P_FN_CMD_REQ_RFU_FF812C_FF812C				,
	C2P_FN_CMD_REQ_RFU_FF812D_FF812D				,
	C2P_FN_CMD_REQ_RFU_FF812E_FF812E				,
	C2P_FN_CMD_REQ_RFU_FF812F_FF812F				,
	C2P_FN_CMD_REQ_RFU_FF8160_FF8160				,
	C2P_FN_CMD_REQ_RFU_FF8161_FF8161				,
	C2P_FN_CMD_REQ_RFU_FF8162_FF8162				,
	C2P_FN_CMD_REQ_RFU_FF8163_FF8163				,
	C2P_FN_CMD_REQ_RFU_FF8164_FF8164				,
	C2P_FN_CMD_REQ_RFU_FF8165_FF8165				,
	C2P_FN_CMD_REQ_RFU_FF8166_FF8166				,
	C2P_FN_CMD_REQ_RFU_FF8167_FF8167				,
	C2P_FN_CMD_REQ_RFU_FF8168_FF8168				,
	C2P_FN_CMD_REQ_RFU_FF8169_FF8169				,
	C2P_FN_CMD_REQ_RFU_FF816A_FF816A				,
	C2P_FN_CMD_REQ_RFU_FF816B_FF816B				,
	C2P_FN_CMD_REQ_RFU_FF816C_FF816C				,
	C2P_FN_CMD_REQ_RFU_FF816D_FF816D				,
	C2P_FN_CMD_REQ_RFU_FF816E_FF816E				,
	C2P_FN_CMD_REQ_RFU_FF816F_FF816F				,
	C2P_FN_CMD_REQ_RFU_FF8200_FF8200				,
	C2P_FN_CMD_REQ_DECRYPT_RSA_FF8201				,
	C2P_FN_CMD_REQ_RFU_FF8202_FF8202				,
	C2P_FN_CMD_REQ_RFU_FF8203_FF8203				,
	C2P_FN_CMD_REQ_RFU_FF8204_FF8204				,
	C2P_FN_CMD_REQ_RFU_FF8205_FF8205				,
	C2P_FN_CMD_REQ_RFU_FF8206_FF8206				,
	C2P_FN_CMD_REQ_RFU_FF8207_FF8207				,
	C2P_FN_CMD_REQ_RFU_FF8208_FF8208				,
	C2P_FN_CMD_REQ_RFU_FF8209_FF8209				,
	C2P_FN_CMD_REQ_RFU_FF820A_FF820A				,
	C2P_FN_CMD_REQ_RFU_FF820B_FF820B				,
	C2P_FN_CMD_REQ_RFU_FF820C_FF820C				,
	C2P_FN_CMD_REQ_RFU_FF820D_FF820D				,
	C2P_FN_CMD_REQ_RFU_FF820E_FF820E				,
	C2P_FN_CMD_REQ_RFU_FF820F_FF820F				,
	C2P_FN_CMD_REQ_RFU_FF8220_FF8220				,
	C2P_FN_CMD_REQ_RFU_FF8221_FF8221				,
	C2P_FN_CMD_REQ_RFU_FF8222_FF8222				,
	C2P_FN_CMD_REQ_RFU_FF8223_FF8223				,
	C2P_FN_CMD_REQ_RFU_FF8224_FF8224				,
	C2P_FN_CMD_REQ_RFU_FF8225_FF8225				,
	C2P_FN_CMD_REQ_RFU_FF8226_FF8226				,
	C2P_FN_CMD_REQ_RFU_FF8227_FF8227				,
	C2P_FN_CMD_REQ_RFU_FF8228_FF8228				,
	C2P_FN_CMD_REQ_RFU_FF8229_FF8229				,
	C2P_FN_CMD_RESP_UNKOWN_FF01						,
	C2P_FN_CMD_RESP_VERIFY_AC_FF02					,
	C2P_FN_CMD_RESP_GEN_ARPC_FF03					,
	C2P_FN_CMD_RESP_GEN_MAC_FF04					,
	C2P_FN_CMD_RESP_ENC_DATA_FF05					,
	C2P_FN_CMD_RESP_VERIFY_DAC_FF06					,
	C2P_FN_CMD_RESP_VERIFY_IDN_FF07					,
	C2P_FN_CMD_RESP_PIN_CHG_FF08					,
	C2P_FN_CMD_RESP_GEN_KEY_FF09					,
	C2P_FN_CMD_RESP_SAVE_KEY_FF0A					,
	C2P_FN_CMD_RESP_IMPORT_KEY_FF0B					,
	C2P_FN_CMD_RESP_DEL_KEY_FF0C					,
	C2P_FN_CMD_RESP_VERIFY_KEY_FF0D					,
	C2P_FN_CMD_RESP_GEN_RSA_FF10					,
	C2P_FN_CMD_RESP_EXT_PUBLIC_KEY_FF11					,
	C2P_FN_CMD_RESP_AUTO_TEST_FF12					,
	C2P_FN_CMD_RESP_STAT_REPORT_FF13				,
	C2P_FN_CMD_RESP_RFU_FF14_FF14					,
	C2P_FN_CMD_RESP_GEN_SIG_FF15					,
	C2P_FN_CMD_RESP_VERIFY_SIG_FF16					,
	C2P_FN_CMD_RESP_GEN_FINGERPRINT_SHA_1_FF17		,
	C2P_FN_CMD_RESP_GEN_HASH_FF18					,
	C2P_FN_CMD_RESP_VERIFY_HASH_FF19				,
	C2P_FN_CMD_RESP_GEN_AUTH_NO_FF1A				,
	C2P_FN_CMD_RESP_PIN_TRSLT_FF1B					,
	C2P_FN_CMD_RESP_VERIFY_PIN_FF1C					,
	C2P_FN_CMD_RESP_VERIFY_CVX_FF1D					,
	C2P_FN_CMD_RESP_VERIFY_CVX2_FF1E				,
	C2P_FN_CMD_RESP_EXPORT_KEY_FF30					,
	C2P_FN_CMD_RESP_RFU_FF31_FF31					,
	C2P_FN_CMD_RESP_RFU_FF32_FF32					,
	C2P_FN_CMD_RESP_RFU_FF33_FF33					,
	C2P_FN_CMD_RESP_RFU_FF34_FF34					,
	C2P_FN_CMD_RESP_RFU_FF35_FF35					,
	C2P_FN_CMD_RESP_RFU_FF36_FF36					,
	C2P_FN_CMD_RESP_RFU_FF37_FF37					,
	C2P_FN_CMD_RESP_RFU_FF38_FF38					,
	C2P_FN_CMD_RESP_GEN_PVV_FF39					,
	C2P_FN_CMD_RESP_GEN_CVX_FF3A					,
	C2P_FN_CMD_RESP_GEN_CVX2_FF3B					,
	C2P_FN_CMD_RESP_RFU_FF3C_FF3C					,
	C2P_FN_CMD_RESP_BUILD_AUTH_TICKET_FF3D			,
	C2P_FN_CMD_RESP_VERIFY_AUTH_TICKET_FF3E			,
	C2P_FN_CMD_RESP_GEN_SPA_HMAC_FF3F				,
	C2P_FN_CMD_RESP_VERIFY_SPA_HMAC_FF52			,
	C2P_FN_CMD_RESP_VERIFY_CSC_FF53					,
	C2P_FN_CMD_RESP_GEN_CSC_FF54					,
	C2P_FN_CMD_RESP_GEN_CARD_KEY_KCV_FF55			,
	C2P_FN_CMD_RESP_RFU_FF56_FF56					,
	C2P_FN_CMD_RESP_RFU_FF57_FF57					,
	C2P_FN_CMD_RESP_RFU_FF58_FF58					,
	C2P_FN_CMD_RESP_RFU_FF59_FF59					,
	C2P_FN_CMD_RESP_RFU_FF5A_FF5A					,
	C2P_FN_CMD_RESP_RFU_FF5B_FF5B					,
	C2P_FN_CMD_RESP_ENCRYPT_RSA_FF5C					,
	C2P_FN_CMD_RESP_RFU_FF5D_FF5D					,
	C2P_FN_CMD_RESP_RFU_FF5E_FF5E					,
	C2P_FN_CMD_RESP_RFU_FF5F_FF5F					,
	C2P_FN_CMD_RESP_RFU_FF70_FF70					,
	C2P_FN_CMD_RESP_RFU_FF71_FF71					,
	C2P_FN_CMD_RESP_RFU_FF72_FF72					,
	C2P_FN_CMD_RESP_RFU_FF73_FF73					,
	C2P_FN_CMD_RESP_RFU_FF74_FF74					,
	C2P_FN_CMD_RESP_RFU_FF75_FF75					,
	C2P_FN_CMD_RESP_RFU_FF76_FF76					,
	C2P_FN_CMD_RESP_RFU_FF77_FF77					,
	C2P_FN_CMD_RESP_RFU_FF78_FF78					,
	C2P_FN_CMD_RESP_RFU_FF79_FF79					,
	C2P_FN_CMD_RESP_RFU_FF7A_FF7A					,
	C2P_FN_CMD_RESP_DECRYPT_ASYM_KEY_FF7B					,
	C2P_FN_CMD_RESP_RFU_FF7C_FF7C					,
	C2P_FN_CMD_RESP_RFU_FF7D_FF7D					,
	C2P_FN_CMD_RESP_RFU_FF7E_FF7E					,
	C2P_FN_CMD_RESP_RFU_FF7F_FF7F					,
	C2P_FN_CMD_RESP_RFU_FF8110_FF8110				,
	C2P_FN_CMD_RESP_RFU_FF8111_FF8111				,
	C2P_FN_CMD_RESP_RFU_FF8112_FF8112				,
	C2P_FN_CMD_RESP_RFU_FF8113_FF8113				,
	C2P_FN_CMD_RESP_RFU_FF8114_FF8114				,
	C2P_FN_CMD_RESP_RFU_FF8115_FF8115				,
	C2P_FN_CMD_RESP_RFU_FF8116_FF8116				,
	C2P_FN_CMD_RESP_RFU_FF8117_FF8117				,
	C2P_FN_CMD_RESP_VERIFY_CVC3_FF8118				,
	C2P_FN_CMD_RESP_GEN_IV_CVC3_FF8119				,
	C2P_FN_CMD_RESP_RFU_FF811A_FF811A				,
	C2P_FN_CMD_RESP_RFU_FF811B_FF811B				,
	C2P_FN_CMD_RESP_RFU_FF811C_FF811C				,
	C2P_FN_CMD_RESP_RFU_FF811D_FF811D				,
	C2P_FN_CMD_RESP_RFU_FF811E_FF811E				,
	C2P_FN_CMD_RESP_RFU_FF811F_FF811F				,
	C2P_FN_CMD_RESP_RFU_FF8130_FF8130				,
	C2P_FN_CMD_RESP_RFU_FF8131_FF8131				,
	C2P_FN_CMD_RESP_RFU_FF8132_FF8132				,
	C2P_FN_CMD_RESP_IMP_SEC_KEY_RSA_FF8133				,
	C2P_FN_CMD_RESP_EXP_SEC_KEY_RSA_FF8134				,
	C2P_FN_CMD_RESP_RFU_FF8135_FF8135				,
	C2P_FN_CMD_RESP_RFU_FF8136_FF8136				,
	C2P_FN_CMD_RESP_RFU_FF8137_FF8137				,
	C2P_FN_CMD_RESP_RFU_FF8138_FF8138				,
	C2P_FN_CMD_RESP_RFU_FF8139_FF8139				,
	C2P_FN_CMD_RESP_RFU_FF813A_FF813A				,
	C2P_FN_CMD_RESP_RFU_FF813B_FF813B				,
	C2P_FN_CMD_RESP_RFU_FF813C_FF813C				,
	C2P_FN_CMD_RESP_RFU_FF813D_FF813D				,
	C2P_FN_CMD_RESP_RFU_FF813E_FF813E				,
	C2P_FN_CMD_RESP_RFU_FF813F_FF813F				,
	C2P_FN_CMD_RESP_RFU_FF8170_FF8170				,
	C2P_FN_CMD_RESP_RFU_FF8171_FF8171				,
	C2P_FN_CMD_RESP_RFU_FF8172_FF8172				,
	C2P_FN_CMD_RESP_RFU_FF8173_FF8173				,
	C2P_FN_CMD_RESP_RFU_FF8174_FF8174				,
	C2P_FN_CMD_RESP_RFU_FF8175_FF8175				,
	C2P_FN_CMD_RESP_RFU_FF8176_FF8176				,
	C2P_FN_CMD_RESP_RFU_FF8177_FF8177				,
	C2P_FN_CMD_RESP_RFU_FF8178_FF8178				,
	C2P_FN_CMD_RESP_RFU_FF8179_FF8179				,
	C2P_FN_CMD_RESP_RFU_FF817A_FF817A				,
	C2P_FN_CMD_RESP_RFU_FF817B_FF817B				,
	C2P_FN_CMD_RESP_RFU_FF817C_FF817C				,
	C2P_FN_CMD_RESP_RFU_FF817D_FF817D				,
	C2P_FN_CMD_RESP_RFU_FF817E_FF817E				,
	C2P_FN_CMD_RESP_RFU_FF817F_FF817F				,
	C2P_FN_CMD_RESP_RFU_FF8210_FF8210				,
	C2P_FN_CMD_RESP_DECRYPT_RSA_FF8211				,
	C2P_FN_CMD_RESP_RFU_FF8212_FF8212				,
	C2P_FN_CMD_RESP_RFU_FF8213_FF8213				,
	C2P_FN_CMD_RESP_RFU_FF8214_FF8214				,
	C2P_FN_CMD_RESP_RFU_FF8215_FF8215				,
	C2P_FN_CMD_RESP_RFU_FF8216_FF8216				,
	C2P_FN_CMD_RESP_RFU_FF8217_FF8217				,
	C2P_FN_CMD_RESP_RFU_FF8218_FF8218				,
	C2P_FN_CMD_RESP_RFU_FF8219_FF8219				,
	C2P_FN_CMD_RESP_RFU_FF821A_FF821A				,
	C2P_FN_CMD_RESP_RFU_FF821B_FF821B				,
	C2P_FN_CMD_RESP_RFU_FF821C_FF821C				,
	C2P_FN_CMD_RESP_RFU_FF821D_FF821D				,
	C2P_FN_CMD_RESP_RFU_FF821E_FF821E				,
	C2P_FN_CMD_RESP_RFU_FF821F_FF821F				,
	C2P_FN_CMD_RESP_RFU_FF8230_FF8230				,
	C2P_FN_CMD_RESP_RFU_FF8231_FF8231				,
	C2P_FN_CMD_RESP_RFU_FF8232_FF8232				,
	C2P_FN_CMD_RESP_RFU_FF8233_FF8233				,
	C2P_FN_CMD_RESP_RFU_FF8234_FF8234				,
	C2P_FN_CMD_RESP_RFU_FF8235_FF8235				,
	C2P_FN_CMD_RESP_RFU_FF8236_FF8236				,
	C2P_FN_CMD_RESP_RFU_FF8237_FF8237				,
	C2P_FN_CMD_RESP_RFU_FF8238_FF8238				,
	C2P_FN_CMD_RESP_RFU_FF8239_FF8239				,

	C2P_CMD_CODE_QTY

} E_C2P_CMD_CODE;

#endif
