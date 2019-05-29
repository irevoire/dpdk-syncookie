#ifndef _UTILS_H_
#define _UTILS_H_

/* Per-port statistics struct */
struct l2fwd_port_statistics {
	uint64_t tx;
	uint64_t rx;
	uint64_t dropped;
} __rte_cache_aligned;

extern struct l2fwd_port_statistics port_statistics[RTE_MAX_ETHPORTS];

void
print_stats(void);

#endif /* _UTILS_H_ */
