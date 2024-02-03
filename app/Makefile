# Compiler
CXX := g++

# Application name
APP := app

# Source files
SOURCES := main.cpp

# Formatter
FORMATTER := clang-format

# Format style
FORMAT_STYLE := file  # Uses .clang-format file in the current directory

# Targets
.PHONY: all clean format

# Include paths
INCLUDES := -I/opt/homebrew/opt/glew/include -I/opt/homebrew/opt/glfw/include -I/opt/homebrew/opt/glm/include

# Library paths
LIBRARY_PATHS := -L/opt/homebrew/opt/glew/lib -L/opt/homebrew/opt/glfw/lib -L/opt/homebrew/opt/glm/lib

# Libraries to link
LIBRARIES := -lGLEW -lglfw -framework OpenGL

# Compiler flags
CXXFLAGS := -std=c++11 $(INCLUDES)

# Linker flags
LDFLAGS := $(LIBRARY_PATHS) $(LIBRARIES)

# The first rule is the one executed when no parameters are fed into the Makefile
all: $(APP)

# Rule for making the actual target
$(APP): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(APP) $(LDFLAGS)

# Target for formatting
format:
	$(FORMATTER) -i -style=$(FORMAT_STYLE) $(SOURCES)

# Clean up
clean:
	rm -f $(APP)

