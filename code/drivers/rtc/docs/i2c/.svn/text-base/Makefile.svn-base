COMP=gcc
CROSS=arm-linux-
SRC=*.c
OUTP=i2c
TARGET=i2c

all: compile

compile: 
	$(COMP) -ggdb -Wall -o $(OUTP)$(TARGET) $(SRC) $(LD_FLAGS) $(OP_FLAGS)

cross:
	$(CROSS)$(COMP) -Wall -o $(OUTP)$(TARGET) $(SRC) $(LD_FLAGS) $(OP_FLAGS) $(ARM_FLAGS)

clean:
	rm -vf $(OUTP)$(TARGET) $(OUTP)*.o $(OUTP)*.s* $(OUTP)*~
