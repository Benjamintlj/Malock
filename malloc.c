//
// Created by Benjamin Lewis-Jones on 15/08/2022.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGESIZE 4069


typedef struct metadata {

    int length;
    int free; // 0 - taken, 1 - free

    struct metadata * next;
    struct metadata * prev;

} metadata;



metadata * start;
metadata * headOfFreeList;


//
void init() {
    start = sbrk(PAGESIZE);
    start->free = 1;
    start->length = PAGESIZE - sizeof(start);
    start->next = NULL;

    headOfFreeList = start;
}



//
void newFree(metadata * freedBlock) {
    freedBlock->free = 1;

    freedBlock->next = headOfFreeList->next;
    headOfFreeList->next = freedBlock;
    freedBlock->prev = headOfFreeList;

}




int getGapToNextBlock(metadata * newBlock, int oldLength) {

    printf("space to next: %d\n", ((oldLength - newBlock->length) - sizeof(metadata)));
    return (oldLength - newBlock->length) - sizeof(metadata);

}

void pointToNextAvailableSpace(metadata * current, metadata * previous, int size) {


}

void createMetadata(metadata * current, metadata * previous, int size) {
    // free block, and length is allocated
    current->free = 0;
    current->length = size;

    previous->next = current->next;
}

void createMetadataForNewFreeBlock(metadata * ptr, metadata * newBlock, int oldLength) {

    // move to new metadata location
    ptr += ptr->length + sizeof(metadata);

    // length of new metadata to the space to the next block
    // set the new metadata to free
    metadata newMetadata = {getGapToNextBlock(newBlock, oldLength), 1};
    *ptr = newMetadata;

    ptr->next = headOfFreeList->next;
    headOfFreeList->next = ptr;

}


void * newMalloc(int size) {

    // look at start of list
    metadata * previous = headOfFreeList;
    metadata * current = headOfFreeList->next;

    if (current == NULL) {
        current = previous;
    } else {
        // if not free move forward OR if it does not fit inside the gap
        while (current->free == 0 || current->length < (size + sizeof(metadata))) {

            previous = current;
            current = current->next;
            current->prev = previous;

        }
    }

    // storing old length
    int oldLength = current->length;

    createMetadata(current, previous, size);

    // pointer to new block
    metadata * newBlock = current;

    createMetadataForNewFreeBlock(current, newBlock, oldLength);

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
    addr2 = newMalloc(110);
    addr3 = newMalloc(120);
    
//
//    newFree(addr2);
//
//    addrA = newMalloc(30);
//    addrB = newMalloc(10);
//    addrC = newMalloc(4);

    printf("size of metadata: %lu\n", sizeof(metadata));

    printf("length: %lu\n", addr1->next->next->length);

    return 0;
}
