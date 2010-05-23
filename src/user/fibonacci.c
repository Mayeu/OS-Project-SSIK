#include <stdio.h>

void fibonacci(int N)
{
	int i = 2;
	int f0=1, f1=1, f;
	printiln(f1);
	printiln(f1);
	while(i <= N)
	{
		f = f0 + f1;
		f0 = f1;
		f1 = f;
		printiln(f1);
		i++;
	}
}
