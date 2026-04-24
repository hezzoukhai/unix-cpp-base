# Main source file
cmake_minimum_required (VERSION 2.8.11)
find_package(Git)

if(GIT_FOUND)
	#Get working branch
	execute_process(
        	COMMAND git rev-parse --abbrev-ref HEAD
        	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        	OUTPUT_VARIABLE GIT_BRANCH
        	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	#Get latest commit hash for working branch
	execute_process(
        	COMMAND git rev-parse --short HEAD
        	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        	OUTPUT_VARIABLE GIT_COMMIT_HASH
        	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	#Get current Tag
	execute_process(
        	COMMAND git describe --tags --abbrev=0
        	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        	OUTPUT_VARIABLE GIT_TAG
        	OUTPUT_STRIP_TRAILING_WHITESPACE
	)	

	message(STATUS "Git current branch: ${GIT_BRANCH}")
	message(STATUS "Git commit: ${GIT_COMMIT_HASH}")
	message(STATUS "Git tag ${GIT_TAG}")


	add_definitions("-DGIT_TAG=\"${GIT_TAG}\"")
	add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")
	add_definitions("-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")

endif()

set(CMAKE_INSTALL_PREFIX $ENV{HOME}/usr/ CACHE PATH "Install path")
#SET(CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules")

#set(CTL_INSTALL_PATH $ENV{HOME}/usr/ctl_ext)
set(CTL_INSTALL_PATH $ENV{HOME}/usr/ctl)
set(CTL_SOURCE_PATH $ENV{HOME}/usr/ctl)
#set(SHL_INSTALL_PATH $ENV{HOME}/usr/shl_ext)
set(SHL_INSTALL_PATH $ENV{HOME}/usr/shl)
set(SHL_SOURCE_PATH $ENV{HOME}/usr/shl)

#Dev perm
set(P7_DFLT_BIN_PERM	"OWNER_EXECUTE;OWNER_WRITE;OWNER_READ;GROUP_WRITE;GROUP_READ;GROUP_EXECUTE;WORLD_READ;WORLD_EXECUTE")
set(P7_DFLT_LIB_PERM	"OWNER_EXECUTE;OWNER_WRITE;OWNER_READ;GROUP_WRITE;GROUP_READ;GROUP_EXECUTE;WORLD_READ")

#Prod perm
#set(P7_DFLT_BIN_PERM	"OWNER_EXECUTE;OWNER_WRITE;OWNER_READ")
#set(P7_DFLT_LIB_PERM	"OWNER_EXECUTE;OWNER_WRITE;OWNER_READ")


#PowerCARD configuration options
include(instance_config.txt)

project (PowerCARD C) 

configure_file (
  "p7_config.h.in"
  "$ENV{INC}/p7_config.h"
  )
  
# Uncomment to make compilation verbose  
set(CMAKE_VERBOSE_MAKEFILE ON)

include(CMakeDetermineSystem)


if( MQ_SERVER )
	if( NOT DEFINED ENV{MQ_PATH})
		message( FATAL_ERROR "Missing MQ PATH" )
	endif()
endif()

if(HSM_BOX MATCHES "C2P")
	set(HSM_LIB hsmc2p)
elseif(HSM_BOX MATCHES "ATALLA")
	set(HSM_LIB hsmatalla)	
else()
	set(HSM_LIB hsmthales)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(PLATFORM "LINUX")
	#add_definitions("-fPIC")
elseif(CMAKE_SYSTEM_NAME MATCHES "AIX")
	set(PLATFORM "AIX")
	# we do the below because CMake apply rtl by default. We don't want that. libclntsh has a DES function too which may cause issues
	set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-Wl,-bnortl")
elseif(CMAKE_SYSTEM_NAME MATCHES "HP-UX")
	set(PLATFORM "HPUX")
	add_definitions("-DSS_64BIT_SERVER")
elseif(CMAKE_SYSTEM_NAME MATCHES "SunOS")
	set(PLATFORM "SOLARIS")
	set(CMAKE_C_FLAGS "-m64")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(PLATFORM "WIN32")
else(CMAKE_SYSTEM_NAME MATCHES "Windows")
	message("NO SYSTEM FOUND!!!")
endif() 

message(">>>>>> PLATFORM: ${PLATFORM}")



IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
  #SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

add_definitions(-D_THREAD_SAFE -D${PLATFORM} -D_POSIX_PTHREAD_SEMANTICS)


# Proc-C compiler
set(PROC proc)

#uncomment below to compile with insure(in .pcard_profile, we should have export CC="insure gcc")
#set(ins_c_compiler		"/app/pwrcard/insure/bin/insure /usr/bin/gcc")
#separate_arguments(insure_c_compiler UNIX_COMMAND "${ins_c_compiler}")



# Pro-C compiler options
set(proc_args_list "THREADS=YES  sqlcheck=full userid=$ENV{usercomp}/$ENV{passwcomp} define=_LONG_LONG code=KR_C include=/usr/include include=$ENV{HOME}/src/inc include=$ENV{HOME}/src-spec/inc include=$ENV{ORACLE_HOME}/precomp/public nls_local=yes varchar=yes ireclen=132")
separate_arguments(proc_args UNIX_COMMAND "${proc_args_list}")


# Include directories
include_directories($ENV{INC};$ENV{ORACLE_HOME}/precomp/public;$ENV{ORACLE_HOME}/rdbms/public;$ENV{ORACLE_HOME}/plsql/public;$ENV{ORACLE_HOME}/network/public)

# Libs directories
link_directories ($ENV{HPS_LIBPATH};$ENV{OPENSSL_PATH}/lib;$ENV{ORACLE_HOME}/lib)

# default libs that may be used
set(p7_dflt_libs 	ctrlunits;actions;int;utils;srvutils;bridge;saf;${HSM_LIB};hsmtdes;utils;privtlv;authotlv;prodtlv;queue;comora;piso;tools;get;pthr;monitoring;ppuf;pcps;psmspuf;psms;iso3;actions;m;pthread;ssl;crypto)

if ( BO_ISS_ST_CORE OR BO_ACQ_ST_CORE )
	set(p7_dflt_libs ${p7_dflt_libs};batchutils)
endif ()

if(PLATFORM MATCHES "SOLARIS")
	set(p7_dflt_libs ${p7_dflt_libs};socket;nsl)
endif()

set(p7_ora_libs 	clntsh;dl)

#this function will process a ProC list of source files(e.g fic1.pc fic2.pc,...etc) to:
# 1- Add command line to compile the files with the ProC compiler
# 2- Add the resulting .c files to _c_src list(results are appended to the incoming list)
# 3- Add the temporary .lis files to _proc_lis_files(the resulting list contains only the .lis of the provided .pc files)
function(preproc_pc _proc_src _c_src _proc_lis_files)
SET(proc_src 	${_proc_src})
SET(c_src 		"${${_c_src}}")
foreach(proc_file ${proc_src})
	ADD_CUSTOM_COMMAND(
	   OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${proc_file}.c
	   COMMAND ${PROC} ${proc_args} iname=${CMAKE_CURRENT_SOURCE_DIR}/${proc_file}
	   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${proc_file}.pc
	   COMMENT "Preprocessing ${proc_file}.pc"
	   )
	set(c_src ${c_src};${proc_file}.c)
	set(proc_lis_files 		${proc_lis_files};${CMAKE_CURRENT_SOURCE_DIR}/${proc_file}.lis)
endforeach()
SET(${_c_src} "${c_src}" PARENT_SCOPE)
SET(${_proc_lis_files} "${proc_lis_files}" PARENT_SCOPE)
endfunction(preproc_pc)



#include spec directory
add_subdirectory (${CMAKE_SOURCE_DIR}/../src-spec ${CMAKE_SOURCE_DIR}/../src-spec/build)


if( BO_ISS_ST_CORE OR
	BO_ACQ_ST_CORE OR
	BO_POS_ST_CORE OR
	BO_ATM_ST_CORE OR
	BO_ACH_ST_CORE )
add_subdirectory (${CMAKE_SOURCE_DIR}/backoff)
endif()
add_subdirectory (${CMAKE_SOURCE_DIR}/libs)
add_subdirectory (${CMAKE_SOURCE_DIR}/daemons)
add_subdirectory (${CMAKE_SOURCE_DIR}/interfaces)
add_subdirectory (${CMAKE_SOURCE_DIR}/servers)
add_subdirectory (${CMAKE_SOURCE_DIR}/utils)




