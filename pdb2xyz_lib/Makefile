#   Copyright 2017 Anna Sinelnikova
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.


# Run $make list_of_objects to see
# the rule for compilation only one file.
#
# Anna Sinelnikova
# Uppsala, Sweden 2016

# Compilator name (for example could be mpicc instead)
CC:= g++

# Compilator flags
CFLAGS:= -Wall -std=c++11

# Libraries (can also include path to these libraries as -L/SOME_PATH)
LIB:= -lm -lcurl

# Optimization (-O3 or -O2)
OPT:= -O3

# Directory for executables
BIN_DIR:= .

# Name for exectutable
STATIC_LIB = build/libpdb2xyz.a

# Root directories for .h files
INC_DIR = ./include

# Root directory for .cpp files
SRC_DIR:= ./source

# Directory for Objects
OBJ_DIR:= ./build


#---------------------No need to change the following!-------------------

# Include files are needed fot Includes
INC_FILES:=  $(foreach inc_dir, $(INC_DIR), $(wildcard $(inc_dir)/*h))
INC_FILES += $(foreach inc_dir, $(INC_DIR), $(wildcard $(inc_dir)/*/*h))

# All include directories are needed for Includes
INC_DIRS:= $(dir $(INC_FILES))

# Includes: -I ...
LDLIBS:=$(addprefix -I, $(INC_DIRS))

# Source files
SRC :=  $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp)

# Object files
OBJ := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.cpp=.o)))

# Files names of objects without directories name
#OBJ_PURE := $(notdir $(OBJ))

all: $(STATIC_LIB)

# Linking
$(STATIC_LIB): $(OBJ)
	@echo "Generating static library..." $(notdir $(STATIC_LIB))
#	@$(CC) $(CFLAGS) $(OPT)  $(OBJ) -o $(PROGRAM) $(LIB)
	@ar rcs $(STATIC_LIB) $(OBJ) 

# Compiling rule
define app_compile_template
 
 $(1)_OBJ = $$(addprefix $$(OBJ_DIR)/, $$(notdir $$(patsubst %.cpp, %.o, $(1))))
 $(1)_OBJ_PURE = $$(notdir $$($(1)_OBJ))

$$($(1)_OBJ): $(1)
	@echo "Compiling: " $$(addsuffix .cpp, $$(basename $$($(1)_OBJ_PURE)))
	@$$(CC) $$(CFLAGS) $$(OPT) -c $(1) -o $$($(1)_OBJ) $$(LDLIBS)
endef

# Compiling
$(foreach app, $(SRC), $(eval $(call app_compile_template,$(app))))

# Cleaning
clean:
	@echo "Cleaning: "
	rm -r build/*

list_of_objects:
	@echo $(OBJ)

# For debugging makefile
print:
#	@echo $(LDLIBS)
	@echo $(SRC)
#	@echo $(addprefix ./object/, $(notdir $(patsubst %.cpp, %.o, $(SRC))))

love:
	@echo "not war?"