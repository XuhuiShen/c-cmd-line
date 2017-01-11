#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "type.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <readline/readline.h>
#include <readline/history.h>

typedef struct {
				char *obj_cmd_name;
				Function *func;
				char *doc;
				const char *cmd_name;
				void *private_data;
} cmd_t;

extern u64 nr_cycle;

void initialize_readline(void);
void cmd_mode(void);

#endif // __COMMAND_H__
