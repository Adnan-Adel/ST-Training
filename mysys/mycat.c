#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    
    int fd, num_read, num_writes;
    unsigned char buf[100];
    
    if(argc != 2){
	
	printf("Usage: %s file_name\n", argv[0]);
	return -1;
    }

    fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
	printf("could not open the file\n");
	return -1;
    }

    printf("the file opened with fd= %d\n", fd);

    while((num_read = read(fd, buf, 100)) != 0){

    if (num_read < 0) {
	printf("could not read from the file\n");
	return -1;
    }

    printf(">> I read %d bytes\n", num_read);

    num_writes = write(1, buf, num_read);

    if (num_writes < 0) {
	printf("could not write to stdout\n");
	return -1;
    }
    printf("\n>> I wrote %d bytes\n", num_writes);
    }

    close(fd);

    return 0;
}
