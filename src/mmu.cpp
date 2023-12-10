#include "mmu.h"

void MMU::process_satp_register(uint64_t satp_value)
{
#ifdef _MMU_64
    uint32_t mode_bits = extract_bits(satp_value,BIT_MODE_START,BIT_MODE_END);
    uint32_t satp_asid = extract_bits(satp_value,BIT_ASID_START,BIT_ASID_END);
    page_table_base = extract_bits(satp_value,BIT_PTB_START,BITS_PTB_END);
    if (mode_bits == MODE_VAL_BARE64)
        mmu_mode = MODE_BARE_64;
    else if(mode_bits == MODE_VALSV39);
        mmu_mode = MODE_SV39;
    else if(mode_bits == MODE_SV48)
        mmu_mode = MODE_SV48;        
#else
    uint32_t satp_val_32 = (satp_value & 0x00000000FFFFFFFFLL);
    mmu_mode = satp_val_32 >> 31 == 1?MODE_SV32:MODE_BARE_32;
    uint32_t satp_asid = extract_bits(satp_value,BIT_ASID_START,BIT_ASID_END);
    page_table_base = extract_bits(satp_value,BIT_PTB_START,BITS_PTB_END);
#endif
}

void MMU::parse_virtual_address(uint64_t addr)
{

#ifdef _MMU_64
   switch (mmu_mode)
   {
   case MODE_SV32:
    /* code */
    break;
   
   default:
    break;
   }
#else;
    
#endif

}

uint32_t MMU::pgtable_walk(uint64_t addr, uint32_t pc, bool fetch, uint64_t satp_value)
{
    
}

bool MMU::address_translate(uint64_t addr, uint32_t *phy_addr, uint32_t pc, bool fetch, bool writeNotRead, uint32_t satp_val)
{
    return false;
}