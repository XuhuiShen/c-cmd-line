#include <stdio.h>
#include <command.h>

#define NR_THREADS 1

typedef struct {
				volatile run_mode_t run_mode;
				u64 current_time;
				u64 current_thread_id;
} thread_state_t;

static pthread_t *g_pids;
__thread thread_state_t g_state;

static inline void siguser1_handler(int sig)
{
				g_state.run_mode = CMD_MODE;
}

static inline void register_sigaction(void)
{
				struct sigaction act;
				act.sa_handler = siguser1_handler;
				act.sa_flags = SA_SIGINFO;

				assert(!sigaction(SIGUSR1, &act, NULL));
}

static void init_thread_state(thread_state_t *s)
{
				s->run_mode = CMD_MODE;
				s->current_time = MIN_TIME;
				s->current_object_id = -1;

				register_sigaction();
				init_thread_cmd_mode();
}

static void thread_worker()
{
				printf("thread run in !\n");
}

static void *thread_main()
{
				thread_state_t *s = &g_state;

				init_thread_state(s);

				while (1) {
								try_enter_cmd_mode((int *)&(g_state.run_mode));
								thread_worker();
				}
}

static void main_loop(pthread_t *pid, u64 nr_threads)
{
				pthread_barrier_wait(&thread_barrier);
				pthread_barrier_wait(&thread_barrier);

				cmd_sig_handler(pid, nr_threads);

				while (1) {
								int sig;
								assert(!sigwait(&cmdset, &sig));
								cmd_sig_handler(pid, nr_threads);
				}
}

static void create_threads(u64 nr_threads)
{
				int i;
				debug("creating %lu threads ... ", nr_threads);
				g_pids = xmalloc(nr_threads * sizeof(*g_pids));
				for (i = 0; i < nr_threads; i++)
								assert(!pthread_create(&g_pids[i], NULL,
																				thread_main, NULL));
				debug("ok\n");
}

int main(int argc, char *argv[])
{
				init_main_thread_cmd_mode(NR_THREADS);

				create_threads(NR_THREADS);

				main_loop(g_pids, NR_THREADS);

				int i;
				for (i = 0; i < NR_THREADS; i++)
								pthread_join(g_pids[i], NULL);

				return 0;
}
