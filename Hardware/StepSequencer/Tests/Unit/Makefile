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
		SequencerBrain.test.cpp \
		../../Drivers/SequencerBrain.cpp 

clean:
	rm -f rm -fR build
