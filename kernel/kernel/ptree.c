#include <linux/prinfo.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/syscalls.h>

extern rwlock_t tasklist_lock;

SYSCALL_DEFINE2(ptree, struct prinfo *, buf, int *, nr)
{
	struct task_struct *p, *itr;
	struct prinfo q;

	buf = kmalloc_array(*nr, sizeof(struct prinfo), GFP_KERNEL);
	/* TODO handle kmalloc_array error */

	read_lock(&tasklist_lock);
	printk("ptree is called\n");

	for_each_process(p) {
		itr = p;
		while (itr->pid > 1) {
			printk("\t");
			itr = itr->real_parent;
		}
		strncpy(q.comm, p->comm, 64);
		q.pid = p->pid;
		q.state = p-> state;
		q.parent_pid = p->real_parent->pid;
		
		printk("%s,%d,%ld,%d\n", q.comm, q.pid, q.state, q.parent_pid);
	}

	read_unlock(&tasklist_lock);
	return 0;
}
