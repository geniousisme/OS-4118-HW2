#include <linux/prinfo.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <asm-generic/errno-base.h>

extern rwlock_t tasklist_lock;

SYSCALL_DEFINE2(ptree, struct prinfo *, buf, int *, nr)
{
	struct task_struct *p, *itr;
	struct prinfo q;
	int pc = 0; /* pcocess count */
	int check_child, store;

	printk("ptree is called\n");
	buf = kmalloc_array(*nr, sizeof(struct prinfo), GFP_KERNEL);
	/* TODO handle kmalloc_array error */

	read_lock(&tasklist_lock);

	p = &init_task;
	check_child = store = 1;
	while (1) {
		if (store) {
			itr = p;
			while (itr != &init_task) {
				printk("\t");
				itr = itr->real_parent;
			}
			strncpy(q.comm, p->comm, 64);
			q.pid = p->pid;
			q.state = p-> state;
			q.parent_pid = p->real_parent->pid;
			q.uid = p->real_cred->uid;
			printk("%s,%d,%ld,%d,%ld\n", q.comm, q.pid, q.state, q.parent_pid, q.uid);

			if (++pc <= *nr) {
				/* copy data into buf */
			}
		}		
		/* check children */
		if (check_child && !list_empty(&p->children)) {
			p = list_first_entry(&p->children, struct task_struct, sibling);
			continue;
		}
		/* check sibling */
		if (!list_is_last(&p->sibling, &p->parent->children)) {
			p = list_first_entry(&p->sibling, struct task_struct, sibling);
			check_child = store = 1;
			continue;
		}
		/* go back */
		p = p->parent;
		check_child = store = 0;

		if (p == &init_task)
			break;
	}

	read_unlock(&tasklist_lock);

	if (pc < *nr && copy_to_user(nr, &pc, sizeof(int)))
		return -EFAULT;
	return pc;
}
