# Project Name
TARGET = OpdVaSynth

# Sources
CPP_SOURCES = \
	main.cpp \
	vasynth.cpp \
	port.cpp

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
