# Project Name
TARGET = Interface.test

# Sources
CPP_SOURCES = \
	UiFramework.test.cpp  \
	../../Display.cpp \
	../../View/ListPageSsd1306I2cView.cpp \
	../../Presenters/NavigationPageItem.cpp \
	../../Presenters/SettingsPageItem.cpp \
	../../Presenters/NumericSettingsPageItem.cpp \
	../../Presenters/OptionsSettingsPageItem.cpp \
	../../Presenters/ListPage.cpp \
	../../Utilities/UiParameter.cpp
	
# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
