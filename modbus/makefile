# Detect the operating system
UNAME_S := $(shell uname -s)

# Compiler
CXX := g++

# Include directories
INCLUDES := -I../hidapi/hidapi

# Compiler flags
CXXFLAGS := -Wall $(INCLUDES)

# Linker flags
LDFLAGS :=

# Source files
SOURCES := $(wildcard src/*.cpp)

# Object files
OBJECTS := $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE := X12

# Setting up platform-specific variables
ifeq ($(UNAME_S), Linux)
    # Linux specific code
    LDFLAGS += -L../hidapi/build/src/linux/Release -lhidapi
    OS_MESSAGE := "Working on Linux"
    OS_SUPPORTED := yes
endif
ifneq (,$(findstring MINGW,$(UNAME_S)))
    # Windows specific code (MinGW, both 32 and 64-bit)
    LDFLAGS += -L../hidapi/build/src/windows/Release -lhidapi
    OS_MESSAGE := "Working on Windows"
    OS_SUPPORTED := yes
endif

# Check if the OS is supported
ifndef OS_SUPPORTED
    $(error Unsupported Operating System: $(UNAME_S))
endif

all: os_check $(SOURCES) $(EXECUTABLE)

os_check:
	@echo $(OS_MESSAGE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	@echo "Cleaning up..."
	rm -rf src/*.o $(EXECUTABLE)
