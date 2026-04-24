/*
** <Name>
**  trace_macros.h
**
** <Descr>
**  header file of common macros
**
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       04Jan04   Y. OUADI       Initiale Version
**
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef TRACE_MACROS_H
#define TRACE_MACROS_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   
   /* - External Declaration When It is missing from Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */
   /* - Static Values */

   /* - Macros */
   #define START_TRACE(calling_funct) \
   {\
      sprintf(sLine, "Start %s()", calling_funct);\
      trace_event(sLine, PROCESSING);\
   }

   #define START_TRACE_PAR1(calling_funct, fmt, arg1) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "Start %%s(%s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, arg1);\
      trace_event(sLine, PROCESSING);\
   }

   #define START_TRACE_PAR2(calling_funct, fmt, arg1, arg2) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "Start %%s(%s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, arg1, arg2);\
      trace_event(sLine, PROCESSING);\
   }

   #define START_TRACE_PAR3(calling_funct, fmt, arg1, arg2, arg3) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "Start %%s(%s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, arg1, arg2, arg3);\
      trace_event(sLine, PROCESSING);\
   }

   #define START_TRACE_PAR4(calling_funct, fmt, arg1, arg2, arg3, arg4) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "Start %%s(%s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, arg1, arg2, arg3, arg4);\
      trace_event(sLine, PROCESSING);\
   }

   #define START_TRACE_PAR5(calling_funct, fmt, arg1, arg2, arg3, arg4, arg5) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "Start %%s(%s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, arg1, arg2, arg3, arg4, arg5);\
      trace_event(sLine, PROCESSING);\
   }

   #define START_PLSQL_CALL(calling_funct, plsql_funct) \
   {\
      sprintf(sLine, "%s(): Start Call of PL/SQL %s()", calling_funct, plsql_funct);\
      trace_event(sLine, PROCESSING);\
   }

   #define END_PLSQL_CALL(calling_funct, plsql_funct) \
   {\
      sprintf(sLine, "%s(): End   Call of PL/SQL %s()", calling_funct, plsql_funct);\
      trace_event(sLine, PROCESSING);\
   }

   #define TRACE_PAR(arg_type, arg_name, arg_fmt, arg_val) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "%%s %%-20.20s : [%s]", arg_fmt);\
      sprintf(sLine, local_fmt, arg_type, arg_name, arg_val);\
      trace_event(sLine, TRACE);\
   }

   #define TRACE_PAR_LONG_STR(arg_type, arg_name, arg_val, arg_len) \
   {\
      int  cntr = 0;\
      while(cntr < arg_len){\
         sprintf(sLine, "%s %-20.20s : [%.55s]", arg_type, cntr == 0 ? arg_name : " ", arg_val + cntr);\
         trace_event(sLine, TRACE);\
         cntr+=55;\
      }\
   }

   #define TRACE_IN_PAR(arg_name, arg_fmt, arg_val) \
           TRACE_PAR(">  ", arg_name, arg_fmt, arg_val)

   #define TRACE_OUT_PAR(arg_name, arg_fmt, arg_val) \
           TRACE_PAR(" < ", arg_name, arg_fmt, arg_val)

   #define TRACE_INOUT_PAR(arg_name, arg_fmt, arg_val) \
           TRACE_PAR(">< ", arg_name, arg_fmt, arg_val)

   #define TRACE_ERR_MSG(            arg_name, arg_val, arg_len)\
           TRACE_PAR_LONG_STR("ER ", arg_name, arg_val, arg_len)

   #define TRACE_FAILURE_PAR1(calling_funct, fmt, arg1) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "%%s() failed :%s", fmt);\
      sprintf(sLine, local_fmt, calling_funct, arg1);\
      trace_event(sLine, ERROR);\
   }

   #define TRACE_CALL_FAILURE_PAR1(calling_funct, called_funct, fmt, arg1) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "%%s()-%%s() failed :%s", fmt);\
      sprintf(sLine, local_fmt, calling_funct, called_funct, arg1);\
      trace_event(sLine, ERROR);\
   }

   #define return_INT(calling_funct, ret) \
   {\
      sprintf(sLine, "End   %s(%d)", calling_funct, ret);\
      trace_event(sLine, PROCESSING);\
      return (ret);\
   }
   #define return_OK( calling_funct    )\
           return_INT(calling_funct, OK)


   #define return_INT_PAR1(calling_funct, ret, fmt, par1) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "End   %%s(%%d, %s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, ret, par1);\
      trace_event(sLine, PROCESSING);\
      return (ret);\
   }
   #define return_OK_PAR1( calling_funct,     fmt, par1) \
           return_INT_PAR1(calling_funct, OK, fmt, par1)

   #define return_INT_PAR2(calling_funct, ret, fmt, par1, par2) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "End   %%s(%%d, %s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, ret, par1, par2);\
      trace_event(sLine, PROCESSING);\
      return (ret);\
   }
   #define return_OK_PAR2( calling_funct,     fmt, par1, par2) \
           return_INT_PAR2(calling_funct, OK, fmt, par1, par2)

   #define return_INT_PAR3(calling_funct, ret, fmt, par1, par2, par3) \
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "End   %%s(%%d, %s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, ret, par1, par2, par3);\
      trace_event(sLine, PROCESSING);\
      return (ret);\
   }
   #define return_OK_PAR3( calling_funct,     fmt, par1, par2, par3) \
           return_INT_PAR3(calling_funct, OK, fmt, par1, par2, par3)

   #define return_INT_PAR4(calling_funct, ret, fmt, par1, par2, par3, par4)\
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "End   %%s(%%d, %s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, ret, par1, par2, par3, par4);\
      trace_event(sLine, PROCESSING);\
      return (ret);\
   }
   #define return_OK_PAR4( calling_funct,     fmt, par1, par2, par3, par4)\
           return_INT_PAR4(calling_funct, OK, fmt, par1, par2, par3, par4)

   #define return_INT_PAR5(calling_funct, ret, fmt, par1, par2, par3, par4, par5)\
   {\
      char local_fmt[128];\
      sprintf(local_fmt, "End   %%s(%%d, %s)", fmt);\
      sprintf(sLine, local_fmt, calling_funct, ret, par1, par2, par3, par4, par5);\
      trace_event(sLine, PROCESSING);\
      return (ret);\
   }
   #define return_OK_PAR5( calling_funct,     fmt, par1, par2, par3, par4, par5)\
           return_INT_PAR5(calling_funct, OK, fmt, par1, par2, par3, par4, par5)

   #define exit_OK(calling_funct) \
   {\
      sprintf(sLine, "End   %s(OK) ==> exit program", calling_funct);\
      trace_event(sLine, PROCESSING);\
      exit (OK);\
   }

   #define return_VOID(calling_funct) \
   {\
      sprintf(sLine, "End   %s(VOID)", calling_funct);\
      trace_event(sLine, PROCESSING);\
      return;\
   }

   #define return_FAILURE(calling_funct, return_code) \
   {\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define exit_FAILURE(calling_funct, return_code) \
   {\
      sprintf(sLine, "End   %s(%d) ==> exit program", calling_funct, return_code);\
      trace_event(sLine, FATAL);\
      exit (return_code);\
   }

   #define return_FAILURE_INT(calling_funct, status, return_code) \
   {\
      sprintf(sLine, "%s() failed : %d", calling_funct, status);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_FAILURE_STR(calling_funct, message, return_code) \
   {\
      sprintf(sLine, "%s() failed : %.60s", calling_funct, message);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_FAILURE_VOID(calling_funct, status) \
   {\
      sprintf(sLine, "%s() failed : %d", calling_funct, status);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(VOID)", calling_funct);\
      trace_event(sLine, PROCESSING);\
      return;\
   }

   #define return_FAILURE_INT_VAR(calling_funct, message, int_var, return_code) \
   {\
      sprintf(sLine, "%s() failed : %.60s [%d]", calling_funct, message, int_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_FAILURE_STR_VAR(calling_funct, message, str_var, return_code) \
   {\
      sprintf(sLine, "%s() failed : %.60s [%.20s]", calling_funct, message, str_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_FAILURE_VOID_INT(calling_funct, message, int_var)\
   {\
      sprintf(sLine, "%s() failed : %.60s [%d]", calling_funct, message, int_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(VOID)", calling_funct);\
      trace_event(sLine, PROCESSING);\
      return;\
   }

   #define return_FAILURE_VOID_STR(calling_funct, message, str_var)\
   {\
      sprintf(sLine, "%s() failed : %.60s [%.20s]", calling_funct, message, str_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(VOID)", calling_funct);\
      trace_event(sLine, PROCESSING);\
      return;\
   }

   #define return_CALL_FAILURE(calling_funct, called_funct, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed", calling_funct, called_funct);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define exit_CALL_FAILURE(calling_funct, called_funct, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed", calling_funct, called_funct);\
      trace_event(sLine, FATAL);\
      sprintf(sLine, "End   %s(%d) ==> exit program", calling_funct, return_code);\
      trace_event(sLine, FATAL);\
      exit (return_code);\
   }

   #define return_CALL_FAILURE_INT(calling_funct, called_funct, status, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %d", calling_funct, called_funct, status);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_CALL_FAILURE_STR(calling_funct, called_funct, message, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %.60s", calling_funct, called_funct, message);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_CALL_FAILURE_VOID(calling_funct, called_funct, status) \
   {\
      sprintf(sLine, "%s()-%s() failed : %d", calling_funct, called_funct, status);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(VOID)", calling_funct);\
      trace_event(sLine, PROCESSING);\
      return;\
   }

   #define return_CALL_FAILURE_STR_VAR(calling_funct, called_funct, message, str_var, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %.60s [%.20s]", calling_funct, called_funct, message, str_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_CALL_FAILURE_INT_VAR(calling_funct, called_funct, message, int_var, return_code) \
   {\
      sprintf(sLine, "%s()%s failed : %s [%d]", calling_funct, called_funct, message, int_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define return_CALL_FAILURE_VOID_INT(calling_funct, called_funct, message, int_var) \
   {\
      sprintf(sLine, "%s()%s failed : %.60s [%d]", calling_funct, called_funct, message, int_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(VOID)", calling_funct);\
      trace_event(sLine, PROCESSING);\
      return;\
   }

   #define return_CALL_FAILURE_VOID_STR(calling_funct, called_funct, message, str_var) \
   {\
      sprintf(sLine, "%s()%s failed : %.60s [%.20s]", calling_funct, called_funct, message, str_var);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(VOID)", calling_funct);\
      trace_event(sLine, PROCESSING);\
      return;\
   }

   #define return_CALL_FAILURE_ERRNO(calling_funct, called_funct, status, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %d->%s", calling_funct, called_funct, status, strerror(status));\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define exit_CALL_FAILURE_ERRNO(calling_funct, called_funct, status, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %d->%s", calling_funct, called_funct, status, strerror(status));\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d) ==> exit program", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      exit (return_code);\
   }

   #define return_CALL_FAILURE_ERRNO_VAR(calling_funct, called_funct, message, status, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %.60s [%d->%.20s]", calling_funct, called_funct, message, status, strerror(status));\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d)", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      return (return_code);\
   }

   #define exit_CALL_FAILURE_ERRNO_VAR(calling_funct, called_funct, message, status, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %.60s [%d->%.20s]", calling_funct, called_funct, message, status, strerror(status));\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "End   %s(%d) ==> exit program", calling_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      exit (return_code);\
   }

   #define START_THREAD_TRACE(thread_funct) \
   {\
      sprintf(sLine, "<< Start %s >>", thread_funct);\
      trace_event(sLine, PROCESSING);\
   }

   #define return_THREAD_CALL_FAILURE_INT(thread_funct, called_funct, status, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %d", thread_funct, called_funct, status);\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "<< End   %s(%d) >>", thread_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      pthread_exit((void *)return_code);\
   }

   #define return_THREAD_CALL_FAILURE_ERRNO(thread_funct, called_funct, status, return_code) \
   {\
      sprintf(sLine, "%s()-%s() failed : %d->%s", thread_funct, called_funct, status, strerror(status));\
      trace_event(sLine, ERROR);\
      sprintf(sLine, "<< End   %s(%d) >>", thread_funct, return_code);\
      trace_event(sLine, PROCESSING);\
      pthread_exit((void *)return_code);\
   }

   #define return_THREAD_INT(thread_funct, ret) \
   {\
      sprintf(sLine, "<< End   %s(%d) >>", thread_funct, ret);\
      trace_event(sLine, PROCESSING);\
      pthread_exit((void *)ret);\
   }

   #define return_THREAD_OK(thread_funct    ) \
           return_THREAD_INT(thread_funct, OK)

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */

   /* - Private Functions Declaration */
   #ifdef _FOR_INT_USE_
   #endif /* _FOR_INT_USE_ */

/* - Varuiables Definition Section ----------------------------------------- */
   /* - Module Variables Declaration --------------------------------------- */
   #ifdef _FOR_INT_USE_
   #endif /* _FOR_INT_USE_ */

   /* - Module Variables Publication --------------------------------------- */
   #ifdef _FOR_EXT_USE_
   #endif /* _FOR_INT_USE_ */

#endif /* TRACE_MACROS_H */


