#ifndef RM_DSTATE_H
#define RM_DSTATE_H

#define ET_INI          0
#define ET_FIN          1
#define ET_UNDEF        2
#define ET_ERR         99
#define ET_BAL         80
#define ET_DISCO       81
#define ET_START       82

#define ET_AT_0         3
#define ET_IG_0         4
#define ET_A3_0         5
#define ET_HI_0         6
#define ET_I2_0         7
#define ET_I3_0         8
#define ET_IF_0         9
#define ET_IN_0        10
#define ET_IR_0        11
#define ET_A4_0        12
#define ET_I4_0        13
#define ET_IO_0        14
#define ET_CD_0        15
#define ET_CH_0        16
#define ET_CS_0        17
#define ET_DC_0        18
#define ET_DE_0        19
#define ET_RI_0        20
#define ET_RS_0        21
#define ET_TR_0        22
#define ET_VI_0        23
#define ET_RO_0        24
#define ET_EM_0        25
#define ET_IC_0        26
#define ET_10_0        27
#define ET_11_0        28
#define ET_20_0        29
#define ET_21_0        30
#define ET_22_0        31
#define ET_40_0        32
#define ET_41_0        33
#define ET_49_0        34
#define ET_50_0        35
#define ET_51_0        36
#define ET_52_0        37
#define ET_53_0        38
#define ET_54_0        39
#define ET_5C_0        40
#define ET_5E_0        41
#define ET_71_0        42
#define ET_73_0        43
#define ET_77_0        44
#define ET_79_0        45
#define ET_85_0        46
#define ET_86_0        47
#define ET_DB_0        48
#define ET_FI_0        49
#define ET_CP_0        50
#define ET_EL_0        51
#define ET_GC_0        52
#define ET_K5_0        53
#define ET_KT_0        54
#define ET_RF_0        55
#define ET_TE_0        56
#define ET_TP_0        57
#define ET_TX_0        58
#define ET_TY_0        59
#define ET_ML_0        60
#define ET_TL_0        61
#define ET_EC_0        62
#define ET_IP_0        65


#define	TRX_TO_NW_CODE_BASE			500
#define	TRX_AUT_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 1)
#define	TRX_BAL_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 2)
#define	TRX_STAT_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 3)
#define	TRX_CHEQ_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 4)
#define	TRX_TFR_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 5)
#define	TRX_REJ_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 6)

#define	TRX_FROM_NW_CODE_BASE		600
#define	TRX_AUT_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 1)
#define	TRX_BAL_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 2)
#define	TRX_STAT_REP_FROM_NW		(TRX_FROM_NW_CODE_BASE + 3)
#define	TRX_CHEQ_REP_FROM_NW		(TRX_FROM_NW_CODE_BASE + 4)
#define	TRX_TFR_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 5)
#define	TRX_REJ_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 6)

#endif
