#include <stdio.h>
#include <command.h>

int main(int argc, char *argv[])
{
				init_main_thread_cmd_mode();
				for (;;) {
								cmd_mode();
								printf("Current nr_cycle is %lu\n", nr_cycle);
				}

				return 0;
}
