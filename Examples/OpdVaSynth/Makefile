# Project Name
TARGET = oscvasynthpod

USE_DAISYSP_LGPL = 1

# Sources
CPP_SOURCES = main.cpp vasynth.cpp \
	UserInterface.cpp \
	../../Library/UiFramework/Presenters/Root.cpp \
	../../Library/UiFramework/Presenters/NavigationListPageItem.cpp \
	../../Library/UiFramework/Presenters/ListPage.cpp \
	../../Library/UiFramework/Presenters/PotentiometerArrayPage.cpp \
	../../Library/UiFramework/Presenters/PotentiometerArrayPageItem.cpp \
	../../Library/UiFramework/Presenters/TabPage.cpp \
	../../Library/UiFramework/Presenters/TabPageItem.cpp \
	../../Library/UiFramework/Presenters/SettingsListPageItem.cpp \
	../../Library/UiFramework/Presenters/NumericPotentiometerArrayPageItem.cpp \
	../../Library/UiFramework/Presenters/NumericSettingsListPageItem.cpp \
	../../Library/UiFramework/Presenters/OptionsPotentiometerArrayPageItem.cpp \
	../../Library/UiFramework/Presenters/OptionsSettingsListPageItem.cpp \
	../../Library/UiFramework/Utilities/PotentiometerToDisplayValueScaler.cpp \
	../../Library/UiFramework/Views/ListPageView.cpp \
	../../Library/UiFramework/Views/PotentiometerArrayPageView.cpp \
	../../Library/UiFramework/Views/TabPageView.cpp \
	../../Library/UiFramework/ViewAdapters/Ili9341ViewAdapter.cpp \
	../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.cpp \
	../../Hardware/NavigationKeypad/Drivers/NavigationKeypad.cpp \
	../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.cpp


# Library Locations
#LIBDAISY_DIR = ../../libdaisy
#DAISYSP_DIR = ../../DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

