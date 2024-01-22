# Project Name
#TARGET = Test

# Sources
#CPP_SOURCES = \
#	Test.cpp 

# Core location, and generic Makefile.
# SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
# include $(SYSTEM_FILES_DIR)/Makefile
 
test:
	g++ -lstdc++ -o StepSequencerTests -DTESTING \
		Controller.test.cpp \
		../../Drivers/Controller.cpp 

clean:
	rm -f rm -fR build
