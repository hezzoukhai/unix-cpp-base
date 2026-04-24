#include <logigab.h>



/*
static const BerInfoProperties  gs_LGCryptoFields = {
LG_CRPT_FLD_QTY,
{
{LG_CRPT_KEY_TYPE, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_KEY_TYPE","NULL",NOT_REPEATABLE},
{LG_CRPT_KEY_SERIAL, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,12,PP_PRINT,"NULL","LG_CRPT_KEY_SERIAL","NULL",NOT_REPEATABLE},
{LG_CRPT_KEY_VERSION, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_KEY_VERSION","NULL",NOT_REPEATABLE},
{LG_CRPT_KEY_METHOD, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_KEY_METHOD","NULL",NOT_REPEATABLE},
{LG_CRPT_RSA_OUT, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_RSA_OUT","NULL",NOT_REPEATABLE},
{LG_CRPT_MODULO, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_MODULO","NULL",NOT_REPEATABLE},
{LG_CRPT_EXPONENT, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,64,PP_PRINT,"NULL","LG_CRPT_EXPONENT","NULL",NOT_REPEATABLE},
{LG_CRPT_MANUFAC, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","LG_CRPT_MANUFAC","NULL",NOT_REPEATABLE},
{LG_CRPT_SERIAL, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,9,PP_PRINT,"NULL","LG_CRPT_SERIAL","NULL",NOT_REPEATABLE},
{LG_CRPT_RAND, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","LG_CRPT_RAND","NULL",NOT_REPEATABLE},
{LG_CRPT_SERVER_SIG, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_SERVER_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KD_SIG, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_KD_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KG_SIG, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_KG_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KTK_SIG, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_KTK_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KT_KEY, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,16,PP_PRINT,"NULL","LG_CRPT_KT_KEY","NULL",NOT_REPEATABLE},
{LG_CRPT_KSC_KEY, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,16,PP_PRINT,"NULL","LG_CRPT_KSC_KEY","NULL",NOT_REPEATABLE},
{LG_CRPT_MAC, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,8,PP_PRINT,"NULL","LG_CRPT_MAC","NULL",NOT_REPEATABLE},
{LG_CRPT_KT_VERIF_DATA, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","LG_CRPT_KT_VERIF_DATA","NULL",NOT_REPEATABLE},
{LG_CRPT_KSC_VERIF_DATA, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","LG_CRPT_KSC_VERIF_DATA","NULL",NOT_REPEATABLE},
{LG_CRPT_PKCS_REF, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,13,PP_PRINT,"NULL","LG_CRPT_PKCS_REF","NULL",NOT_REPEATABLE},
{LG_CRPT_HASH_ALGO, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_HASH_ALGO","NULL",NOT_REPEATABLE},
{LG_CRPT_ENC_ALGO, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_ENC_ALGO","NULL",NOT_REPEATABLE},
{LG_CRPT_SIG_ALGO, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_SIG_ALGO","NULL",NOT_REPEATABLE},
{LG_CRPT_MASK, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_MASK","NULL",NOT_REPEATABLE},
{LG_CRPT_P_PARAM_IND, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_P_PARAM_IND","NULL",NOT_REPEATABLE},
{LG_CRPT_SALT_PARAM_IND, FT_SINGLE,FF_BIN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_SALT_PARAM_IND","NULL",NOT_REPEATABLE},
{LG_CRPT_P, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_P","NULL",NOT_REPEATABLE},
{LG_CRPT_SALT, FT_SINGLE,FF_BIN,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_SALT","NULL",NOT_REPEATABLE},

},
"gs_LGCryptoFields"
};*/

static const BerInfoProperties  gs_LGCryptoFields = {
LG_CRPT_FLD_QTY,
{
{LG_CRPT_KEY_TYPE, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_KEY_TYPE","NULL",NOT_REPEATABLE},
{LG_CRPT_KEY_SERIAL, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,12,PP_PRINT,"NULL","LG_CRPT_KEY_SERIAL","NULL",NOT_REPEATABLE},
{LG_CRPT_KEY_VERSION, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_KEY_VERSION","NULL",NOT_REPEATABLE},
{LG_CRPT_KEY_METHOD, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_KEY_METHOD","NULL",NOT_REPEATABLE},
{LG_CRPT_RSA_OUT, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_RSA_OUT","NULL",NOT_REPEATABLE},
{LG_CRPT_MODULO, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_MODULO","NULL",NOT_REPEATABLE},
{LG_CRPT_EXPONENT, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,64,PP_PRINT,"NULL","LG_CRPT_EXPONENT","NULL",NOT_REPEATABLE},
{LG_CRPT_MANUFAC, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","LG_CRPT_MANUFAC","NULL",NOT_REPEATABLE},
{LG_CRPT_SERIAL, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,9,PP_PRINT,"NULL","LG_CRPT_SERIAL","NULL",NOT_REPEATABLE},
{LG_CRPT_RAND, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","LG_CRPT_RAND","NULL",NOT_REPEATABLE},
{LG_CRPT_SERVER_SIG, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_SERVER_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KD_SIG, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_KD_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KG_SIG, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_KG_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KTK_SIG, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_KTK_SIG","NULL",NOT_REPEATABLE},
{LG_CRPT_KT_KEY, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,16,PP_PRINT,"NULL","LG_CRPT_KT_KEY","NULL",NOT_REPEATABLE},
{LG_CRPT_KSC_KEY, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,16,PP_PRINT,"NULL","LG_CRPT_KSC_KEY","NULL",NOT_REPEATABLE},
{LG_CRPT_MAC, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,8,PP_PRINT,"NULL","LG_CRPT_MAC","NULL",NOT_REPEATABLE},
{LG_CRPT_KT_VERIF_DATA, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","LG_CRPT_KT_VERIF_DATA","NULL",NOT_REPEATABLE},
{LG_CRPT_KSC_VERIF_DATA, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","LG_CRPT_KSC_VERIF_DATA","NULL",NOT_REPEATABLE},
{LG_CRPT_PKCS_REF, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,13,PP_PRINT,"NULL","LG_CRPT_PKCS_REF","NULL",NOT_REPEATABLE},
{LG_CRPT_HASH_ALGO, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_HASH_ALGO","NULL",NOT_REPEATABLE},
{LG_CRPT_ENC_ALGO, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_ENC_ALGO","NULL",NOT_REPEATABLE},
{LG_CRPT_SIG_ALGO, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_SIG_ALGO","NULL",NOT_REPEATABLE},
{LG_CRPT_MASK, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","LG_CRPT_MASK","NULL",NOT_REPEATABLE},
{LG_CRPT_P_PARAM_IND, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_P_PARAM_IND","NULL",NOT_REPEATABLE},
{LG_CRPT_SALT_PARAM_IND, FT_SINGLE,FF_HEX,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","LG_CRPT_SALT_PARAM_IND","NULL",NOT_REPEATABLE},
{LG_CRPT_P, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_P","NULL",NOT_REPEATABLE},
{LG_CRPT_SALT, FT_SINGLE,FF_HEX,AF_ASCII,FL_B,LU_BYTE,256,PP_PRINT,"NULL","LG_CRPT_SALT","NULL",NOT_REPEATABLE},

},
"gs_LGCryptoFields"
};




static const TSLGCryptoTemplate	gs_ktmplt_kg_id = {
	LG_TMPLT_LAY_KG_ID, LG_TMPLT_KG_ID,LG_TMPLT_KG_ID_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
	}
};

static const TSLGCryptoTemplate	gs_tmplt_enc_ktk = {
LG_TMPLT_LAY_ENC_KTK,LG_TMPLT_ENC_KTK,LG_TMPLT_ENC_KTK_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
		LG_CRPT_FLD_KEY_METHOD,
		LG_CRPT_FLD_RSA_OUT,
	}
};

static const TSLGCryptoTemplate	gs_tmplt_kg_p = {
LG_TMPLT_LAY_KG_P,LG_TMPLT_KG_P,LG_TMPLT_KG_P_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
		LG_CRPT_FLD_MODULO,
		LG_CRPT_FLD_EXPONENT,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_kd_p = {
LG_TMPLT_LAY_KD_P,LG_TMPLT_KD_P,LG_TMPLT_KD_P_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
		LG_CRPT_FLD_MODULO,
		LG_CRPT_FLD_EXPONENT,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_sn_scd = {
LG_TMPLT_LAY_SN_SCD,LG_TMPLT_SN_SCD,LG_TMPLT_SN_SCD_NB_FIELDS,
	{
		LG_CRPT_FLD_MANUFAC,
		LG_CRPT_FLD_SERIAL,
		
	}
};


static const TSLGCryptoTemplate	gs_tmplt_rand = {
LG_TMPLT_LAY_RAND,LG_TMPLT_RAND,LG_TMPLT_RAND_NB_FIELDS,
	{
		LG_CRPT_FLD_RAND,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_gdg_authen = {
LG_TMPLT_LAY_GDG_AUTHEN,LG_TMPLT_GDG_AUTHEN,LG_TMPLT_GDG_AUTHEN_NB_FIELDS,
	{
		LG_CRPT_FLD_SERVER_SIG,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_kd_p_sig = {
LG_TMPLT_LAY_KD_P_SIG,LG_TMPLT_KD_P_SIG,LG_TMPLT_KD_P_SIG_NB_FIELDS,
	{
		LG_CRPT_FLD_KD_SIG,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_kg_p_sig = {
LG_TMPLT_LAY_KG_P_SIG,LG_TMPLT_KG_P_SIG,LG_TMPLT_KG_P_SIG_NB_FIELDS,
	{
		LG_CRPT_FLD_KG_SIG,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_ktk_sig = {
LG_TMPLT_LAY_KTK_SIG,LG_TMPLT_KTK_SIG,LG_TMPLT_KTK_SIG_NB_FIELDS,
	{
		LG_CRPT_FLD_KTK_SIG,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_enc_kt = {
LG_TMPLT_LAY_ENC_KT,LG_TMPLT_ENC_KT,LG_TMPLT_ENC_KT_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
		LG_CRPT_FLD_KT_VERIF_DATA,
		LG_CRPT_FLD_KT_KEY,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_enc_ksc = {
LG_TMPLT_LAY_ENC_KSC,LG_TMPLT_ENC_KSC,LG_TMPLT_ENC_KSC_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
		LG_CRPT_FLD_KSC_VERIF_DATA,
		LG_CRPT_FLD_KSC_KEY,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_mac = {
LG_TMPLT_LAY_MAC,LG_TMPLT_MAC,LG_TMPLT_MAC_NB_FIELDS,
	{
		LG_CRPT_FLD_MAC,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_kd_id = {
LG_TMPLT_LAY_KD_ID,LG_TMPLT_KD_ID,LG_TMPLT_KD_ID_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_ktk_id = {
LG_TMPLT_LAY_KTK_ID,LG_TMPLT_KTK_ID,LG_TMPLT_KTK_ID_NB_FIELDS,
	{
		LG_CRPT_FLD_KEY_TYPE,
		LG_CRPT_FLD_KEY_SERIAL,
		LG_CRPT_FLD_KEY_VERSION,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_crypto_elts = {
LG_TMPLT_LAY_CRYPTO_ELTS,LG_TMPLT_CRYPTO_ELTS,LG_TMPLT_CRYPTO_ELTS_NB_FIELDS,
	{
		LG_CRPT_FLD_PKCS_REF,
		LG_CRPT_FLD_HASH_ALGO,
		LG_CRPT_FLD_ENC_ALGO,
		LG_CRPT_FLD_SIG_ALGO,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_param_1 = {
LG_TMPLT_LAY_PARAM_1,LG_TMPLT_PARAM_1,LG_TMPLT_PARAM_1_NB_FIELDS,
	{
		LG_CRPT_FLD_HASH_ALGO,
		LG_CRPT_FLD_MASK,
		LG_CRPT_FLD_P_PARAM_IND,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_param_2 = {
LG_TMPLT_LAY_PARAM_2,LG_TMPLT_PARAM_2,LG_TMPLT_PARAM_2_NB_FIELDS,
	{
		LG_CRPT_FLD_HASH_ALGO,
		LG_CRPT_FLD_MASK,
		LG_CRPT_FLD_SALT_PARAM_IND,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_param_3 = {
LG_TMPLT_LAY_PARAM_3,LG_TMPLT_PARAM_3,LG_TMPLT_PARAM_3_NB_FIELDS,
	{
		LG_CRPT_FLD_HASH_ALGO,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_param_4 = {
LG_TMPLT_LAY_PARAM_4,LG_TMPLT_PARAM_4,LG_TMPLT_PARAM_4_NB_FIELDS,
	{
		LG_CRPT_FLD_P,
	}
};


static const TSLGCryptoTemplate	gs_tmplt_param_5 = {
LG_TMPLT_LAY_PARAM_5,LG_TMPLT_PARAM_5,LG_TMPLT_PARAM_5_NB_FIELDS,
	{
		LG_CRPT_FLD_SALT,
	}
};




static const TSLGCryptoTemplate* tab_templates[]={
&gs_ktmplt_kg_id	,
&gs_tmplt_enc_ktk 	,
&gs_tmplt_kg_p 		,
&gs_tmplt_kd_p 		,
&gs_tmplt_sn_scd 	,
&gs_tmplt_rand 		,
&gs_tmplt_gdg_authen ,
&gs_tmplt_kd_p_sig 	,
&gs_tmplt_kg_p_sig 	,
&gs_tmplt_ktk_sig 	,
&gs_tmplt_enc_kt 	,
&gs_tmplt_enc_ksc 	,
&gs_tmplt_mac 		,
&gs_tmplt_kd_id 	,	
&gs_tmplt_ktk_id 	,
&gs_tmplt_crypto_elts,
&gs_tmplt_param_1 	,
&gs_tmplt_param_2 	,
&gs_tmplt_param_3 	,
&gs_tmplt_param_4 	,
&gs_tmplt_param_5 	,
};

TSLGCrytTmplDesc tab_templatesDesc[]={
		{LG_TMPLT_KG_ID,"IdKG "},
		{LG_TMPLT_ENC_KTK,"KTK-chiffrée "},
		{LG_TMPLT_KG_P,"KGp "},
		{LG_TMPLT_KD_P,"KDp "},
		{LG_TMPLT_SN_SCD,"SnSCD "},
		{LG_TMPLT_RAND,"Aléa "},
		{LG_TMPLT_GDG_AUTHEN,"Authentification GDG "},
		{LG_TMPLT_KD_P_SIG,"Signature de KDp "},
		{LG_TMPLT_KG_P_SIG,"Signature de KGp "},
		{LG_TMPLT_KTK_SIG,"Signature de KTK "},
		{LG_TMPLT_ENC_KT,"KT-chiffrée "},
		{LG_TMPLT_ENC_KSC,"KSC-chiffrée "},
		{LG_TMPLT_MAC,"Sceau "},
		{LG_TMPLT_KD_ID,"IdKD "},
		{LG_TMPLT_KTK_ID,"IdKTK "},
		{LG_TMPLT_CRYPTO_ELTS,"Elts_Crypto "},
		{LG_TMPLT_PARAM_1,"Param1 "},
		{LG_TMPLT_PARAM_2,"Param2 "},
		{LG_TMPLT_PARAM_3,"Param3 "},
		{LG_TMPLT_PARAM_4,"Param4 "},
		{LG_TMPLT_PARAM_5,"Param5 "},
};


int InitLGCryptoInfo(TSLGCryptoInfo* pkLGCryptoInfo,E_LG_TMPLT_LAYOUT eTemplate)
{

	pkLGCryptoInfo->nLength=0;
	memset(pkLGCryptoInfo->sData,'\0',sizeof(pkLGCryptoInfo->sData));
	memset(pkLGCryptoInfo->nPresent,0,sizeof(pkLGCryptoInfo->nPresent));
	memset(pkLGCryptoInfo->nPosTlv,0,sizeof(pkLGCryptoInfo->nPosTlv));

	pkLGCryptoInfo->pkTemplateLayout = tab_templates[eTemplate];
	pkLGCryptoInfo->pkTagProperties = &gs_LGCryptoFields;
	return OK;
}

int InitLGCryptoMsg(TSLGCryptoMsg* pkLGCryptoMsg)
{

	pkLGCryptoMsg->nNbFields=0;
	pkLGCryptoMsg->nLength=0;
	memset(pkLGCryptoMsg->sData,'\0',sizeof(pkLGCryptoMsg->sData));
	memset(pkLGCryptoMsg->nPresent,0,sizeof(pkLGCryptoMsg->nPresent));
	memset(pkLGCryptoMsg->nPosTlv,0,sizeof(pkLGCryptoMsg->nPosTlv));

	return OK;
}

/*
int InitLGCryptoElts(TSLGCryptoElts* cryptoElts)
{

	memset(cryptoElts->crptHashAlgo	,'\0'	,sizeof(cryptoElts->crptHashAlgo)	);
	memset(cryptoElts->crptEncAlgo	,'\0'	,sizeof(cryptoElts->crptEncAlgo)	);
	memset(cryptoElts->crptSigAlgo	,'\0'	,sizeof(cryptoElts->crptSigAlgo)	);
	memset(cryptoElts->oaepHashAlgo	,'\0'	,sizeof(cryptoElts->oaepHashAlgo)	);
	memset(cryptoElts->oaepMask		,'\0'	,sizeof(cryptoElts->oaepMask)		);

	memset(cryptoElts->oaepPParamInd,'\0'	,sizeof(cryptoElts->oaepPParamInd)	);
	memset(cryptoElts->pssHashAlgo	,'\0'	,sizeof(cryptoElts->pssHashAlgo)	);
	memset(cryptoElts->pssMask		,'\0'	,sizeof(cryptoElts->pssMask)		);
	memset(cryptoElts->pkcsHashAlgo	,'\0'	,sizeof(cryptoElts->pkcsHashAlgo)	);
	memset(cryptoElts->crptP		,'\0'	,sizeof(cryptoElts->crptP)			);
	memset(cryptoElts->crptSalt		,'\0'	,sizeof(cryptoElts->crptSalt)		);

	return OK;
}*/

int InitLGKeyElts(TSLGKeyElts*  keyElts)
{

	memset(keyElts->keyType			,'\0'	,sizeof(keyElts->keyType)		);
	memset(keyElts->keyIndex		,'\0'	,sizeof(keyElts->keyIndex)		);
	memset(keyElts->keySeqNo		,'\0'	,sizeof(keyElts->keySeqNo)		);
	memset(keyElts->keyVersionNo	,'\0'	,sizeof(keyElts->keyVersionNo)	);
	memset(keyElts->keySerialNo		,'\0'	,sizeof(keyElts->keySerialNo)	);
	memset(keyElts->keyValue		,'\0'	,sizeof(keyElts->keyValue)		);
	memset(keyElts->keyEncValue		,'\0'	,sizeof(keyElts->keyEncValue)	);
	memset(keyElts->keyCheckValue	,'\0'	,sizeof(keyElts->keyCheckValue)	);

	return OK;
}
/*
int InitLGCryptoElts(TSLGCryptoElts* cryptoElts)
{

	memset(cryptoElts->crptHashAlgo	,'\0'	,sizeof(cryptoElts->crptHashAlgo)	);
	memset(cryptoElts->crptEncAlgo	,'\0'	,sizeof(cryptoElts->crptEncAlgo)	);
	memset(cryptoElts->crptSigAlgo	,'\0'	,sizeof(cryptoElts->crptSigAlgo)	);
	memset(cryptoElts->oaepHashAlgo	,'\0'	,sizeof(cryptoElts->oaepHashAlgo)	);
	memset(cryptoElts->oaepMask		,'\0'	,sizeof(cryptoElts->oaepMask)		);

	memset(cryptoElts->oaepPParamInd,'\0'	,sizeof(cryptoElts->oaepPParamInd)	);
	memset(cryptoElts->pssHashAlgo	,'\0'	,sizeof(cryptoElts->pssHashAlgo)	);
	memset(cryptoElts->pssMask		,'\0'	,sizeof(cryptoElts->pssMask)		);
	memset(cryptoElts->pkcsHashAlgo	,'\0'	,sizeof(cryptoElts->pkcsHashAlgo)	);
	memset(cryptoElts->crptP		,'\0'	,sizeof(cryptoElts->crptP)			);
	memset(cryptoElts->crptSalt		,'\0'	,sizeof(cryptoElts->crptSalt)		);

	return OK;
}*/

int GetLGCryptoTemplateId(char* template_code)
{
	int i;
	int nSize;

	nSize = sizeof(tab_templates)/sizeof(tab_templates[0]);
	for(i=0; i < nSize; i++ )
	{
		if( memcmp(tab_templates[i]->template_code,template_code,2) == 0 )
			return i;
	}

	return -1;
}





