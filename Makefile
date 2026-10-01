CC      := gcc
AR		:= ar
BUILD   := build
LIB		:= build/libyamalloc.a

VERSION ?= $(shell git describe --tags --always --dirty 2>/dev/null || echo dev)

CFLAGS  := -Wall -Wextra -g -std=gnu11 -pedantic -ggdb -DPACK_VERSION='"$(VERSION)"' \
           $(EXTRA_CFLAGS)

CPPFLAGS += -Iinclude -Isrc

LDFLAGS ?=

SRC     := $(wildcard src/*.c)
FMT_SRC := $(wildcard src/*.c src/*.h tests/*.c)
OBJ     := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))
LIB_OBJ := $(filter-out $(BUILD)/main.o,$(OBJ))

.PHONY: all test run clean format check-format

all: $(LIB)

TEST_SRC := $(wildcard tests/*.c)
TEST_BIN := $(patsubst tests/%.c,$(BUILD)/tests/%,$(TEST_SRC))

test: $(TEST_BIN)
	@set -e; for test_bin in $(TEST_BIN); do \
		echo -e "Running $$test_bin:\n..."; \
		./$$test_bin; \
		echo -e "...\n"; \
	done

RUN_ARG := $(if $(filter run,$(firstword $(MAKECMDGOALS))),$(word 2,$(MAKECMDGOALS)))
RUN_FILE := $(if $(filter %.c,$(RUN_ARG)),$(RUN_ARG),$(RUN_ARG).c)
RUN_BIN := $(BUILD)/tests/$(basename $(notdir $(RUN_FILE)))

ifneq ($(RUN_ARG),)
run: $(RUN_BIN)
	./$(RUN_BIN)

.PHONY: $(RUN_ARG)
$(RUN_ARG):
	@:
else
run:
	@echo 'Usage: make run <test-file> (for example: make run smoke.c)'; exit 2
endif

$(BUILD)/tests/%: tests/%.c $(LIB)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) $(LDFLAGS) -o $@

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
