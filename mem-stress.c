#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#if 0
#include "c-utilities/humanize_number/humanize_number.h"
#endif

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define OneMB ((1024 * 1024))

static int random_number(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

void mask_sigterm_and_sigint(void)
{
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGTERM);
	sigaddset(&mask, SIGINT);
	pthread_sigmask(SIG_BLOCK, &mask, NULL);
}

void handle_sigint(int signal, siginfo_t *x, void *y)
{
	exit(1);
}

void handle_sigterm(int signal, siginfo_t *x, void *y)
{
	exit(0);
}

void set_signal_handler(int signo, void (*handler)(int, siginfo_t *, void *))
{
        struct sigaction action;

        action.sa_flags = SA_SIGINFO; 
        action.sa_sigaction = handler;

        if (sigaction(signo, &action, NULL) == -1) { 
		perror("sigaction");
		_exit(1);
        }
}

static void *thread_start(void *arg)
{
	mask_sigterm_and_sigint();
	void *p;

	if ((p = malloc(OneMB * 1024)) == NULL) {
		perror("malloc");
		exit(1);
	}

	do {
		for (int i = 0; i < OneMB; i++) {
			((char *)p)[random_number(1, OneMB)-1] ^= 0x55;
		}
		sleep(5);
	} while (1);
}

static void oom_kill_disable() {
	FILE *fp = fopen("/proc/self/oom_score_adj", "w");

	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	if (write(fileno(fp), "-1000", 5) != 5) {
		perror("fopen");
		exit(1);
	}

	fclose(fp);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "usage: mem-stress <n-threads>\n");
		exit (1);
	}

	set_signal_handler(SIGINT, handle_sigint);
	set_signal_handler(SIGTERM, handle_sigterm);
#if 0
	oom_kill_disable();
#endif	
	for (int i = 0; i < atoi(argv[1]); i++) {
		pthread_t thrid;
		pthread_create(&thrid, NULL, &thread_start, argv[1]);
	}

	do {
		sleep(UINT_MAX);
	} while (1);
}
