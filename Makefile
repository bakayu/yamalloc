CC      := gcc
AR		:= ar
BUILD   := build
LIB		:= build/libyamalloc.a

VERSION ?= $(shell git describe --tags --always --dirty 2>/dev/null || echo dev)

CFLAGS  := -Wall -Wextra -g -std=gnu11 -DPACK_VERSION='"$(VERSION)"' \
           $(EXTRA_CFLAGS)

CPPFLAGS += -Iinclude

LDFLAGS ?=

SRC     := $(wildcard src/*.c)
FMT_SRC := $(wildcard src/*.c src/*.h tests/*.c)
OBJ     := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))
LIB_OBJ := $(filter-out $(BUILD)/main.o,$(OBJ))

.PHONY: all clean format check-format

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

format:
	clang-format -i $(FMT_SRC)

check-format:
	clang-format --dry-run -Werror $(FMT_SRC)

clean:
	rm -rf $(BUILD)
