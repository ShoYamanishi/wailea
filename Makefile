.DEFAULT_GOAL := all

# Change the following two locations to yours.
GOOGLE_TEST_INC_DIR   = -I./GoogleTest/include
GOOGLE_TEST_LIB_DIR   = -L./GoogleTest/lib

.PHONY:	unit_tests_undirected
.PHONY:	unit_tests_directed
.PHONY: all
.PHONY: install
.PHONY: clean

CC           = c++
LD           = c++
RM           = rm -f

SRC_DIR      = ./src
SRC_DIR_UT   = ./unit_tests

OBJ_DIR_REL  = ./objs_release
OBJ_DIR_DBG  = ./objs_debug
OBJ_DIR_UT   = ./objs_unit_tests

CPPFLAGS     = -Wall -std=c++1y
CPPFLAGS_REL = -fPIC -O3
CPPFLAGS_DBG = -g -O0 -DUNIT_TESTS -DUSE_TEST_GRAPH_GENERATOR

INC          = $(GOOGLE_TEST_INC_DIR) -I. -I./include
CFLAGS       = $(INC)

LDFLAGS      = -shared
TARGET_LIB   = libwailea.so

UNIT_TEST_UNDIRECTED = unit_tester_undirected
UNIT_TEST_DIRECTED   = unit_tester_directed

DIR_GUARD    = @mkdir -p $(@D)

OBJS_UNDIRECTED_REL = $(patsubst %,$(OBJ_DIR_REL)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR)/undirected/*.cpp ))))

OBJS_DIRECTED_REL = $(patsubst %,$(OBJ_DIR_REL)/directed_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR)/directed/*.cpp ))))


OBJS_UNDIRECTED_DBG = $(patsubst %,$(OBJ_DIR_DBG)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR)/undirected/*.cpp ))))

OBJS_DIRECTED_DBG = $(patsubst %,$(OBJ_DIR_DBG)/directed_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR)/directed/*.cpp ))))

OBJS_UNDIRECTED_UT = $(patsubst %,$(OBJ_DIR_UT)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_UT)/undirected/*.cpp ))))

OBJS_DIRECTED_UT = $(patsubst %,$(OBJ_DIR_UT)/directed_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_UT)/directed/*.cpp ))))

OBJ_DIRECTED_BASE = $(patsubst %,$(OBJ_DIR_DBG)/directed_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR)/directed/di_base.cpp ))))


OBJ_UNDIRECTED_BASE = $(patsubst %,$(OBJ_DIR_DBG)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR)/undirected/base.cpp ))))

OBJ_UNDIRECTED_CONN = $(patsubst %,$(OBJ_DIR_DBG)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR)/undirected/connected_decomposer.cpp ))))

$(OBJ_DIR_REL)/undirected_%.o: $(SRC_DIR)/undirected/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CPPFLAGS_REL) -c $< -o $@

$(OBJ_DIR_DBG)/undirected_%.o: $(SRC_DIR)/undirected/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(OBJ_DIR_REL)/directed_%.o: $(SRC_DIR)/directed/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CPPFLAGS_REL) -c $< -o $@

$(OBJ_DIR_DBG)/directed_%.o: $(SRC_DIR)/directed/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(OBJ_DIR_UT)/undirected_%.o: $(SRC_DIR_UT)/undirected/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(OBJ_DIR_UT)/directed_%.o: $(SRC_DIR_UT)/directed/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(TARGET_LIB):	$(OBJS_UNDIRECTED_REL) $(OBJS_DIRECTED_REL)
	$(LD) $(LDFLAGS) -o $@ $^

$(UNIT_TEST_UNDIRECTED): $(OBJS_UNDIRECTED_DBG) $(OBJ_DIRECTED_BASE) $(OBJS_UNDIRECTED_UT)
	$(LD) $(GOOGLE_TEST_LIB_DIR) -lgtest -lgtest_main -lstdc++ $^ -o $@

$(UNIT_TEST_DIRECTED): $(OBJS_DIRECTED_DBG) $(OBJ_UNDIRECTED_BASE) $(OBJ_UNDIRECTED_CONN) $(OBJS_DIRECTED_UT)
	$(LD) $(GOOGLE_TEST_LIB_DIR) -lgtest -lgtest_main -lstdc++ $^ -o $@

unit_tests_undirected:	$(UNIT_TEST_UNDIRECTED)
	./$^

unit_tests_directed:	$(UNIT_TEST_DIRECTED)
	./$^

all:	$(TARGET_LIB)

clean:
	-$(RM) $(TARGET_LIB) $(UNIT_TEST_UNDIRECTED) $(UNIT_TEST_DIRECTED) \
               $(OBJ_DIR_REL)/*.o $(OBJ_DIR_DBG)/*.o $(OBJ_DIR_UT)/*.o

install:
	@echo "Currently we don't support install. Grab ./libwailea.so and put it anywhere you like."
