# Project Name
TARGET = Interface.test

# Sources
CPP_SOURCES = \
	Main.test.cpp  \
	UserInterface.cpp \
	../../Presenters/Root.cpp \
	../../Presenters/NavigationListPageItem.cpp \
	../../Presenters/SettingsListPageItem.cpp \
	../../Presenters/NumericSettingsListPageItem.cpp \
	../../Presenters/OptionsSettingsListPageItem.cpp \
	../../Presenters/ListPage.cpp \
	../../ViewAdapters/Ssd1306I2cViewAdapter.cpp\
	../../Views/ListPageView.cpp \
	../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.cpp
	
# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
