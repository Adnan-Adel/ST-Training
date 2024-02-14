#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

typedef struct {
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partition_type;
    uint8_t last_chs[3];
    uint32_t lba;
    uint32_t sector_count;
} PartitionEntry;

int main(int argc, char **argv) {
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <device>\n", argv[0]);
        return 1;
    }

    char buf[512];
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1) 
    {
        perror("Error opening device");
        return 1;
    }

    read(fd, buf, 512);

    printf("%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Device",
           "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");

    PartitionEntry *table_entry_ptr = (PartitionEntry *) &buf[446];

    for(int i = 0; i < 4; i++) 
    {
        if(table_entry_ptr[i].partition_type == 0x05 || table_entry_ptr[i].partition_type == 0x0F) 
        {
            // The current entry in the partition table is an Extended Partitio

            char buf_ext[512];
            lseek(fd, table_entry_ptr[i].lba * 512, SEEK_SET);
            read(fd, buf_ext, 512);
            PartitionEntry *ext_table_entry_ptr = (PartitionEntry *) &buf_ext[0];
            for(int j = 0; j < 4; j++) 
            {
                if(ext_table_entry_ptr[j].partition_type != 0x00) 
                {
                    printf("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X\n",
                           argv[1],
                           i * 4 + j + 1,
                           ext_table_entry_ptr[j].status == 0x80 ? '*' : ' ',
                           ext_table_entry_ptr[j].lba,
                           ext_table_entry_ptr[j].lba + ext_table_entry_ptr[j].sector_count - 1,
                           ext_table_entry_ptr[j].sector_count,
                           (uint32_t) (((uint64_t) ext_table_entry_ptr[j].sector_count * 512) / (1024 * 1024 * 1024)),
                           ext_table_entry_ptr[j].partition_type);
                }
            }
        } 
        else if(table_entry_ptr[i].partition_type != 0x00)    
        {
            // The current partition entry is not empty and contains a valid partition.
            // it represents either a primary partition or a logical partition within an extended partition.

            printf("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X\n",
                   argv[1],
                   i + 1,
                   table_entry_ptr[i].status == 0x80 ? '*' : ' ',
                   table_entry_ptr[i].lba,
                   table_entry_ptr[i].lba + table_entry_ptr[i].sector_count - 1,
                   table_entry_ptr[i].sector_count,
                   (uint32_t) (((uint64_t) table_entry_ptr[i].sector_count * 512) / (1024 * 1024 * 1024)),
                   table_entry_ptr[i].partition_type);
        }
    }

    close(fd);
    return 0;
}
