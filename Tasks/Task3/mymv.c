#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>


int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Usage: %s src dst\n", argv[0]);
		return -1;
	}
  
	int ret= rename(argv[1], argv[2]);
	
	if(ret == -1)
	{
		/* An Error Occurred */

	}

	return 0;
}
