#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "prinfo.h"

#define N 100

int main(void)
{
	struct prinfo ps[N], p;
	int nr = N, pc, i, j, indents[N] = {0};
	/* indents store the next pid has such indent level.
	   If indents[1] = 10, it means process 10 has indent level 1 */

	pc = syscall(223, ps, &nr);

	for (i = 0; i < nr && i < pc; i++) {
		p = ps[i];
		for (j = 0; indents[j] != p.pid; j++)
			printf("\t");
		indents[j] = p.next_sibling_pid;
		indents[j+1] = p.first_child_pid;
		printf("%s,%d,%ld,%d,%d,%d,%ld\n", p.comm, p.pid, p.state,
		       p.parent_pid, p.first_child_pid, p.next_sibling_pid,
		       p.uid);
	}
	return 0;
}
