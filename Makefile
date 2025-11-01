# Simple C starter (macOS, LLVM tools)
# Usage:
#   make                 # build Debug
#   make run ARGS="5 7"
#   make lldb ARGS="5 7" # open LLDB with args
#   make release         # optimized build
#   make asan            # Address/UBSan build
#   make tidy            # run clang-tidy (if installed)
#   make format          # run clang-format (if installed)
#   make scan            # static analysis via scan-build (if installed)
#   make compile_commands  # generate compile_commands.json (bear or CMake)
#   make cov cov-run cov-report  # coverage (llvm-cov) (if installed)
#   make clean

PROJECT       := tmpc
CC            := clang
BUILD_DIR     := build
CONFIG        ?= Debug
BINDIR        := $(BUILD_DIR)/$(CONFIG)
OBJDIR        := $(BUILD_DIR)/obj/$(CONFIG)

SRC           := src/main.c src/adder.c src/mp.c
OBJ           := $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

INCLUDES      := -Iinclude

CSTD          := -std=c17
WARN          := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-unused-parameter
DEBUG_FLAGS   := $(CSTD) $(WARN) -g -O0
RELEASE_FLAGS := $(CSTD) $(WARN) -O2
ASAN_FLAGS    := -fsanitize=address,undefined -fno-omit-frame-pointer

# Allow users to append flags from CLI: make EXTRA_CFLAGS="..."
EXTRA_CFLAGS  ?=

ifeq ($(CONFIG),Debug)
  CFLAGS := $(DEBUG_FLAGS) $(EXTRA_CFLAGS)
else ifeq ($(CONFIG),Asan)
  CFLAGS := $(DEBUG_FLAGS) $(ASAN_FLAGS) $(EXTRA_CFLAGS)
else ifeq ($(CONFIG),Release)
  CFLAGS := $(RELEASE_FLAGS) $(EXTRA_CFLAGS)
else
  $(error Unknown CONFIG '$(CONFIG)'; use Debug/Asan/Release)
endif

TARGET := $(BINDIR)/$(PROJECT)

.PHONY: all debug release asan run lldb tidy format scan clean help cov cov-run cov-report compile_commands

all: $(TARGET)

# Dirs
$(BINDIR) $(OBJDIR):
	@mkdir -p $@

# Compile
$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo [CC] $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link
$(TARGET): $(BINDIR) $(OBJ)
	@echo [LD] $@
	@$(CC) $(CFLAGS) $(OBJ) -o $@

debug:
	@$(MAKE) CONFIG=Debug all

release:
	@$(MAKE) CONFIG=Release all

asan:
	@$(MAKE) CONFIG=Asan all

run: all
	@$(TARGET) $(ARGS)

# Launch LLDB (you can pass ARGS="...")
lldb: all
	@lldb -- $(TARGET) $(ARGS)

# Optional tooling
format:
	@command -v clang-format >/dev/null 2>&1 && clang-format -i $(SRC) include/*.h || \
	  echo "clang-format not found; skipping"

tidy:
	@command -v clang-tidy >/dev/null 2>&1 || { echo "clang-tidy not found"; exit 1; }
	# clang-tidy works best with a compile database; for this small project we pass flags inline
	@clang-tidy $(SRC) -- $(INCLUDES) $(CFLAGS)

scan:
	@command -v scan-build >/dev/null 2>&1 || { echo "scan-build not found"; exit 1; }
	@scan-build -o $(BUILD_DIR)/scan $(MAKE) clean all

# Coverage (LLVM): build with instrumentation, run, then report
COV_FLAGS := -fprofile-instr-generate -fcoverage-mapping
cov:
	@$(MAKE) clean
	@$(MAKE) CONFIG=Debug EXTRA_CFLAGS="$(COV_FLAGS)" all

cov-run:
	@LLVM_PROFILE_FILE="$(BINDIR)/coverage.profraw" $(TARGET) $(ARGS)

cov-report:
	@command -v llvm-profdata >/dev/null 2>&1 || { echo "llvm-profdata not found"; exit 1; }
	@command -v llvm-cov >/dev/null 2>&1 || { echo "llvm-cov not found"; exit 1; }
	@llvm-profdata merge -sparse $(BINDIR)/coverage.profraw -o $(BINDIR)/coverage.profdata
	@echo "Generating HTML report in $(BINDIR)/coverage-html"
	@llvm-cov show $(TARGET) -instr-profile=$(BINDIR)/coverage.profdata \
	  -format=html -output-dir=$(BINDIR)/coverage-html -show-branches=count
	@echo "Open $(BINDIR)/coverage-html/index.html"

# Compile database generation (prefer Bear; fallback to CMake's exporter)
compile_commands: compile_commands.json

compile_commands.json:
	@echo "Generating compile_commands.json ..."
	@if command -v bear >/dev/null 2>&1; then \
		echo " -> using bear"; \
		rm -f compile_commands.json; \
		bear -- $(MAKE) clean all >/dev/null; \
	elif command -v cmake >/dev/null 2>&1; then \
		echo " -> using CMake exporter"; \
		cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null; \
		cp build/compile_commands.json .; \
	else \
		echo "Neither 'bear' nor 'cmake' found. Please install one of them."; \
		exit 1; \
	fi

clean:
	@rm -rf $(BUILD_DIR)

help:
	@echo "Targets:"
	@echo "  make [CONFIG=Debug|Release|Asan]    Build (default Debug)"
	@echo "  make run ARGS=\"5 7\"                Run with args"
	@echo "  make lldb ARGS=\"5 7\"               Debug in LLDB"
	@echo "  make tidy|format|scan                Tooling (optional)"
	@echo "  make compile_commands                Generate compile_commands.json"
	@echo "  make cov cov-run cov-report          Coverage (optional)"
	@echo "  make clean                           Clean build artifacts"
