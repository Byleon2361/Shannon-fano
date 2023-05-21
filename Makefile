APP_NAME = Shannon
LIB_NAME = libShannon
TEST_NAME = main_test

CFLAGS = -Wall -g -O0
DOPFLAGS = -I src -MMD
TESTFLAGS = -I thirdparty
CC = gcc



BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = test

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a
TEST_PATH = $(BIN_DIR)/$(TEST_NAME)

SRC_EXT = c

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

TEST_SOURCES = $(shell find $(TEST_DIR) -name '*.$(SRC_EXT)')
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(TEST_DIR)/%.o)

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

.PHONY: all test clean
all: $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DOPFLAGS) -c -o $@ $< -lm

test: $(LIB_PATH) $(TEST_PATH)
	$(TEST_PATH)

$(TEST_PATH): $(TEST_OBJECTS) $(LIB_PATH)
	$(CC) $(TESTFLAGS) $(CFLAGS) $(DOPFLAGS) -o $@ $^ -lm

$(OBJ_DIR)/test/main.o: test/main.c
	$(CC) $(TESTFLAGS) $(CFLAGS) $(DOPFLAGS) -c -o $@ $< -lm

$(OBJ_DIR)/test/tests.o: test/tests.c
	$(CC) $(TESTFLAGS) $(CFLAGS) $(DOPFLAGS) -c -o $@ $< -lm
run:
	./$(APP_PATH)
clean:
	rm -f $(APP_PATH) $(LIB_PATH) $(LIB_OBJECTS) $(APP_OBJECTS) $(DEPS) $(TEST_PATH) $(TEST_OBJECTS)

