#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "prinfo.h"

#define N 100

int main()
{
	struct prinfo p[N], q;
	int nr = N, pc, i, j, indents[N] = {0};

	pc = syscall(223, p, &nr);

	for (i = 0; i < nr && i < pc; i++) {
		q = p[i];
		j = 0;
		while (indents[j] != q.pid && j < 5) {
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
