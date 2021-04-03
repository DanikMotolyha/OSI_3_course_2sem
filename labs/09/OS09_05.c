#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	char c;
	int rowCount = 0;
	int in = open("/home/user/Документы/Lab9/file.txt", O_RDONLY);
	while(read(in, &c, 1) == 1)
	{
		if (c == '\n')
		{
			rowCount++;
		}
	}
	printf("count: %d\n", rowCount);
	return 0;
}
