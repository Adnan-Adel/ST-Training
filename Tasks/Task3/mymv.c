#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>


int main(int argc, char** argv)
{
	/*unsigned int num_reads, num_writes;
	int src_fd, dst_fd;
	unsigned char buf[100];
	char* pathbuf;
	int ret;*/

	char* oldpath;
	char* newpath;

	oldpath= getcwd(oldpath, 50);
	newpath= getcwd(newpath, 50);

	if(argc != 3)
	{
		printf("Usage: %s src dst\n", argv[0]);
		return -1;
	}



	/*src_fd= open(argv[1], O_RDONLY);
	if(src_fd < 0)
	{
		printf(">> couldn't open the src file!!\n");
		return -1;
	}

	dst_fd= open(argv[2], O_WRONLY | O_CREAT, 0644);
	if(dst_fd < 0)
	{
		printf(">> couldn't the dest file!!\n");
		return -1;
	}

	while((num_reads= read(src_fd, buf, 100)) != 0)
	{
		if(num_reads < 0)
		{
			printf(">> Could not read from the file!!\n");
           	 	return -1;
		}

		num_writes= write(dst_fd, buf, 100);
		if(num_writes < 0)
                {
                        printf(">> Could not write to the file!!\n");
                        return -1;
                }
	}


	pathbuf= getcwd(pathbuf, 50);
	ret= unlink(pathbuf);
*/


	return 0;
}
