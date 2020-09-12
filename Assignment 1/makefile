CXX=g++
LDFLAGS=-framework OpenGL -framework GLUT
CFLAGS=-D GL_SILENCE_DEPRECATION --std=c++11
SRCS=main.cpp

out: $(SRCS)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $<

clean:
	rm -rf out