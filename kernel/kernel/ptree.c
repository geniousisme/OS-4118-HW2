#include <linux/prinfo.h>
#include <linux/spinlock.h>
#include <linux/syscalls.h>

extern rwlock_t tasklist_lock;

SYSCALL_DEFINE2(ptree, struct prinfo *, buf, int *, nr)
{
	read_lock(&tasklist_lock);
	printk("ptree is called\n");
	read_unlock(&tasklist_lock);
	return 0;
}
