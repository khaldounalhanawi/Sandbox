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
	g_time_out = 0;

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
	{
		kill (pid, SIGKILL);
		waitpid (pid, NULL, 0);
		return (-1);
	}

	alarm (timeout);

	while (waitpid (pid, &status, 0) == -1 && errno == EINTR)
	{
		if (g_time_out == 1)
		{
			if (kill (pid, SIGKILL) == -1)
				return_val = -1;
			if (verbose)
				printf ("Bad function: timed out after %u seconds\n", timeout);
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
