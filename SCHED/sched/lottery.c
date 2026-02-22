// SPDX-License-Identifier: GPL-2.0
#include "sched.h"
#include <linux/random.h>
#include <linux/sched.h>
#include <linux/list.h>

/* Lottery Scheduling: pick a task based on a random ticket selection */
static struct task_struct *pick_next_task_lottery(struct rq *rq)
{
	struct task_struct *p;
	unsigned long total_tickets = 0, winner, sum = 0;

	/* Calculate total tickets of all tasks in the run queue */
	list_for_each_entry(p, &rq->cfs_tasks, se.group_node) {
		/* [FIXED] Corrected access to weight (was p->se.weight) */
		total_tickets += p->se.load.weight;
	}

	/* If no task has any tickets, return NULL */
	if (!total_tickets)
		return NULL;

	/* Draw a winner */
	/* [FIXED] Use get_random_u32() instead of deprecated prandom_u32() */
	winner = get_random_u32() % total_tickets;

	/* Find the task that owns the winning ticket */
	list_for_each_entry(p, &rq->cfs_tasks, se.group_node) {
		/* [FIXED] Corrected access to weight */
		sum += p->se.load.weight;
		if (sum > winner)
			return p;
	}
	return NULL;  /* Just in case, should not reach here */
}