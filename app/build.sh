#!/usr/bin/env bash

g++ main.cpp -o app -I/opt/homebrew/opt/glew/include -I/opt/homebrew/opt/glfw/include -L/opt/homebrew/opt/glew/lib -L/opt/homebrew/opt/glfw/lib -lGLEW -lglfw -framework OpenGL


