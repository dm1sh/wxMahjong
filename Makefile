CXX = clang++
 
PROGRAM = wxMahjong
BDIR = build

OBJECTS := $(addprefix $(BDIR)/,$(patsubst %.cpp,%.o,$(wildcard *.cpp)))

$(BDIR)/%.o: %.cpp
	$(CXX) -c -g `wx-config --cxxflags` -o $@ $<

all: $(BDIR) $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $(BDIR)/$(PROGRAM) $(OBJECTS) `wx-config --libs` -g

$(BDIR):
	mkdir $@

clean:
	rm -f $(BDIR)/*.o $(PROGRAM)

install:
	mkdir -p ~/.wxMahjong/
	cp resources/* -r ~/.wxMahjong/
	cp build/wxMahjong /usr/local/bin/
