#include <stdio.h>
#include <stdlib.h>

// readline
#include <readline/readline.h>
#include <readline/history.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

# define PROMPT "cmd"

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

/* Read a string, and return a pointer to it.  Returns NULL on EOF. */
char *rl_gets() {
	/* If the buffer has already been allocated, return the memory
	 to the free pool. */
	if (line_read) {
		free (line_read);
		line_read = (char *)NULL;
	}

	/* Get a line from the user. */
	line_read = readline(ANSI_COLOR_CYAN PROMPT ">" ANSI_COLOR_RESET " ");

	/* If the line has any text in it, save it on the history. */
	if (line_read && *line_read)
		add_history(line_read);

	return (line_read);
}

void cmd_help(char *line) {
	printf("Command\n\n");
	printf("  exit - quit the program\n");
	printf("\n\n");
}

int main(int argc, char **argv) {
	char *line = (char *)NULL;
	
	while (1) {
		line = rl_gets();
		
		// show help text
		if (strcmp(line, "help") == 0) {
			cmd_help(line);
			continue;
		}
		
		// quit
		if (strcmp(line, "exit") == 0)
			return 0;
	}
	
	return 0;
}
