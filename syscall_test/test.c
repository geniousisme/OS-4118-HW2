#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "prinfo.h"

int main()
{
	struct prinfo p;
	int nr = 10, pc;

	printf("run test\n");
	pc = syscall(223, &p, &nr);
	printf("nr after system call: %d\n", nr);
	printf("total processes count: %d\n", pc);
	return 0;
}
