COMP=g++
CROSS=arm-linux-
SRC=./src/*.cpp
OUTP=./lib/
TARGET=libhcharExtractor
LDIR_ARM=/usr/local/arm/4.3.2/arm-none-linux-gnueabi/libc/armv4t/lib/
LD_FLAGS=
OPT_FLAGS=-O0
ARM_FLAGS=-msoft-float -D__GCC_FLOAT_NOT_NEEDED -march=armv4t -mtune=arm920t
INSTALL_DIR=/lib/
INSTALL_DIR_ARM=/usr/local/arm/4.3.2/arm-none-linux-gnueabi/libc/armv4t/lib/
INC_FILES=./include/*
INC_INSTALL_DIR=/usr/include/bird
INC_INSTALL_DIR_ARM=/usr/local/arm/4.3.2/arm-none-linux-gnueabi/include/c++/4.3.2/bird

all: compile

compile:
	$(COMP) -ggdb -Wall -fPIC -c $(SRC) -O0
#movendo arquivos para pasta ./lib
	mv ./*.o $(OUTP)
	$(COMP) -shared -Wl,-soname,$(TARGET).so.1 -o $(OUTP)$(TARGET).so.1.0 $(OUTP)*.o $(LD_FLAGS)

cross:
	$(CROSS)$(COMP) -Wall -fPIC -c $(SRC) $(OPT_FLAGS) $(ARM_FLAGS)
#movendo arquivos para pasta ./lib
	mv ./*.o $(OUTP)
	$(CROSS)$(COMP) -shared -Wl,-soname,$(TARGET).so.1 -o $(OUTP)$(TARGET).so.1.0 $(OUTP)*.o -L$(LDIR_ARM) $(LD_FLAGS)

clean:
	rm -vf $(OUTP)$(TARGET) $(OUTP)*.o $(OUTP)*.s* $(OUTP)*~ *.o *.s* *~

#Instala a biblioteca na pasta lib
install: $(OUTP)$(TARGET).so.1.0
	cp $(OUTP)$(TARGET).so.1.0 $(INSTALL_DIR)
	ln -sf $(INSTALL_DIR)$(TARGET).so.1.0 $(INSTALL_DIR)$(TARGET).so
	ln -sf $(INSTALL_DIR)$(TARGET).so.1.0 $(INSTALL_DIR)$(TARGET).so.1
	mkdir -p $(INC_INSTALL_DIR)
	cp -r $(INC_FILES) $(INC_INSTALL_DIR)

#Instala a biblioteca na pasta lib do compilador arm
install-arm: $(OUTP)$(TARGET).so.1.0
	cp $(OUTP)$(TARGET).so.1.0 $(INSTALL_DIR_ARM)
	ln -sf $(INSTALL_DIR_ARM)$(TARGET).so.1.0 $(INSTALL_DIR_ARM)$(TARGET).so
	ln -sf $(INSTALL_DIR_ARM)$(TARGET).so.1.0 $(INSTALL_DIR_ARM)$(TARGET).so.1
	mkdir -p $(INC_INSTALL_DIR_ARM)
	cp -r $(INC_FILES) $(INC_INSTALL_DIR_ARM)
