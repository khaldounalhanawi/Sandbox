# Sandbox

A small C utility that runs a function in a forked process, catching crashes, non-zero exits, and timeouts.

## API

```c
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);
```

- **f** — function to execute
- **timeout** — max seconds before it's killed
- **verbose** — print what went wrong (or right)

**Returns:** `1` if the function ran cleanly, `0` if it crashed/exited abnormally, `-1` on error.

## Usage

```c
#include "sandbox.h"

void my_func(void) { /* ... */ }

int main(void)
{
    int result = sandbox(my_func, 2, true);
    return 0;
}
```

## Build

```sh
make        # builds sandbox.a static library
make test   # builds and runs the tester
```

You don't need the tester to use it — `make` alone produces `sandbox.a`, which you can link into your own project.
