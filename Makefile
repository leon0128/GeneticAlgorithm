CXX           = g++-9 -std=c++17
CXXFLAGS      = -g3 -Wall 
LIBS          = `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lpthread
OBJS          = $(patsubst %.cpp, %.o, $(wildcard */*.cpp)) $(patsubst %.cpp, %.o, $(wildcard */*/*.cpp))
PROGRAM       = main

all:            $(PROGRAM)

$(PROGRAM):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:;         rm -f *.o */*.o */*/*.o $(PROGRAM)
