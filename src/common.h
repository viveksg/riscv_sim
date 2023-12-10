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
#include "riscv_isa.h"

#define MAX_MEMSIZE 0xFFFFFFFF
#define SIZE_WORD 4
#define SIZE_HALFWORD 2
#define SIZE_BYTE 1
#define SIZE_DOUBLEWORD 8
#define KILOBYTES 1024
#define MAX_REG 128
#define MAX_CSRS 1024
#define DECODE_SUCCESSFULL 2000
#define DECODE_UNSUCCESSFULL 2001
#define MODE_BARE_32 1000
#define MODE_BARE_64 1001
#define MODE_SV32 1002
#define MODE_SV39 1003
#define MODE_SV48 1004

using namespace std;

#ifdef _MMU_64
#define BIT_ASID_START 44
#define BIT_ASID_END 59
#define BIT_MODE_START 60
#define BIT_MODE_END 63
#define BIT_PTB_START 0
#define BITS_PTB_END 43

#define MODE_VAL_BARE64 0
#define MODE_VALSV39 8
#define MODE_VALSV56 9

#else
#define BIT_ASID_START 22
#define BIT_ASID_END 30
#define BIT_MODE_START 31
#define BIT_MODE_END 31
#define BIT_PTB_START 0
#define BITS_PTB_END 21
#endif

typedef struct
{
    uint64_t start_addr;
    uint64_t end_addr;
    uint64_t region_size;
    string name;
} memory_region;

#define MAX_LEVELS 4

typedef struct
{
    uint32_t mask;
    uint32_t shift;
} level_info;

typedef struct
{
    level_info linfo[MAX_LEVELS];
    uint32_t page_offset_bits;
    uint32_t levels;
} virtual_addr_info;

typedef struct
{
    uint32_t level_offsets[MAX_LEVELS];
    uint32_t page_offset;
} virtual_addr;

typedef struct
{
    uint32_t next_level_phy_addr;
    bool is_valid;
    bool is_read;
    bool is_write;
    bool is_execute;
    bool is_user;
    bool is_global;
    uint32_t accessed;
    bool is_dirty;
} pte_entry;

#define set_mask(a, b) (((1 << (b - a +1)) - 1) << a)
#define extract_bits(b, c, d) ((b & set_mask(c, d)) >> d)
#endif