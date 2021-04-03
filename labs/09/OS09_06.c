#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int arg = atoi(argv[1]);
	char c;
	int rowCount1 = 1;
	int rowCount2 = 1;
	int in = open("/home/user/Документы/Lab9/file.txt", O_RDONLY);
	if (arg % 2 == 0)
	{
		int out = open("/home/user/Документы/Lab9/chet.txt", O_RDWR | O_CREAT);
		while(read(in, &c, 1) == 1)
		{
			if (rowCount1 % 2 == 0)
			{
				write(out, &c, 1);
			}
			if (c == '\n')
			{
				rowCount1++;
			}
		}	
	}
	else
	{
		int out = open("/home/user/Документы/Lab9/nechet.txt", O_RDWR | O_CREAT);
		while(read(in, &c, 1) == 1)
		{
			if (rowCount2 % 2 != 0)
			{
				write(out, &c, 1);
			}
			if (c == '\n')
			{
				rowCount2++;
			}
		}
	}
	return 0;
}
