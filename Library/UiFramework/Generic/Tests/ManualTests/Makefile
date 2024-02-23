# Project Name
TARGET = Interface.test

# Sources
CPP_SOURCES = \
	Main.test.cpp  \
	UserInterface.cpp \
	../../Presenters/Root.cpp \
	../../Views/ListPageSsd1306I2cView.cpp \
	../../Views/ListPageIli9341View.cpp \
	../../Presenters/NavigationPageItem.cpp \
	../../Presenters/SettingsPageItem.cpp \
	../../Presenters/NumericSettingsPageItem.cpp \
	../../Presenters/OptionsSettingsPageItem.cpp \
	../../Presenters/ListPage.cpp \
	../../Utilities/UiParameter.cpp \
	../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.cpp
	
# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
