.DEFAULT_GOAL := all

# Change the following two locations to yours.
GOOGLE_TEST_INC_DIR   = -I./GoogleTest/include
GOOGLE_TEST_LIB_DIR   = -L./GoogleTest/lib

INSTALL_DIR_LIB = /usr/local/lib
INSTALL_DIR_BIN = /usr/local/bin
INSTALL_DIR_INC = /usr/local/include

.PHONY:	unit_tests_undirected
.PHONY:	unit_tests_directed
.PHONY: all
.PHONY: install
.PHONY: clean

CC           = c++
LD           = c++
RM           = rm -f
RMR          = rm -fr
CP           = cp
MKDIR        = mkdir -p

PWD          =  $(shell pwd)
SRC_DIR_LIB  = ./src_lib
SRC_DIR_UT   = ./unit_tests
SRC_DIR_BIN  = ./src_bin
BIN_DIR      = ./bin
LIB_DIR      = ./libs

OBJ_DIR_REL  = ./objs_release
OBJ_DIR_DBG  = ./objs_debug
OBJ_DIR_UT   = ./objs_unit_tests

CPPFLAGS     = -Wall -std=c++1y
CPPFLAGS_REL = -fPIC -O3
CPPFLAGS_DBG = -g -O0 -DUNIT_TESTS -DUSE_TEST_GRAPH_GENERATOR
CPPFLAGS_BIN = -L./libs

CFLAGS_DBG   = $(GOOGLE_TEST_INC_DIR) -I. -I./include
CFLAGS_REL   = -I. -I./include

LDFLAGS      = -shared
TARGET_LIB   = $(LIB_DIR)/libwailea.so


UNIT_TEST_UNDIRECTED = unit_tester_undirected
UNIT_TEST_DIRECTED   = unit_tester_directed

DIR_GUARD    = @mkdir -p $(@D)

OBJS_UNDIRECTED_REL = $(patsubst %,$(OBJ_DIR_REL)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_LIB)/undirected/*.cpp ))))

OBJS_DIRECTED_REL = $(patsubst %,$(OBJ_DIR_REL)/directed_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_LIB)/directed/*.cpp ))))


OBJS_UNDIRECTED_DBG = $(patsubst %,$(OBJ_DIR_DBG)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_LIB)/undirected/*.cpp ))))

OBJS_DIRECTED_DBG = $(patsubst %,$(OBJ_DIR_DBG)/directed_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_LIB)/directed/*.cpp ))))

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
                               $(SRC_DIR_LIB)/directed/di_base.cpp ))))

OBJ_UNDIRECTED_BASE = $(patsubst %,$(OBJ_DIR_DBG)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_LIB)/undirected/base.cpp ))))

OBJ_UNDIRECTED_CONN = $(patsubst %,$(OBJ_DIR_DBG)/undirected_%, \
                         $(subst .cpp,.o, \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_LIB)/undirected/connected_decomposer.cpp ))))

BIN_LIST            = $(patsubst %,$(BIN_DIR)/%, \
                         $(basename \
                           $(notdir \
                             $(wildcard \
                               $(SRC_DIR_BIN)/*.cpp ))))


$(OBJ_DIR_REL)/undirected_%.o: $(SRC_DIR_LIB)/undirected/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS_REL) $(CPPFLAGS) $(CPPFLAGS_REL) -c $< -o $@

$(OBJ_DIR_DBG)/undirected_%.o: $(SRC_DIR_LIB)/undirected/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS_DBG) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(OBJ_DIR_REL)/directed_%.o: $(SRC_DIR_LIB)/directed/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS_REL) $(CPPFLAGS) $(CPPFLAGS_REL) -c $< -o $@

$(OBJ_DIR_DBG)/directed_%.o: $(SRC_DIR_LIB)/directed/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS_DBG) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(OBJ_DIR_UT)/undirected_%.o: $(SRC_DIR_UT)/undirected/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS_DBG) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(OBJ_DIR_UT)/directed_%.o: $(SRC_DIR_UT)/directed/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS_DBG) $(CPPFLAGS) $(CPPFLAGS_DBG) -c $< -o $@

$(TARGET_LIB):	$(OBJS_UNDIRECTED_REL) $(OBJS_DIRECTED_REL)
	$(DIR_GUARD)
	$(LD) $(LDFLAGS) -o $@ $^

$(BIN_DIR)/%:	$(TARGET_LIB) $(SRC_DIR_BIN)/%.cpp
	$(eval LIBWAILEA=$(notdir $(word 1, $^)))
	$(eval LWAILEA=$(subst lib,-l,$(basename $(LIBWAILEA))))
	$(eval BIN_SRC=$(word 2, $^))
	$(DIR_GUARD)
	$(CC) $(CFLAGS_REL) $(CPPFLAGS) $(CPPFLAGS_REL) $(CPPFLAGS_BIN) $(BIN_SRC) $(LWAILEA) -lstdc++ -o $@

$(UNIT_TEST_UNDIRECTED): $(OBJS_UNDIRECTED_DBG) $(OBJ_DIRECTED_BASE) $(OBJS_UNDIRECTED_UT)
	$(LD) $(GOOGLE_TEST_LIB_DIR) $^ -lgtest -lgtest_main -lstdc++ -o $@

$(UNIT_TEST_DIRECTED): $(OBJS_DIRECTED_DBG) $(OBJ_UNDIRECTED_BASE) $(OBJ_UNDIRECTED_CONN) $(OBJS_DIRECTED_UT)
	$(LD) $(GOOGLE_TEST_LIB_DIR) $^ -lgtest -lgtest_main -lstdc++ -o $@

unit_tests_undirected:	$(UNIT_TEST_UNDIRECTED)
	./$^

unit_tests_directed:	$(UNIT_TEST_DIRECTED)
	./$^

all:	$(TARGET_LIB) $(BIN_LIST)

clean:
	-$(RM) $(TARGET_LIB) $(UNIT_TEST_UNDIRECTED) $(UNIT_TEST_DIRECTED)
	-$(RMR) $(OBJ_DIR_REL) $(OBJ_DIR_DBG) $(OBJ_DIR_UT) $(LIB_DIR) $(BIN_DIR)

install:
	sudo $(CP) $(TARGET_LIB) $(INSTALL_DIR_LIB)
	sudo $(CP) $(BIN_LIST) $(INSTALL_DIR_BIN)
	sudo $(MKDIR) $(INSTALL_DIR_INC)/undirected
	sudo $(MKDIR) $(INSTALL_DIR_INC)/directed
	sudo $(CP) include/undirected/* $(INSTALL_DIR_INC)/undirected
	sudo $(CP) include/directed/*   $(INSTALL_DIR_INC)/directed
