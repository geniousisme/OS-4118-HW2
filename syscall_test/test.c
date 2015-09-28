#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "prinfo.h"

#define N 100

int main(void)
{
	struct prinfo p[N], q;
	int nr = N, pc, i, j, indents[N] = {0};
	/* indents store the next pid has such indent level.
	   If indents[1] = 10, it means process 10 has indent level 1 */

	pc = syscall(223, p, &nr);

	for (i = 0; i < nr && i < pc; i++) {
		q = p[i];
		j = 0;
		while (indents[j] != q.pid) {
			printf("\t");
			j++;
		}
		indents[j] = q.next_sibling_pid;
		indents[j+1] = q.first_child_pid;
		printf("%s,%d,%ld,%d,%d,%d,%ld\n", q.comm, q.pid, q.state,
		       q.parent_pid, q.first_child_pid, q.next_sibling_pid,
		       q.uid);
	}
	return 0;
}
