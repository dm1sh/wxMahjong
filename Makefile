# CXX = $(shell wx-config --cxx)
CXX = clang++
 
PROGRAM = wxMahjong
BDIR = build

OBJECTS := $(addprefix $(BDIR)/,$(patsubst %.cpp,%.o,$(wildcard *.cpp)))

$(BDIR)/%.o: %.cpp
	$(CXX) -c `wx-config --cxxflags` -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $(BDIR)/$(PROGRAM) $(OBJECTS) `wx-config --libs`

clean:
	rm -f $(BDIR)/*.o $(PROGRAM)
