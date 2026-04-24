#ifndef HPS_SECURITY_DATA_H
#define HPS_SECURITY_DATA_H

enum {

	SECURITY_FORMAT             =0	,
	PIN_BLOCK_FORMAT				,
	PIN_ENCRYPTION_KEY_INDEX		,
	MAC_KEY_INDEX					,
	SOURCE_PID						,
	SOURCE_RESOURCE					,
	DESTINATION_PID					,
	DESTINATION_RESOURCE			,
	USER							,
	PURGE_TIME						,
	SOURCE_NODE_ID					,
	DESTINATION_NODE_ID				,
	PURGE_TIME_MS					,
	SECURITY_DATA_QTY
};


#define SECURITY_FORMAT_LEN             2
#define PIN_BLOCK_FORMAT_LEN            2
#define PEK_INDEX_LEN					3
#define MAC_KEY_INDEX_LEN               3
#define SOURCE_PID_LEN                  8
#define SOURCE_RESOURCE_LEN             6 
#define DESTINATION_PID_LEN             8 
#define DESTINATION_RESOURCE_LEN        6
#define USER_LEN                        15
#define PURGE_TIME_LEN                  9
#define PURGE_TIME_PERIOD_LEN			6
#define PURGE_TIME_MS_LEN               3

/*#define SECURITY_DATA_LEN				62*/
/*#define  POWERCARD_USER_OFFSET     38*/




typedef struct s_secdata
{
    int offset;
    int length;
}t_secdata;


int GetSecurityData (int subfield,char *security_data, char *value,int *length);
int PutSecurityData (int subfield,char *security_data, char *value ,int length);

#endif  /** HPS_SECURITY_DATA_H  **/
