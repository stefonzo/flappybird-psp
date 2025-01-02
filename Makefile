TARGET = flappybird
OBJS = main.o nucleus.o callbacks.o

INCDIR =
CFLAGS = -Wall -std=c++17
CXXFLAGS = $(CLFAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS = -lpspgum -lpspgu -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Flappybird PSP

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
