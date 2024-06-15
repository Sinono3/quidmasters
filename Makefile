SRC = src/main.cpp
BUILDDIR = build
EXE = build/pou

CXX = g++
CXXFLAGS = -Wall --std=c++20 -I/opt/homebrew/include/
LFLAGS =-L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

OBJS = $(SRC:%=$(BUILDDIR)/%.o)

all: $(EXE)
run: $(EXE)
	$(EXE)
clean:
	rm -rf $(BUILDDIR)
.PHONY: all run clean

$(BUILDDIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LFLAGS)
