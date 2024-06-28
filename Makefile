# SRC = src/main.cpp src/Face.cpp
SRC = src/Dungeon.cpp src/draw.cpp src/Face.cpp src/Store.cpp src/aabb.cpp src/Fog.cpp src/systems/player.cpp src/systems/waves.cpp src/systems/bullets.cpp src/GameSound.cpp
BUILDDIR = build
EXE = build/pou

CXX = g++
GCC = gcc
CXXFLAGS = -Wall -I/opt/homebrew/include/
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

$(BUILDDIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(GCC) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LFLAGS)
