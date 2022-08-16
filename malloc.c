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


void loopThroughFreeList() {

    metadata * current = headOfFreeList->next;

    while (current != NULL) {
        printf("  %u", current->length);
        current = current->next;
    }

    printf("\n");
}



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

void createMetadata(metadata * current, metadata * previous, int size) {
    // free block, and length is allocated
    current->free = 0;
    current->length = size;

    previous->next = current->next;
    current->prev = previous;
}

void createMetadataForNewFreeBlock(metadata * current, metadata * newBlock, int oldLength) {

    // move to new metadata location
    current += current->length + sizeof(metadata);

    // length of new metadata to the space to the next block
    // set the new metadata to free
    metadata newMetadata = {getGapToNextBlock(newBlock, oldLength), 1};
    *current = newMetadata;

    current->next = headOfFreeList->next;
    headOfFreeList->next = current;

}


void * newMalloc(int size) {

    // look at start of list
    metadata * previous = headOfFreeList;
    metadata * current = headOfFreeList->next;

    metadata * previousBestFit = NULL;
    metadata * currentBestFit = NULL;

    // occurs when heap is empty
    if (current == NULL) {
        current = previous;
    } else {

        // move through the list to the end
        while (current->next != NULL) {

            previous = current;
            current = current->next;
            current->prev = previous;

            // stores the best fitting location
            if (currentBestFit == NULL || current->length >= (size + sizeof(metadata)) && currentBestFit->length < current->length) {
                currentBestFit = current;
                previousBestFit = previous;
            }
        }

        // if a best fitted location was found then make current equal to it
        if (currentBestFit != NULL) {
            current = currentBestFit;
            previous = previousBestFit;
        }

        // if there is not enough space then make a new page
        if (current->length < size) {
            previous = current;
            current = current->next;

            // this should only occur when at the end of the free list and there is still not enough space
            current = sbrk(PAGESIZE);
            current->length = PAGESIZE - sizeof(metadata);
        }
    }


    // allocate new block, and create new metadata
    int oldLength = current->length;
    createMetadata(current, previous, size);
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

    loopThroughFreeList();
    addr1 = newMalloc(1000);
    loopThroughFreeList();
    addr2 = newMalloc(1000);
    loopThroughFreeList();
    addr3 = newMalloc(1000);
    loopThroughFreeList();

    addrA = newMalloc(1000);
    loopThroughFreeList();
    addrB = newMalloc(800);
    loopThroughFreeList();

    return 0;
}
