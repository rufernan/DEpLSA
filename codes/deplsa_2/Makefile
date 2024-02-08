CC = g++
LIBINCLUDE = `pkg-config --cflags opencv`
CFLAGS = -Wall -O3 -ltbb -I$(LIBINCLUDE)
CLINK = -Wall -ltbb `pkg-config --libs opencv`

EXE = pLSA
OBJS = data.o plsa.o main.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(CLINK) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(EXE)
