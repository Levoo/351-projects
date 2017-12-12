#include <iostream>
#include <fstream>


#define BUFFER_SIZE          32
#define FRAME_SIZE           256
#define PAGE_SIZE            256
#define TLB_SIZE             16


void trans_logi_to_phys(FILE * input_file);

int main(int argc, char ** argv) {
    if(argc < 2){
        printf("Not enough enough argumetns\n\t Usage ./a.out file.txt\n");
        exit(1);
    }
    printf("STARTING APP .......................\n\n");
    // open address file
    FILE *address_file;
    address_file = fopen(argv[1], "r");
    //check if able to open
    if(!address_file){
        printf("Error in opening file: %s \n", argv[1]);
        return 0;
    }
    trans_logi_to_phys(address_file);
    fclose(address_file);
    return 0;
}

void  trans_logi_to_phys(FILE * input_file){
    printf("Beginning translation of logical addresses to physical addresses...\n\n");
    int total_addr_trans = 0;
    char buffer[BUFFER_SIZE]; /*make a buffer*/
    int input_num; /*variable to store the address being read in*/
    int address;
    int page;
    int offset;

    while(!feof(input_file)){  /*keep reading until end of file is reached*/
        total_addr_trans++;
        fscanf(input_file, "%s", buffer);
        input_num = atoi(buffer);  /*convert to int*/

        address = (input_num &  0xffff);
        page = (address >> 8);
        offset = (address & 0xff);
        printf("virt: 0x%x (pg:0x%x,off:0x%x)---> "
                       "-----------, ---------- , *IN_BACK_S*,  "
                       "phys: 0x0014, val:    0\t  tlb_Hits:  0\n",
               address, page, offset);
        if(total_addr_trans % 5 == 0 && total_addr_trans != 0) printf("\n");
    }
}
