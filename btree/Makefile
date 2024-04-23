###
# btree/Makefile
###

OBJ_PATH=build

INCLUDE_PATH	+= -I .

vpath %.c .

WARNINGS += -Wall
WARNINGS += -Wmissing-field-initializers
WARNINGS += -Wpointer-arith
WARNINGS += -Wuninitialized
WARNINGS += -Winit-self
WARNINGS += -Wstrict-overflow
WARNINGS += -Wundef
# WARNINGS += -Wlogical-op
WARNINGS += -Wshadow

CXXFLAGS  = -g $(WARNINGS) $(DEFINES) -std=c++11

SRC += BinaryTree.cpp
SRC += main.cpp

OBJ_CPP	= $(SRC:.cpp=.o)
OBJ_C	= $(OBJ_CPP:.c=.o)
OBJ_O	= $(OBJ_C:%.o=$(OBJ_PATH)/%.o)

all: $(OBJ_PATH) $(OBJ_PATH)/btree

clean:
	rm -rf build

relink:
	rm -f $(OBJ_PATH)/btree.exe
	make all

$(OBJ_PATH)/btree: $(OBJ_O)
	g++ $(CXXFLAGS)  $^ -o $@

$(OBJ_PATH):
	mkdir $(OBJ_PATH)

###
# Implicit Rules
###
$(OBJ_PATH)/%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE_PATH) $< -o $@

$(OBJ_PATH)/%.o : %.c
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE_PATH) $< -o $@
