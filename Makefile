# SRC = src/main.cpp src/Face.cpp
SRC = src/Dungeon.cpp src/glad.c src/Shader.cpp src/Model.cpp src/draw.cpp src/Player.cpp
BUILDDIR = build
EXE = build/pou

CXX = g++
GCC = gcc
CXXFLAGS = -Wall -I/opt/homebrew/include/ -Iglad/include
CFLAGS =
CPPFLAGS = --std=c++20 
LFLAGS =-L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -framework OpenGL

OBJS = $(SRC:%=$(BUILDDIR)/%.o)

all: $(EXE)
run: $(EXE)
	$(EXE)
clean:
	rm -rf $(BUILDDIR)
.PHONY: all run clean

$(BUILDDIR)/%.cpp.o: %.cpp src/Map.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(GCC) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LFLAGS)
