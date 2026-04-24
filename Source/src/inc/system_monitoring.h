#ifndef PWC_SYSTEM_MONITORING_H
#define PWC_SYSTEM_MONITORING_H


typedef struct {
	/* System */
	unsigned long		uptime;
	unsigned long		cpu_load;
	unsigned short		cpu_usage;
	unsigned long long	total_ram;
	unsigned long long	free_ram;
	unsigned long		mem_unit;
	unsigned short		nb_processes;
	/* Net*/
	unsigned long long	net_bytes_received; /* sum of all interfaces */
	unsigned long long	net_bytes_sent;
	/*Disk*/
	unsigned long long	disk_size;		/* f_blocks*f_bsize */ /* sum of all partitions*/
	unsigned long long	disk_free_space; /*f_bavail*f_bsize */
	unsigned long long	app_partion_size;
	unsigned long long	app_free_space;
	unsigned long long	disk_nb_reads;
	unsigned long long	disk_time_reading;
	unsigned long long	disk_nb_writes;
	unsigned long long	disk_time_writing;
	unsigned long long	disk_time;
	/* Msg queue*/
	unsigned long	msg_q_nb_msg;
	unsigned long	msg_q_curr_size;
	unsigned long	msg_q_max_size;
	/* Tablespaces */
	/*
	unsigned long	dbts_free_size;
	unsigned long	dbts_total_size;
	*/
} TSSysInfo;



extern int GetSystemStat(TSSysInfo*	SysInfo);

extern int		purge_period;
extern int		refresh_period;
extern char		app_partion_name[];


#endif

