#ifndef _CONF_H_
#define _CONF_H_

#include <stdint.h>

extern uint32_t l2fwd_enabled_port_mask;

extern uint32_t l2fwd_dst_ports[RTE_MAX_ETHPORTS];

extern unsigned int l2fwd_rx_queue_per_lcore;

/* A tsc-based timer responsible for triggering statistics printout */
extern uint64_t timer_period; /* default period is 10 seconds */

#endif /* _CONF_H_ */
