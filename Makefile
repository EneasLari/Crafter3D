CC = g++

GCCFLAGS = -fpcc-struct-return
CFLAGS = -g
ifeq ($(OSTYPE),solaris)
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib/
LDLIBS =  -lGL -lglut -lm
else
INCLUDE = -I/usr/include
# Gia cygwin -L/usr/lib
LDFLAGS = -L/lib/
LDLIBS =  -L. -lSOIL  -lGLU -lglut -lGL -lm 
endif

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) $(PROG).c $(LDFLAGS) $(LDLIBS) -o $(PROG)

clean:
	rm  *.exe
