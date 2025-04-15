# Project Name
TARGET = SensorSynth

# Sources
CPP_SOURCES = SensorSynth.cpp

# Library Locations
LIBDAISY_DIR = ../libraries/libDaisy/
DAISYSP_DIR = ../libraries/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Float support
# Uncomment the following line to enable float support in printf
 LDFLAGS += -u _printf_float