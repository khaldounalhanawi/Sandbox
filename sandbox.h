#ifndef SANDBOX_H
# define SANDBOX_H

#include <stdbool.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

#endif