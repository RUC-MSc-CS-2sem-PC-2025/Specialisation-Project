# Project Name
TARGET = sensorsynth

# Sources
CPP_SOURCES = main.cpp $(shell find ./SynthLib -name "*.cpp") $(shell find ./Hardware -name "*.cpp")
CXXFLAGS += -I. -I./SynthLib -I$(shell find ./SynthLib -type d)
CXXFLAGS += -I./Hardware -I$(shell find ./Hardware -type d)
CFLAGS += -I../libraries/libDaisy/src


CPP_SOURCES = main.cpp \
              $(shell find ./SynthLib -name "*.cpp") \
              $(shell find ./Hardware -name "*.cpp") \

# Library Locations
LIBDAISY_DIR = ../libraries/libDaisy/
DAISYSP_DIR = ../libraries/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Float support
# Uncomment the following line to enable float support in printf
 LDFLAGS += -u _printf_float


C_INCLUDES += -I../libraries/libDaisy/Drivers/STM32_USB_Device_Library/Class/CDC/Inc
C_INCLUDES += -I../libraries/libDaisy/Drivers/STM32_USB_Device_Library/Core/Inc