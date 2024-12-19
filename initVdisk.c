#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXFileName 15

// Student structure definition
typedef struct {
    long id;           
    char full_name[40];   
    char mat[20];         
    char section;      
}Student;
typedef struct{
    int n_blocks;
    int f_blockage;
}FileData;
// Structure to represent a file in the menu files table
typedef struct {
    char file_name[MAXFileName];
    // file start block
    int first_block;
    // Number of blocks per file (size)
    int num_blocks;
    // Number of records in the file   
    int block_count;         
    // Organisation mode (0,1....n for each mode) used by org mode team
    int org_mode_intern;     
    // Organisation mode (0,1....n for each mode) used by org mode team
    int org_mode_extern;
} FileMetaData;

// Function to create the vdisk and organisation part
void create_vdisk() {
    int num_blocks, block_factor,max_files, i;
    FileData myfdata;
    FILE *vdisk_file;
    
    printf("Enter the number of blocks in the vdisk: ");
    scanf("%d", &num_blocks);

    printf("Enter the block factor value: ");
    scanf("%d", &block_factor);

    // In worst case, each file contains 1 block, so max_files = num_blocks
    max_files = num_blocks;  // 1 file per block
    myfdata.n_blocks=num_blocks;
    myfdata.f_blockage=block_factor;
    // Create and open the binary file
    vdisk_file = fopen("vdisk.bin", "wb");
    if (vdisk_file == NULL) {
        perror("Error opening the vdisk file");
        return;
    }

    //Write the Binary Table (Organisation Part)
    unsigned char *block_status = (unsigned char *)calloc(num_blocks, sizeof(unsigned char));
    if (block_status == NULL) {
        perror("Memory allocation failed for block status table");
        fclose(vdisk_file);
        return;
    }

    // Fill the binary table: All blocks are initially empty
    for (i = 0; i < num_blocks; i++) {
        // 0 means the block is empty
        block_status[i] = 0; 
    }

    // Write table to vdisk 
    fwrite(&myfdata,sizeof(myfdata),1,vdisk_file);
    fwrite(block_status, sizeof(unsigned char), num_blocks, vdisk_file);
    free(block_status);

    //Write the Menu Files Table (Metadata for Files)
    FileMetaData *file_table = (FileMetaData *)malloc(max_files * sizeof(FileMetaData));
    if (file_table == NULL) {
        perror("Memory allocation failed for file table");
        fclose(vdisk_file);
        return;
    }

    // Initially, all file metadata will be empty (0 values)
    for (i = 0; i < max_files; i++) {
        strncpy(file_table[i].file_name, "Unnamed", MAXFileName - 1);
        file_table[i].first_block = -1;  
        file_table[i].num_blocks = 0;  
        file_table[i].block_count = 0;
        file_table[i].org_mode_extern = 0;
        file_table[i].org_mode_intern = 0;
    }

    //Write the file metadata (menu table) to the file
    fwrite(file_table, sizeof(FileMetaData), max_files, vdisk_file);
    free(file_table);
    // Close the binary file
    fclose(vdisk_file);
    printf("vdisk.bin created successfully with organisation and menu tables.\n");
}

int check_vdisk(const char *filename) {
    FILE *file = fopen(filename, "rb"); 
    if (file != NULL) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}
int main() {
    if(!check_vdisk("vdisk.bin")){
        create_vdisk();
    }
    return 0;
}
