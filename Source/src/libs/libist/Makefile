include $(HOME_ENV_MAKEFILE)/env_precomp.mk

INCSPEC=$(HOME)/src/interfaces/uaeswitch/inc
TARGET=libuaeswitch.a

LIBNET=libnetuaeswitch.a

all : $(TARGET) $(LIBNET)

OBJS = uaeswitch_ini.o \
	uaeswitch_analyse.o \
	uaeswitch_util.o \
	uaeswitch_convert.o \
	uaeswitch_dump.o

OBJNET = uaeswitch_tcp.o

.c.o :
	$(CC) -I$(INCSPEC) $<

$(TARGET) : $(OBJS)
	ar -q -v $(TARGET) $(OBJS)
	mv $(TARGET) $(HPS_LIBPATH)

$(LIBNET) : $(OBJNET)
	ar -q -v $(LIBNET) $(OBJNET)
	mv $(LIBNET) $(HPS_LIBPATH)
