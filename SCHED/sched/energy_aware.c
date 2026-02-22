// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/cpu.h>
#include <linux/string.h>
//#include "fair.h"

/* [ADDED] Extern declaration to link with variable defined in fair.c */
extern u32 cpu_energy_coeff[NR_CPUS];

static struct kobject *sched_ka;

static ssize_t coeff_show(struct kobject *kobj,
			  struct kobj_attribute *attr, char *buf)
{
	int cpu = get_cpu(); 
	u32 val = cpu_energy_coeff[cpu];
	put_cpu();
	/* Display as integer (fixed point 1024 = 1.0) */
	return sprintf(buf, "%u\n", val);
}

static ssize_t coeff_store(struct kobject *kobj,
			   struct kobj_attribute *attr,
			   const char *buf, size_t count)
{
	int cpu = get_cpu();
	unsigned int v;
	
	/* [FIXED] Use kstrtouint instead of kstrtod (floating point not allowed in kernel) */
	if (kstrtouint(buf, 10, &v)) {
		put_cpu();
		return -EINVAL;
	}
	
	cpu_energy_coeff[cpu] = v;
	put_cpu();
	return count;
}

static struct kobj_attribute coeff_attr =
	__ATTR(coeff, 0664, coeff_show, coeff_store);

static int __init ea_init(void)
{
	int err;
	sched_ka = kobject_create_and_add("sched_energy", kernel_kobj);
	if (!sched_ka)
		return -ENOMEM;
	
	err = sysfs_create_file(sched_ka, &coeff_attr.attr);
	if (err) {
		kobject_put(sched_ka);
		return err;
	}
	return 0;
}

static void __exit ea_exit(void)
{
	/* [FIXED] Used kobject_put to clean up kobject_create_and_add */
	kobject_put(sched_ka);
}

module_init(ea_init);
module_exit(ea_exit);

MODULE_AUTHOR("you");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Energy-Aware Scheduling interface for CFS");