#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/sched/signal.h> 
#include <linux/rcupdate.h>

#define PROC_NAME "sched_log"

static int show_log(struct seq_file *m, void *v)
{
    struct task_struct *p;
    seq_printf(m, "PID\tComm\tCtxSw\tRunTime(ns)\n");
    rcu_read_lock();
    for_each_process(p) {
        seq_printf(m, "%d\t%s\t%llu\t%llu\n",
                   p->pid, p->comm,
                   p->sched_log.ctx_switches,
                   p->sched_log.total_runtime_ns);
    }
    rcu_read_unlock();
    return 0;
}

static int open_log(struct inode *inode, struct file *file)
{
    return single_open(file, show_log, NULL);
}

static const struct proc_ops fops = {
    .proc_open    = open_log,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init log_init(void)
{
    if (!proc_create(PROC_NAME, 0444, NULL, &fops))
        return -ENOMEM;
    return 0;
}

static void __exit log_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(log_init);
module_exit(log_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("you");
MODULE_DESCRIPTION("Scheduler logging for process runtime and context switches");

