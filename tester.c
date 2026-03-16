#include <stdio.h>
#include <stdlib.h>
#include "sandbox.h"

void	test1(void)
{
	printf ("hola, works\n");
}

void	test2(void)
{
	exit (42);
}

void	test3(void)
{
	int	*p = NULL;
	*p = 44;
}

void	test4(void)
{
	while (1);
}

int	main(void)
{
	int	code;

	code = sandbox (test1, 1, true);
	printf ("return val is %d\n\n", code);

	code = sandbox (test2, 1, true);
	printf ("return val is %d\n\n", code);

	code = sandbox (test3, 1, true);
	printf ("return val is %d\n\n", code);

	code = sandbox (test4, 1, true);
	printf ("return val is %d\n\n", code);

	return (0);
}
