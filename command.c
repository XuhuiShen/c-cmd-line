#include "command.h"
#include "utility.h"
#include <assert.h>
#include <readline/readline.h>
#include <readline/history.h>

static int is_continue = 0;

static const int PRINT_COL = 6;

static int cmd_continue(void);
static int cmd_exit(void);
static int cmd_help(char *cmd, char *params);
static int cmd_run(char *cmd, char *params);
static int cmd_run_to_nr_cycle(char *cmd, char *params);

u64 nr_cycle = 0;

cmd_t commands[] = {
				{ "continue", cmd_continue, "Continue x...", NULL, NULL },
				{ "exit", cmd_exit, "Exit x... ", NULL, NULL },
				{ "help", cmd_help, "Help x ...", NULL, NULL },
				{ "quit", cmd_exit, "quit x ...", NULL, NULL },
				{ "run", cmd_run, "run x... ", NULL, NULL },
				{ "runto", cmd_run_to_nr_cycle, "runto x... ", NULL, NULL },
				{ NULL, NULL, NULL, NULL, NULL }
};

static int cmd_run(char *cmd, char *params)
{
				if (!*params) {
								printf("Please input a cycle!\n");
								return 0;
				}

				nr_cycle += strtol(params, (char **)NULL, 10);
				is_continue = 1;
				return 0;
}

static int cmd_run_to_nr_cycle(char *cmd, char *params)
{
				if (!*params) {
								printf("Please input a run cycle!\n");
								return 0;
				}

				nr_cycle = strtol(params, (char **)NULL, 10);
				is_continue = 1;
				return 0;
}

static int cmd_continue(void)
{
				is_continue = 1;
				return 0;
}

static int cmd_exit(void) {
				exit(0);
}

static int cmd_help(char *cmd, char *params)
{
				int i;
				int printed = 0;

				for (i = 0; commands[i].obj_cmd_name; i++) {
								if (!*params || !strcmp(params, commands[i].obj_cmd_name)) {
												printf("%s\t\t%s.\n", commands[i].obj_cmd_name,
																commands[i].doc);
												printed++;
								}
				}

				if (!printed) {
								printf("No commands match `%s'.  Possibilties are:\n", params);

								for (i = 0; commands[i].obj_cmd_name; i++) {
												if (printed == PRINT_COL) {
																printed = 0;
																printf("\n");
												}

												printf("%s\t", commands[i].obj_cmd_name);
												printed++;
								}

								if (printed)
												printf("\n");
				}

				return (0);
}

static char* dupstr(char *s)
{
				char *r;

				r = xmalloc(strlen(s) + 1);
				strcpy(r, s);
				return (r);
}

static cmd_t* find_command(char *name)
{
				int i;

				for (i = 0; commands[i].obj_cmd_name; i++)
								if (strcmp(name, commands[i].obj_cmd_name) == 0)
												return (&commands[i]);

				return ((cmd_t *)NULL);
}

char* stripwhite(char *string)
{
				char *s, *t;

				for (s = string; whitespace(*s); s++)
								;

				if (*s == 0)
								return s;

				t = s + strlen(s) - 1;
				while (t > s && whitespace(*t))
								t--;
				*++t = '\0';

				return s;
}

int execute_line(char *line)
{
				int i;
				cmd_t *command;
				char *word;

				i = 0;
				while (line[i] && whitespace(line[i]))
								i++;
				word = line + i;

				while (line[i] && !whitespace(line[i]))
								i++;

				if (line[i])
								line[i++] = '\0';

				command = find_command(word);

				if (!command) {
								fprintf(stderr, "%s: No such command.\n", word);
								return (-1);
				}

				while (whitespace(line[i]))
								i++;

				word = line + i;

				return command->func(command->cmd_name, word, command->private_data);
}

static char* command_generator(const char *text, int state)
{
				static int idx1, len;
				char *name;

				if (!state) {
								idx1 = 0;
								len = strlen(text);
				}

				while ((name = commands[idx1].obj_cmd_name)) {
								idx1++;

								if (strncmp(name, text, len) == 0)
												return (dupstr(name));
				}

				return ((char *)NULL);
}

static char **command_completion(const char *text, int start, int end)
{
				char **matches = (char **)NULL;
				if (start == 0)
								matches = rl_completion_matches(text, command_generator);
				return (matches);
}

void initialize_readline(void)
{
				rl_readline_name = "CMD";
				rl_attempted_completion_function = (CPPFunction *)command_completion;
}

void cmd_mode(void)
{
				is_continue = 0;
				while (!is_continue) {
								char *line = readline("CMD > ");
								if (!line)
												break;

								char *s = stripwhite(line);
								if (*s)	{
												add_history(s);
												execute_line(s);
								}

								free(line);
				}
}
