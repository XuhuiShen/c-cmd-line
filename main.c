#include <stdio.h>
#include <command.h>

int main()
{
				initialize_readline();
				for (;;) {
								cmd_mode();
								printf("Current nr_cycle is %lu\n", nr_cycle);
				}

				return 0;
}
