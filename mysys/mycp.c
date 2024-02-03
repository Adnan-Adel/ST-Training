#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{

    int src_fd, dst_fd;
    int  num_read, num_writes;
    unsigned char buf[100];

    if(argc != 3){

        printf("Usage: %s src dst\n", argv[0]);
        return -1;
    }

    
    src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        printf("could not open the source file\n");
        return -1;
    }

    dst_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        printf("could not open the destination file\n");
        return -1;
    }

    while((num_read = read(src_fd, buf, 100)) != 0){

    if (num_read < 0) {
        printf("could not read from the file\n");
        return -1;
    }


    num_writes = write(dst_fd, buf, num_read);

    if (num_writes < 0) {
        printf("could not write to file\n");
        return -1;
    }
   
    }

    close(src_fd);
    close(dst_fd);

    return 0;
}
