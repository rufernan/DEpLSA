CC = g++
LIBINCLUDE = `pkg-config --cflags opencv`
CFLAGS = -Wall -O3 -fopenmp -I$(LIBINCLUDE)
CLINK = -Wall -fopenmp `pkg-config --libs opencv`

EXE = pLSA
OBJS = data.o plsa.o main.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(CLINK) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(EXE)
