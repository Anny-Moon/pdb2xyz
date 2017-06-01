CC := g++
OPT:= -O3
CFLAGS := -Wall -std=c++11
PROGRAM := pdb2xyz.out
#LIB:= -I/Users/annsi118/Documents/Git_projects/PCA_private/include -L/Users/annsi118/Documents/Git_projects/PCA_private/build
LDLIB := -I/Users/annasinelnikova/Documents/git_projects/pdb2xyz/pdb2xyz_lib/include -L/Users/annasinelnikova/Documents/git_projects/pdb2xyz/pdb2xyz_lib/build -lpdb2xyz

all:
	@echo "Generating executable file..." $(PROGRAM)
	@$(CC) $(OPT) $(CFLAGS) main.cpp -o $(PROGRAM) $(LDLIB)
clean:
	rm $(PROGRAM)