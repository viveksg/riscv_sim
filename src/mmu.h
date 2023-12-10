#ifndef __MCNTRL__
#define __MCNTRL__
#include "common.h"
class MMU{
     
    private:
       uint32_t mmu_mode;
       uint32_t detected_mmu_mode;
       uint32_t satp_asid;
       uint32_t page_table_base;
       uint8_t *memory_buffer;
    public:
       MMU(uint8_t *mem_ptr);
       void process_satp_register(uint64_t satp_value);
       void parse_virtual_address(uint64_t addr);
       uint32_t pgtable_walk(uint64_t addr, uint32_t pc, bool fetch, uint64_t satp_value); 
       bool address_translate(uint64_t addr, uint32_t* phy_addr, uint32_t pc, bool fetch, bool writeNotRead, uint32_t satp_val);   
};
#endif