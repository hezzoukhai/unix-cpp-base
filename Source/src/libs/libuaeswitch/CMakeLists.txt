# directory: libuaeswitch
 
set(lib_sources	uaeswitch_analyse.c
                uaeswitch_convert.c
                uaeswitch_dump.c
                uaeswitch_ini.c
                uaeswitch_util.c)

add_library (uaeswitch ${lib_sources})

install ( TARGETS uaeswitch
          ARCHIVE 			
          PERMISSIONS ${P7_DFLT_LIB_PERM}			
          DESTINATION lib)			
