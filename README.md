# C starter project (macOS + LLVM tools)

A tiny C project with a Makefile that works out of the box with Apple Clang/LLDB on macOS. Good for teaching and quick demos.

## Prereqs (macOS)
- Install Xcode Command Line Tools:
  ```bash
  xcode-select --install
  ```
- Verify tools:
  ```bash
  clang --version
  lldb -v
  ```

Optional tooling used by some targets:
- `clang-format`, `clang-tidy`, `scan-build`, `llvm-profdata`, `llvm-cov` (usually present with Command Line Tools or full Xcode).

## Layout
```
.
├─ Makefile
├─ include/
│  └─ adder.h
└─ src/
   ├─ adder.c
   └─ main.c
```

## Quick start
```bash
make                    # build Debug
make run ARGS="5 7"     # run with args
make lldb ARGS="5 7"    # open LLDB with args
```

## Other useful targets
```bash
make release            # optimized build
make asan               # Address/UBSan build (good for catching bugs)
make format             # clang-format (if installed)
make tidy               # clang-tidy (if installed)
make scan               # static analyzer (scan-build)
make cov                # build with coverage
make cov-run ARGS="5 7" # run instrumented binary
make cov-report         # HTML coverage report in build/Debug/coverage-html
make clean              # remove build/
```

## LLDB cheat sheet
```bash
lldb -- build/Debug/tmpc -- 5 7
(lldb) breakpoint set --name add
(lldb) run
(lldb) bt
(lldb) frame variable
(lldb) next
(lldb) continue
(lldb) quit
```

## Notes
- This uses **C17** and sticks to C (not C++), so no libc++ issues.
- For classroom use, copy this folder and have students tweak `main.c` and add modules.

## `compile_commands.json`
Generate a compilation database (useful for clangd, clang-tidy, etc.). The Makefile provides a helper target that tries a few tools in order.

### Option A: Bear (recommended for Make builds)
```bash
brew install bear
make compdb
```

### Option B: intercept-build (part of scan-build / LLVM extras)
```bash
brew install llvm
make compdb
```

### Option C: compiledb (Python tool)
```bash
pipx install compiledb   # or: pip install compiledb
make compdb
```

After running, you'll have `./compile_commands.json` in the project root.
## compile_commands.json
To generate the database for clangd/clang-tidy:
```bash
brew install bear     # recommended
make compile_commands # uses bear if present; falls back to CMake exporter
```
The `compile_commands.json` included here is portable and uses relative paths, but you can regenerate it anytime with the command above.
