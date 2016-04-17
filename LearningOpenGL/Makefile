INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib

FLAGS = -std=gnu++0x -Wall
CXX = g++
CXXFLAGS = $(FLAGS) $(INCLUDE)
LIBS = -lGL -lGLU -lglfw3 -lGLEW -lm

All: LearningOpenGL

LearningOpenGL: LearningOpenGL.o
	$(CXX) $(CXXFLAGS) -o $@ $(LIBDIR) $< $(LIBS)
