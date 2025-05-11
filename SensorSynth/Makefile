# Project Name
TARGET = sensorsynth

# Sources
CPP_SOURCES = synth.cpp $(shell find ./SynthLib -name "*.cpp")
CXXFLAGS += -I. -I./SynthLib -I$(shell find ./SynthLib -type d) -I../libraries/DaisySP

C_INCLUDES += -I../libraries/DaisySP/Source -I../libraries/libDaisy/src

# Library Locations
LIBDAISY_DIR = ../libraries/libDaisy/
DAISYSP_DIR = ../libraries/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Float support
# Uncomment the following line to enable float support in printf
# LDFLAGS += -u _printf_float
