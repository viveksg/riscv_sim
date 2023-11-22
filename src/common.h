#ifndef __COMMON__
#define __COMMON__
#include "constants.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdbool.h>
#include <assert.h>
#include <vector>
#define MAX_MEMSIZE 0xFFFFFFFF
#define SIZE_WORD 4
#define SIZE_HALFWORD 2
#define SIZE_BYTE 1
#define SIZE_DOUBLEWORD 8
#define KILOBYTES 1024
#define MAX_REG 128
#define MAX_CSRS 1024
using namespace std;
typedef struct
{
    uint64_t start_addr;
    uint64_t end_addr;
    uint64_t region_size;
    string name;
} memory_region;
#endif