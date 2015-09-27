#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "prinfo.h"

int main()
{
	struct prinfo p;
	int nr = 0;

	printf("run test\n");
	syscall(223, &p, &nr);
	return 0;
}
