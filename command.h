#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "types.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <readline/readline.h>
#include <readline/history.h>

typedef enum {
				SIM_MODE,
				CMD_MODE
} run_mode_t;

typedef struct {
				char *obj_cmd_name;
				Function *func;
				char *doc;
				const char *cmd_name;
				void *private_data;
} cmd_t;

void init_main_thread_cmd_mode(u64 nr_threads);
void init_sim_thread_cmd_mode(void);
void main_loop(pthread_t *pid, u64 nr_threads);
void try_enter_cmd_mode(int *mode);

extern cmd_t *obj_commands;
extern pthread_barrier_t thread_barrier;

#endif // __COMMAND_H__
