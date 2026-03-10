#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

static volatile int	g_time_out = 0;

void	alarm_handler(int i)
{
	(void) i;

	g_time_out = 1;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status;
	struct sigaction action;
	int		return_val;

	if (!f)
		return (-1);

	return_val = 0;
	
	action.sa_handler = alarm_handler;
	action.sa_flags = 0;
	sigfillset (&action.sa_mask);

	pid = fork ();
	if (pid == -1)
		return (-1);

	if (pid == 0)
	{
		f ();
		exit (0);
	}
	
	if (sigaction (SIGALRM, &action, NULL) == -1)
		return (-1);

	alarm (timeout);

	while (waitpid (pid, &status, 0) == -1 && errno == EINTR)
	{
		if (g_time_out == 1)
		{
			if (kill (pid, SIGKILL) == -1)
				return_val = -1;
			if (verbose)
				printf ("Bad function: timed out after %d seconds\n", timeout);
		}
	}

	alarm (0);
	if (g_time_out)
		return (return_val);

	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		if (verbose)
			printf ("Bad function: exited with code %d\n",
				WEXITSTATUS(status));
		return (0);
	}
	if (WIFSIGNALED(status))
	{
		if (verbose)
			printf ("Bad function: %s\n",
				strsignal (WTERMSIG(status)));
		return (0);
	}

	if (verbose)
		printf ("Nice function!\n");
	return (1);
}

// ______________________________________________________________________________________________________________

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



/*
Assignment name  : sandbox
Expected files   : sandbox.c
Allowed functions: fork, waitpid, exit, alarm, sigaction, kill, printf, strsignal,
errno, sigaddset, sigemptyset, sigfillset, sigdelset, sigismember
--------------------------------------------------------------------------------------

Write the following function:
#include <stdbool.h>
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

This function must test if the function f is a nice function or a bad function

A function is considered bad if
** it is terminated or stopped by a signal (segfault, abort...),
** if it exit with any other exit code than 0 
** or if it times out.

// you will return 1 if f is nice,
// 0 if f is bad
// or -1 in case of an error in your function.

If verbose is true, you must write the appropriate message among the following:
"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal description>\n"
"Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using wait).
We will test your code with very bad functions.
*/