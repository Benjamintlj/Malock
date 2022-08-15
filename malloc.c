//
// Created by Benjamin Lewis-Jones on 15/08/2022.
//

#include <unistd.h>
#include <printf.h>

#define PAGESIZE 4069


typedef struct metadata {

    int length;
    int free; // 0 - taken, 1 - free

} metadata;

metadata * start;




void init() {
    start = sbrk(PAGESIZE);
    start->free = 1;
    start->length = PAGESIZE - sizeof(start);
}




void * newMalloc(int size) {

    // look at start of list
    metadata * ptr = start;

    int length = 0;

    // if not free move forward
    while (ptr->free == 0) {

        length += ptr->length + sizeof(metadata);
        ptr += ptr->length + sizeof(metadata);

    }

    // free block made not free, and length is allocated
    ptr->free = 0;
    ptr->length = size;

    // pointer to new block
    metadata * newBlock = ptr;

    // move to new metadata location
    ptr += ptr->length + sizeof(metadata);

    // length of new metadata to the space to the next block
    // set the new metadata to free
    metadata newMetadata = {PAGESIZE - length, 1};
    *ptr = newMetadata;

    // return location of new block
    return newBlock;
}

void newFree(metadata * freedBlock) {
    freedBlock->free = 1;
}


int getGapToNextBlock() {
    return 0;
}



int main() {
    init();

    metadata * addr1;
    metadata * addr2;
    metadata * addr3;

    addr1 = newMalloc(100);
    addr2 = newMalloc(100);
    addr3 = newMalloc(100);

    printf("%p \n", addr1);
    printf("%p \n", addr2);
    printf("%p \n", addr3);

    return 0;
}
