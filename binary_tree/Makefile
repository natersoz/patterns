###
# binary_tree/Makefile
###

NO_ECHO		?= @

TARGET_NAME	= test_binary_tree

OBJ_PATH=_build

INCLUDE_PATH	+= -I .

WARNINGS += -Wall
WARNINGS += -Wmissing-field-initializers
WARNINGS += -Wpointer-arith
WARNINGS += -Wuninitialized
WARNINGS += -Winit-self
WARNINGS += -Wstrict-overflow
WARNINGS += -Wundef
#WARNINGS += -Wlogical-op
WARNINGS += -Wshadow

CXXFLAGS  = -g $(WARNINGS) $(DEFINES) -std=c++2a
CFLAGS    = -g $(WARNINGS) $(DEFINES) -std=c99

SRC += test_binary_tree.cc

OBJ_CXX	= $(SRC:.cc=.o)
OBJ_C	= $(OBJ_CXX:.c=.o)
OBJ_O	= $(OBJ_C:%.o=$(OBJ_PATH)/%.o)
DEPS	= $(OBJ_C:%.o=$(OBJ_PATH)/%.dep)

all: $(OBJ_PATH) $(OBJ_PATH)/$(TARGET_NAME)
	$(OBJ_PATH)/$(TARGET_NAME)

clean:
	rm -rf $(OBJ_PATH)

relink:
	rm -f $(OBJ_PATH)/$(TARGET_NAME)
	make all

info:
	@echo "DEFINES           = '$(DEFINES)'"
	@echo "SRC               = '$(SRC)'"
	@echo "DEPS              = '$(DEPS)'"
	@echo "INCLUDE_PATH      = '$(INCLUDE_PATH)'"

$(OBJ_PATH)/$(TARGET_NAME): $(OBJ_O)
	@echo "Linking $@"
	$(NO_ECHO) g++ $(CXXFLAGS)  $^ -o $@

$(OBJ_PATH):
	mkdir $(OBJ_PATH)

###
# Implicit Rules
###
$(OBJ_PATH)/%.o : %.cc
	@echo "Compiling $@"
	$(NO_ECHO) $(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) $< -o $@
	$(NO_ECHO) $(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) -MM -MT $@ -MF $(@:.o=.dep) $<

$(OBJ_PATH)/%.o : %.c
	@echo "Compiling $@"
	$(NO_ECHO) $(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
	$(NO_ECHO) $(CC) -c $(CFLAGS) $(INCLUDE_PATH) -MM -MT $@ -MF $(@:.o=.dep) $<


-include $(DEPS)
