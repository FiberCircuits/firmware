BIN_DIR = /tmp/arduino_build

ELF = $(BIN_DIR)/*elf


# tip: look for "fqbn" when compiling with arduino, this is what comes:
BOARD = STMicroelectronics:stm32:GenC0:pnum=GENERIC_C011D6YX,upload_method=swdMethod,xserial=generic,opt=oslto,dbg=none,rtlib=nano


.PHONY: all upload compile pref_list debug startgdbserver local_startgdbserver \
		stopgdbserver clean clean_cache c l d


all: compile # TODO make upload


# TODO: make install examples:
#
# Install AVR board support, 1.6.2
# arduino --install-boards "arduino:avr:1.6.2"
#
# Install Bridge and Servo libraries
# arduino --install-library "Bridge:1.0.0,Servo:1.2.0"
#
# Documentation:
# https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc
#
# TODO
# upload: *.ino *.cpp *.h stopgdbserver clean_cache
# 	arduino  --pref build.path=$(BIN_DIR) --preserve-temp-files --board $(BOARD) \
# 			 --upload --verbose-upload --useprogrammer --verbose *.ino

compile: *.ino *.cpp *.h clean_cache
	arduino --pref build.path=$(BIN_DIR) --preserve-temp-files --board $(BOARD) \
			--verify --verbose *ino

pref_list:
	arduino --get-pref

clean_cache:
	rm -rf /tmp/arduino_cache*

clean: clean_cache
	rm -rf $(BIN_DIR)

