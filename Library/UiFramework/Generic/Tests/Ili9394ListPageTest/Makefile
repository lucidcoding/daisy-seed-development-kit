# Project Name
TARGET = Interface.test

# Sources
CPP_SOURCES = \
	Main.test.cpp  \
	UserInterface.cpp \
	../../Presenters/Root.cpp \
	../../Views/ListPageIli9341View.cpp \
	../../Views/ListPageView.cpp \
	../../Presenters/NavigationPageItem.cpp \
	../../Presenters/SettingsPageItem.cpp \
	../../Presenters/NumericSettingsPageItem.cpp \
	../../Presenters/OptionsSettingsPageItem.cpp \
	../../Presenters/ListPage.cpp \
	../../Utilities/UiParameter.cpp \
	../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.cpp \
	../../ViewAdapters/Ssd1306I2cViewAdapter.cpp\
	../../ViewAdapters/Ili9341ViewAdapter.cpp
	
# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
