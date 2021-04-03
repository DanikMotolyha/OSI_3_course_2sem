#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int row = 5;
	char c;
	int in = open("/home/user/Документы/Lab9/file.txt", O_RDONLY);
	lseek(in, (long)2 * row, SEEK_SET); // SEEK_SET SEEK_CUR SEEK_END
	while(read(in, &c, 1) == 1)
	{
		write(1, &c, 1);
	}
	return 0;
}
