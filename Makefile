TARGET = SmashGpsp
PSP_EBOOT_ICON = ICON0.png
PSP_EBOOT_ICON1 = NULL
PSP_EBOOT_PIC1 = NULL
PSP_EBOOT_SND0 = NULL
OBJS = main.o
LIBS = -lpspaudiolib -lpspaudio -lpspgu

INCDIR = 
CFLAGS = -Os -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = SmashGpsp 0.2b

#USE_PSPSDK_LIBC = 1
USE_USER_LIBS = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build2.mak
