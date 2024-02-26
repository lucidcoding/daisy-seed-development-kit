# Project Name
TARGET = Interface.test

# Sources
CPP_SOURCES = \
	Main.test.cpp  \
	UserInterface.cpp \
	../../Presenters/Root.cpp \
	../../Presenters/NavigationListPageItem.cpp \
	../../Presenters/ListPage.cpp \
	../../Presenters/PotentiometerArrayPage.cpp \
	../../Presenters/PotentiometerArrayPageItem.cpp \
	../../Presenters/TabPage.cpp \
	../../Presenters/TabPageItem.cpp \
	../../Presenters/SettingsListPageItem.cpp \
	../../Presenters/NumericSettingsListPageItem.cpp \
	../../Presenters/NumericPotentiometerArrayPageItem.cpp \
	../../Presenters/OptionsPotentiometerArrayPageItem.cpp \
	../../Utilities/UiParameter.cpp \
	../Utilities/PotentiometerToDisplayValueScaler.cpp \
	../../Views/ListPageView.cpp \
	../../Views/PotentiometerArrayPageView.cpp \
	../../Views/TabPageView.cpp \
	../../ViewAdapters/Ili9341ViewAdapter.cpp \
	../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.cpp \
	../../../../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.cpp
	
# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
