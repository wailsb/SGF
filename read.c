#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXFileName 15
// Structure to represent a file in the menu files table
typedef struct{
    int n_blocks;
    int f_blockage;
}FileData;
typedef struct {
    char file_name[MAXFileName];  // File name (max length 14, plus null terminator)
    int first_block;              // File start block
    int num_blocks;               // Number of blocks per file (size)
    int block_count;                // Number of records in the file   
    int org_mode_intern;          // Internal organisation mode
    int org_mode_extern;          // External organisation mode
} FileMetaData;

// Function to read and print the organization part of vdisk
void read_org_part(const char *filename) {
    int num_blocks;
    int block_factor;
    FILE *vdisk_file;
    FileData myfdata;
    unsigned char *block_status;
    FileMetaData *file_table;
    int i, max_files;

    // Open the vdisk binary file
    vdisk_file = fopen(filename, "rb");
    if (vdisk_file == NULL) {
        perror("Error opening the vdisk file");
        return;
    }
    fread(&myfdata,sizeof(myfdata),1,vdisk_file);
    num_blocks=myfdata.n_blocks;
    block_factor=myfdata.f_blockage;
    // Read the Binary Table (Organization Part): block status table
    block_status = (unsigned char *)malloc(num_blocks * sizeof(unsigned char)); // Allocate memory for block status table

    if (block_status == NULL) {
        perror("Memory allocation failed for block status table");
        fclose(vdisk_file);
        return;
    }

    // Read the block status table (first part of the vdisk)
    printf("block n:%d block fact :%d :\n",num_blocks,block_factor);
    
    fread(block_status, sizeof(unsigned char), num_blocks , vdisk_file);

    // Print the block status
    printf("Block Status (0 = empty, 1 = occupied):\n");
    for (i = 0; i < num_blocks; i++) {
        
        printf("Block %d: %s\n", i, *(block_status+i) == 0 ? "empty" : "occupied");
    }
    file_table=(FileMetaData *)malloc(num_blocks*sizeof(FileMetaData));
    fread(file_table,sizeof(FileMetaData),num_blocks,vdisk_file);
    // Print the menu table
    printf("Menu data:\n");
    for (i = 0; i < num_blocks; i++) {
        
        printf("File : %d\n\tName :%s\n\tFirst block :%d\n\tSize in blocks :%d\n\tRecord count :%d\n", i, (file_table+i)->file_name,(file_table+i)->first_block,(file_table+i)->num_blocks,(file_table+i)->block_count);
    }

}
int main() {
    const char *filename = "vdisk.bin";  // Define the file name

    // Read and print the organization part of vdisk
    read_org_part(filename);

    return 0;
}