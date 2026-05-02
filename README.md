# Power Quality Analyser (C)

Simple C project built with CMake. The build produces the `pq_analyser` executable in the build folder.

## gihub link
https://github.com/yousefalshalahi/pqa.git

## Build

From the project root:

```sh
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

## Run

```sh
cmake-build-debug\pq_analyser
```

## Project layout

- `main.c`: program entry point
- `io.c` / `io.h`: input/output helpers
- `waveform.c` / `waveform.h`: waveform processing logic
