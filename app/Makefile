# Compiler
CXX := g++

# Application name
APP := app

# Source files
SOURCES := main.cpp

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

# Clean up
clean:
	rm -f $(APP)

