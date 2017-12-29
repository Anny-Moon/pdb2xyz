CC := g++
OPT:= -O3
CFLAGS := -Wall -std=c++11
PROGRAM := pdb2xyz.out

LDLIB := -Ipdb2xyz_lib/include -Lpdb2xyz_lib/build -lpdb2xyz

LDLIB+= -lcurl

all:
	@echo "Generating executable file..." $(PROGRAM)
	@$(CC) $(OPT) $(CFLAGS) main.cpp -o $(PROGRAM) $(LDLIB)
clean:
	rm $(PROGRAM)