###################################################
# This File should be included in all Makefile :RF#
###################################################
.SUFFIXES :
.SUFFIXES : .pc .c .o .bin

#
#  Uncomment this block on Solaris Server :fcc compiler
#############################################################################################################
#LLIBSOCKETS=-lsocket

#LIBSOCKETS = -lsocket -lnsl -lpthread 

#CC = cc -KV9 -c -g -D_THREAD_SAFE -D_POSIX_PTHREAD_SEMANTICS -DSOLARIS $(LIBS) -I$(INC) -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public

#GCC = cc -KV9 -D_THREAD_SAFE -DSOLARIS -D_POSIX_PTHREAD_SEMANTICS -DSOLARIS -g

#
#  Uncomment this block on Solaris Server :cc compiler
#############################################################################################################
LLIBSOCKETS=-lsocket
#
#LIBSOCKETS = -lsocket -lnsl -lpthread 
#
#CC = cc -xarch=v9 -c -g -D_THREAD_SAFE -DSOLARIS -D_POSIX_PTHREAD_SEMANTICS $(LIBS) -I$(INC) -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public
#
#GCC = cc -xarch=v9 -D_THREAD_SAFE -DSOLARIS -D_POSIX_PTHREAD_SEMANTICS -DSOLARIS -g
#
#
#
#############################################################################################################
#  Uncomment this block on Aix Server (verify that OBJECT_MODE=64 exist in   .profile )
#############################################################################################################
LIBSOCKETS = -lnsl -lpthread 

#############################################################################################################
#  Compil with Insure Librairies
#############################################################################################################

#CC = insure  -q64 -c -g -D_THREAD_SAFE -D_POSIX_PTHREAD_SEMANTICS $(LIBS) -I$(INC) -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public


#GCC = insure  -q64  -D_THREAD_SAFE -D_POSIX_PTHREAD_SEMANTICS -g

#############################################################################################################
#  Compil with cc Librairies
#############################################################################################################

CC = cc -c -g -D_THREAD_SAFE -D_POSIX_PTHREAD_SEMANTICS -D_LARGE_THREADS -I$(INC) -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public

GCC = cc -D_THREAD_SAFE -D_POSIX_PTHREAD_SEMANTICS -D_LARGE_THREADS 



#############################################################################################################
#
#############################################################################################################
#  Uncomment this block on HPUX  Server
#############################################################################################################
#LIBSOCKETS = -lnsl -lpthread 
#
#CC = cc -c -g cc +Z +DA2.0W +DS2.0 -DSS_64BIT_SERVER -D_THREAD_SAFE -DSOLARIS -D_POSIX_PTHREAD_SEMANTICS $(LIBS) -I$(INC) -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public
#
#GCC = cc cc +Z +DA2.0W +DS2.0 -DSS_64BIT_SERVER  -D_THREAD_SAFE -DSOLARIS -D_POSIX_PTHREAD_SEMANTICS -DSOLARIS -g
#
#############################################################################################################


#PROCPPFLAGS= define=__64BIT__ code=KR_C include=/usr/include include=$(HOME)/src/inc include=$(ORACLE_HOME)/precomp/public nls_local=yes varchar=yes
PROCPPFLAGS= define=_LONG_LONG code=KR_C include=/usr/include include=$(HOME)/src/inc include=$(ORACLE_HOME)/precomp/public nls_local=yes varchar=yes

PROCPLSFLAGS= sqlcheck=full userid=$(usercomp)/$(passwcomp)

PROC=proc THREADS=YES

PROCBO=proc 


LIBS = -lpthread -L$(HPS_LIBPATH) -lbatchutils -lpthr -lqueue -lcomora -ltools -lutils -lpiso -lprivtlv -lprocessing -lutils -lauthotlv -lcb2a -lget -lpiccprod -lprivtlvemv -L$(ORACLE_HOME)/lib $(ORACLE_LINK)

LLIBTHREAD   =-lpthread -lnsl

LIBSSTANDARD =-lpthr -lqueue -lcomora -ltools -lpiso -lget

LIBSTH        = $(LLIBNSL) $(LLIBTHREAD) -L$(HPS_LIBPATH) $(LIBSSTANDARD) -L$(ORACLE_HOME)/lib $(LIBSSPEC) $(ORACLE_LINK)

CGC          =$(CC) -c -g -D_THREAD_SAFE -D$(PLATFORM) $(LIBSSTANDARD) -I$(INC) -I$(ORACLE_HOME)/precomp/public \
              -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public


CGO          =$(CC) -D_THREAD_SAFE -D_POSIX_PTHREAD_SEMANTICS -D$(PLATFORM) -g






