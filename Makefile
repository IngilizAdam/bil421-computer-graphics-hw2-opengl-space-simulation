
LDFLAGS=-L/opt/local/lib -framework Cocoa -framework OpenGL  -framework GLUT  -lGLEW -lm

CC = g++
CFLAGS=-g -I/opt/local/include 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -w

first: first.o initshader.o
	$(CC) $@.o initShader.o $(LDFLAGS) -o $@

clean: 
	-rm -r *.o core *.dSYM
