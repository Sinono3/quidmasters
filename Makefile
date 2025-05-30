SRC = src/main.cpp\
	src/GameDef.cpp\
	src/Assets.cpp\
	src/assets/Sounds.cpp\
	src/assets/Textures.cpp\
	src/systems/enemy.cpp\
	src/systems/hacks.cpp\
	src/systems/store.cpp\
	src/systems/waves.cpp\
	src/systems/player.cpp\
	src/systems/bullets.cpp\
	src/systems/message.cpp\
	src/systems/waves.cpp\
	src/systems/music.cpp\
	src/draw/fog.cpp\
	src/draw/face.cpp\
	src/draw/game.cpp\
	src/draw/gameOver.cpp\
	src/draw/menu.cpp\
	src/draw/pausedOverlay.cpp\
	src/draw/player.cpp\
	src/draw/store.cpp\
	src/draw/statusbar.cpp\
	src/math/physics.cpp
DEPENDS := $(wildcard *.hpp)
BUILDDIR = build
EXE = build/pou

CXX = g++
GCC = gcc
CXXFLAGS = -Wall -Wextra $(shell pkg-config --cflags sfml-all)
CFLAGS =
CPPFLAGS = --std=c++23 -O3 -march=native
LFLAGS = -L/opt/homebrew/lib $(shell pkg-config --libs sfml-all)

OBJS = $(SRC:%=$(BUILDDIR)/%.o)

all: $(EXE)
run: $(EXE)
	$(EXE)
rebuild: clean $(EXE)
rebuild-run: clean $(EXE)
	$(EXE)
clean:
	rm -rf $(BUILDDIR)
.PHONY: all run clean

$(BUILDDIR)/%.cpp.o: %.cpp $(DEPENDS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(GCC) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LFLAGS)
