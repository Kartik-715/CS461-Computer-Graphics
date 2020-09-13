CXX=g++

OS := $(shell uname)
ifeq ($(OS),Darwin)
  # Run MacOS commands
  LDFLAGS=-framework OpenGL -framework GLUT
  CFLAGS=-D GL_SILENCE_DEPRECATION --std=c++11
else
  LDFLAGS=-lGL -lGLU -lglut
  CFLAGS=--std=c++11
endif

SRCS=main.cpp

out: $(SRCS)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $<

clean:
	rm -rf out