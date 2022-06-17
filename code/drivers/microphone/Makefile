COMP=g++
CROSS=arm-linux-
SRC=./src/*.cpp
OUTP=./bin/
TARGET=microphone
LD_FLAGS=
ARM_FLAGS=-msoft-float -D__GCC_FLOAT_NOT_NEEDED -march=armv4t -mtune=arm920t

all: compile

compile:
	$(COMP) -ggdb -Wall -o $(OUTP)$(TARGET) $(SRC) $(LD_FLAGS)

gdb:
	$(COMP) -g $(SRC) $(LD_FLAGS)

cross:
	$(CROSS)$(COMP) -ggdb -Wall -o $(OUTP)$(TARGET) $(SRC) $(LD_FLAGS) $(ARM_FLAGS) -O3

clean:
	rm -vf $(OUTP)$(TARGET) $(OUTP)*.o $(OUTP)*.s* $(OUTP)*~ *.o *.s* *~
