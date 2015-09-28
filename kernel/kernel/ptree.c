#include <linux/prinfo.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <asm-generic/errno-base.h>

extern rwlock_t tasklist_lock;

int has_child(struct task_struct *p)
{
	return !list_empty(&p->children);
}

int has_next_sibling(struct task_struct *p)
{
	return !list_is_last(&p->sibling, &p->parent->children);
}

struct task_struct *first_child(struct task_struct *p)
{
	return list_first_entry(&p->children, struct task_struct, sibling);
}

struct task_struct *next_sibling(struct task_struct *p)
{
	return list_first_entry(&p->sibling, struct task_struct, sibling);
}

void print_prinfo(struct task_struct *p)
{
	/* this function is for debugging */
	struct prinfo q;
	struct task_struct *itr;
	itr = p;
	while (itr != &init_task) {
		printk("\t");
		itr = itr->real_parent;
	}
	strncpy(q.comm, p->comm, 64);
	q.pid = p->pid;
	q.state = p->state;
	q.parent_pid = p->real_parent->pid;
	q.uid = p->real_cred->uid;
	q.first_child_pid = has_child(p) ? first_child(p)->pid : 0;
	q.next_sibling_pid = has_next_sibling(p) ? next_sibling(p)->pid : 0;
	printk("%s,%d,%ld,%d,%d,%d,%ld\n", q.comm, q.pid, q.state,
	       q.parent_pid, q.first_child_pid, q.next_sibling_pid, q.uid);
}

int copy_prinfo(struct prinfo *dst, struct task_struct *p)
{
	struct prinfo q;
	strncpy(q.comm, p->comm, 64);
	q.pid = p->pid;
	q.state = p->state;
	q.parent_pid = p->real_parent->pid;
	q.uid = p->real_cred->uid;
	q.first_child_pid = has_child(p) ? first_child(p)->pid : 0;
	q.next_sibling_pid = has_next_sibling(p) ? next_sibling(p)->pid : 0;
	
	if (copy_to_user(dst, &q, sizeof(struct prinfo)))
		return -EFAULT;
	return 0;
}

SYSCALL_DEFINE2(ptree, struct prinfo *, buf, int *, nr)
{
	struct task_struct *p;
	int pc = 0; /* pcocess count */
	int check_child, store;

	if (buf == NULL && nr == NULL)
		return -EINVAL;

	read_lock(&tasklist_lock);

	p = &init_task;
	check_child = store = 1;
	while (1) {
		if (store && pc < *nr)
			if (copy_prinfo(&buf[pc++], p))
				return -EFAULT;
		/* check child */
		if (check_child && has_child(p)) {
			p = first_child(p);
			continue;
		}
		/* check sibling */
		if (has_next_sibling(p)) {
			p = next_sibling(p);
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

	if (pc < 1)
		return -EINVAL;
	if (pc < *nr && copy_to_user(nr, &pc, sizeof(int)))
		return -EFAULT;
	return pc;
}
