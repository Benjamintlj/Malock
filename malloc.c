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

void newFree(metadata * freedBlock) {
    freedBlock->free = 1;
}


int getGapToNextBlock(metadata * newBlock, int oldLength) {

    printf("space to next: %d\n", ((oldLength - newBlock->length) - sizeof(metadata)));
    return (oldLength - newBlock->length) - sizeof(metadata);

}




void * newMalloc(int size) {

    // look at start of list
    metadata * ptr = start;


    // if not free move forward OR if it does not fit inside the gap
    while (ptr->free == 0 || ptr->length < (size + sizeof(metadata))) {

        ptr += ptr->length + sizeof(metadata);

    }

    // storing old length
    int oldLength = ptr->length;

    // free block made not free, and length is allocated
    ptr->free = 0;
    ptr->length = size;

    // pointer to new block
    metadata * newBlock = ptr;

    // move to new metadata location
    ptr += ptr->length + sizeof(metadata);

    // length of new metadata to the space to the next block
    // set the new metadata to free
    metadata newMetadata = {getGapToNextBlock(newBlock, oldLength), 1};
    *ptr = newMetadata;

    // return location of new block
    return newBlock;
}


int main() {
    init();

    metadata * addr1;
    metadata * addr2;
    metadata * addr3;

    metadata * addrA;
    metadata * addrB;
    metadata * addrC;

    addr1 = newMalloc(100);
    addr2 = newMalloc(100);
    addr3 = newMalloc(100);

    newFree(addr2);

    addrA = newMalloc(50);
    addrB = newMalloc(35);
    addrC = newMalloc(34);



    return 0;
}
