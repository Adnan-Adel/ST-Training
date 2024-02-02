#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	char *buf;
	unsigned int size= 50;
	buf= getcwd(buf, size);

	if(buf == NULL)
	{
		switch(errno)
		{
			case EACCES:
				printf("Permission to read or search a component of the filename was denied.\n");
			break;

			case EFAULT:
				printf("The Pointer Points to a bad address.\n");
                        break;

			case EINVAL:
				/* The size argument is zero and buf is not a null pointer */
				size= 50;
				buf= getcwd(buf, size);
                        break;

                        case ENAMETOOLONG:
				printf("The absolute path size is too long!!\n");
                        break;

			case ENOENT:
				printf("The current working directory has been unlinked!!\n");
                        break;

                        case ENOMEM:
				printf("Out of memory!!\n");
                        break;

                        case ERANGE:
				/* the length of the absolute pathname of the current working directory, exceeds size bytes */
				size= 100;
				buf= getcwd(buf, size);
                        break;
		}
	}

	printf("%s\n", buf);
	return 0;
}
