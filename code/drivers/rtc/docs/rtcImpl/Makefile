COMP=g++
CROSS=arm-linux-
SRC=./src/main.cpp
OUTP=./bin/
IDIR=../../include
LDIR=../../lib
TARGET=rtcImpl
LD_FLAGS=-lhrtc
ARM_FLAGS=-msoft-float -D__GCC_FLOAT_NOT_NEEDED -march=armv4t -mtune=arm920t

all: compile

compile:
	$(COMP) -ggdb -Wall -I$(IDIR) -L$(LDIR) -o $(OUTP)$(TARGET) $(LD_FLAGS) $(SRC)

cross:
	$(CROSS)$(COMP) -Wall -I$(IDIR) -L$(LDIR) -o $(OUTP)$(TARGET) $(LD_FLAGS) $(SRC) -O3 $(ARM_FLAGS)

clean:
	rm -vf $(OUTP)$(TARGET) $(OUTP)*.o $(OUTP)*.s* $(OUTP)*~ *.o *.s* *~
