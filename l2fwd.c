#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include "conf.h"
#include "lcore_queue.h"
#include "l2fwd.h"

static const char short_options[] =
"p:"  /* portmask */
"q:"  /* number of queues */
"T:"  /* timer period */
;

/* display usage */
static void
l2fwd_usage(const char *prgname)
{
	printf("%s [EAL options] -- -p PORTMASK [-q NQ]\n"
			"  -p PORTMASK: hexadecimal bitmask of ports to configure\n"
			"  -q NQ: number of queue (=ports) per lcore (default is 1)\n"
			"  -T PERIOD: statistics will be refreshed each PERIOD seconds (0 to disable, 10 default, 86400 maximum)\n",
			prgname);
}

static int
l2fwd_parse_portmask(const char *portmask)
{
	char *end = NULL;
	unsigned long pm;

	/* parse hexadecimal string */
	pm = strtoul(portmask, &end, 16);
	if ((portmask[0] == '\0') || (end == NULL) || (*end != '\0'))
		return -1;

	if (pm == 0)
		return -1;

	return pm;
}

static unsigned int
l2fwd_parse_nqueue(const char *q_arg)
{
	char *end = NULL;
	unsigned long n;

	/* parse hexadecimal string */
	n = strtoul(q_arg, &end, 10);
	if ((q_arg[0] == '\0') || (end == NULL) || (*end != '\0'))
		return 0;
	if (n == 0)
		return 0;
	if (n >= MAX_RX_QUEUE_PER_LCORE)
		return 0;

	return n;
}

#define MAX_TIMER_PERIOD 86400 /* 1 day max */
static int
l2fwd_parse_timer_period(const char *q_arg)
{
	char *end = NULL;
	int n;

	/* parse number string */
	n = strtol(q_arg, &end, 10);
	if ((q_arg[0] == '\0') || (end == NULL) || (*end != '\0'))
		return -1;
	if (n >= MAX_TIMER_PERIOD)
		return -1;

	return n;
}

int
l2fwd_parse_args(int argc, char **argv)
{
	int opt, ret, timer_secs;
	char **argvopt;
	int option_index;
	char *prgname = argv[0];

	argvopt = argv;

	while ((opt = getopt_long(argc, argvopt, short_options,
					NULL, &option_index)) != EOF) {

		switch (opt) {
			/* portmask */
			case 'p':
				l2fwd_enabled_port_mask = l2fwd_parse_portmask(optarg);
				if (l2fwd_enabled_port_mask == 0) {
					printf("invalid portmask\n");
					l2fwd_usage(prgname);
					return -1;
				}
				break;

				/* nqueue */
			case 'q':
				l2fwd_rx_queue_per_lcore = l2fwd_parse_nqueue(optarg);
				if (l2fwd_rx_queue_per_lcore == 0) {
					printf("invalid queue number\n");
					l2fwd_usage(prgname);
					return -1;
				}
				break;

				/* timer period */
			case 'T':
				timer_secs = l2fwd_parse_timer_period(optarg);
				if (timer_secs < 0) {
					printf("invalid timer period\n");
					l2fwd_usage(prgname);
					return -1;
				}
				timer_period = timer_secs;
				break;

				/* long options */
			case 0:
				break;

			default:
				l2fwd_usage(prgname);
				return -1;
		}
	}

	if (optind >= 0)
		argv[optind-1] = prgname;

	ret = optind-1;
	optind = 1; /* reset getopt lib */
	return ret;
}

