# Project Name
TARGET = DevelopmentKit

# Sources
CPP_SOURCES = \
	DevelopmentKit.cpp \
	Hardware/PotentiometerArray/PotentiometerArray.cpp

# Library Locations
#LIBDAISY_DIR = ../libdaisy/
#DAISYSP_DIR = ../daisysp/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
