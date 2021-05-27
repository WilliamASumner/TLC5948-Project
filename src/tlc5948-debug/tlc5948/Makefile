ARDUINO_LIBS =
ARDMK_DIR = ../../

DEVICE_MAKE ?= Teensy

ARDUINO_LIBS += SPI
ARDUINO_LIBS += FastLED

ifeq ($(DEVICE_MAKE),Teensy)
	F_CPU=600000000
	BOARD_TAG    = teensy41
	TOOLS_PATH=/Applications/Teensyduino.app/Contents/Java/hardware/tools/arm/bin/
else
	BOARD_TAG    = uno
endif

include $(ARDMK_DIR)/$(DEVICE_MAKE).mk
