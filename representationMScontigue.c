#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXFileName 15

// Structure to represent the disk organization information
typedef struct {
    int n_blocks;     // Number of blocks in the vdisk
    int f_blockage;   // Block factor (how many records per block)
} FileData;

// Structure for the metadata of each file on the virtual disk
typedef struct {
    char file_name[MAXFileName];  // File name
    int first_block;              // The first block where the file starts
    int num_blocks;               // Number of blocks used by the file
    int block_count;              // Number of records in the file
    int org_mode_intern;          // Internal organization mode
    int org_mode_extern;          // External organization mode
} FileMetaData;
typedef struct {
    long id;           
    char full_name[40];   
    char mat[20];         
    char section;      
}Student;
// Function to write data to the vdisk based on organization and file structure
void write_data_generic(const char *vdisk_name, const char *file_name, Student *data, size_t record_size, int record_count) {
    FILE *vdisk_file = fopen(vdisk_name, "r+b");  // Open vdisk for read/write binary
    if (vdisk_file == NULL) {
        perror("Error opening the vdisk");
        return;
    }

    // Read the binary organization part: block status and file metadata
    FileData myfdata;
    fread(&myfdata, sizeof(FileData), 1, vdisk_file);
    
    int num_blocks = myfdata.n_blocks;
    int block_factor = myfdata.f_blockage;
    
    unsigned char *block_status = (unsigned char *)malloc(num_blocks * sizeof(unsigned char));
    if (block_status == NULL) {
        perror("Memory allocation failed for block status table");
        fclose(vdisk_file);
        return;
    }

    fread(block_status, sizeof(unsigned char), num_blocks, vdisk_file);  // Read block status

    // File table (metadata for each file)
    FileMetaData *file_table = (FileMetaData *)malloc(num_blocks * sizeof(FileMetaData));
    if (file_table == NULL) {
        perror("Memory allocation failed for file table");
        free(block_status);
        fclose(vdisk_file);
        return;
    }

    fread(file_table, sizeof(FileMetaData), num_blocks, vdisk_file);  // Read file metadata

    // Calculate number of blocks required to store the data
    int blocks_required =  ceil((float)record_count / block_factor);
    int free_blocks[blocks_required];
    int found_blocks = 0;

    // Find available blocks
    for (int i = 0; i < num_blocks && found_blocks < blocks_required; i++) {
        if (block_status[i] == 0) {  // Block is empty
            free_blocks[found_blocks++] = i;
        }
    }

    if (found_blocks < blocks_required) {
        printf("Not enough free blocks available on the vdisk.\n");
        free(block_status);
        free(file_table);
        fclose(vdisk_file);
        return;
    }

    // Write the data into free blocks
    int record_index = 0;
    
    for (int i = 0; i < blocks_required; i++) {
        int records_in_block = 0;
        Student block[block_factor];  // Block to write data into
        
        // Copy records into the block
        while (records_in_block < block_factor && record_index < record_count) {
            *(block+record_index)=*(data+record_index);
            record_index++;
            records_in_block++;
        }
        
        // Pad the block if not full
        

        // Write the block to the vdisk
        fseek(vdisk_file, free_blocks[i] * block_factor*sizeof(Student) + sizeof(FileData) + num_blocks * sizeof(unsigned char)+num_blocks*sizeof(FileMetaData), SEEK_SET);
        fwrite(block, block_factor*sizeof(Student), 1, vdisk_file);
        
        // Mark the block as occupied
        block_status[free_blocks[i]] = 1;
    }

    // Update file metadata (file_table)
    FileMetaData file_meta;
    strncpy(file_meta.file_name, file_name, MAXFileName - 1);
    file_meta.first_block = free_blocks[0];
    file_meta.num_blocks = blocks_required;
    file_meta.block_count = record_count;
    file_meta.org_mode_intern = 0;  // Internal organizational mode (to be set later)
    file_meta.org_mode_extern = 0;  // External organizational mode (to be set later)

    // Find an empty spot in the file metadata table
    int file_index = -1;
    for (int i = 0; i < num_blocks; i++) {
        if (file_table[i].first_block == -1) {  // Empty entry
            file_index = i;
            break;
        }
    }

    if (file_index == -1) {
        printf("No available space for new files in the file metadata table.\n");
        free(block_status);
        free(file_table);
        fclose(vdisk_file);
        return;
    }

    // Write the file metadata to the table
    fseek(vdisk_file, sizeof(FileData) + num_blocks * sizeof(unsigned char) + file_index * sizeof(FileMetaData), SEEK_SET);
    fwrite(&file_meta, sizeof(FileMetaData), 1, vdisk_file);

    // Update the block status table
    fseek(vdisk_file, sizeof(FileData), SEEK_SET);
    fwrite(block_status, sizeof(unsigned char), num_blocks, vdisk_file);

    // Clean up and close the file
    free(block_status);
    free(file_table);
    fclose(vdisk_file);

    printf("File '%s' written successfully, spanning %d blocks starting at block %d.\n", file_name, blocks_required, free_blocks[0]);
}

int main() {
    const char *vdisk_name = "vdisk.bin";
    const char *file_name = "StudentData";
    Student students[3] = {
        {12345, "John Doe", "20211234", 'A'},
        {23456, "Jane Smith", "20211345", 'B'},
        {34567, "Alice Johnson", "20211456", 'C'}
    };
    
    write_data_generic(vdisk_name, file_name, students, sizeof(Student), 3);  // Example usage for writing student data
    return 0;
}
