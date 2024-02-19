# Project Name
TARGET = Interface.test

# Sources
CPP_SOURCES = \
	main.cpp  \
	UserInterface.cpp \
	../../View/ListPageSsd1306I2cView.cpp \
	../../View/ListPageIli9341View.cpp \
	../../View/TabPageIli9341View.cpp \
	../../View/PotentiometerArrayPageIli9341View.cpp \
	../../Presenters/NavigationPageItem.cpp \
	../../Presenters/SettingsPageItem.cpp \
	../../Presenters/NumericSettingsPageItem.cpp \
	../../Presenters/OptionsSettingsPageItem.cpp \
	../../Presenters/PotentiometerArrayPageItem.cpp \
	../../Presenters/ListPage.cpp \
	../../Presenters/TabPage.cpp \
	../../Presenters/PotentiometerArrayPage.cpp \
	../../Presenters/TabPageItem.cpp \
	../../Utilities/UiParameter.cpp \
	../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.cpp \
	../../../../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.cpp \
	../../../../Utilities/Scaling/FloatToIntScaler.cpp
	
# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
