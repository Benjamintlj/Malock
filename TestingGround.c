//
// Created by Benjamin Lewis-Jones on 15/08/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    printf("%p \n", sbrk(0));
    printf("%p \n", sbrk(10));
    printf("%p \n", sbrk(0));
}