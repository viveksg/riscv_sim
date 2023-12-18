#include "mm_handler.h"

bool MMHandler::write_byte(uint64_t addr, uint8_t dbyte)
{
    addr = addr - base_addr;
    if (addr >= MAX_MEMSIZE)
        return false;
    memory_buff[addr] = dbyte;
    return true;
}
bool MMHandler::read_byte(uint64_t addr, uint8_t *dbyte)
{
    addr = addr - base_addr;
    if (addr >= MAX_MEMSIZE)
        return false;
    *dbyte = memory_buff[addr];
    return true;
}
bool MMHandler::read_bytes(uint64_t addr, uint8_t *dbytes, uint32_t num_bytes)
{
    addr = addr - base_addr;
    if (addr >= MAX_MEMSIZE || addr + num_bytes >= MAX_MEMSIZE)
        return false;
    for (uint64_t curr_offset = 0; curr_offset < num_bytes; curr_offset++)
        dbytes[curr_offset] = memory_buff[addr + curr_offset];
    return true;
}

bool MMHandler::write_bytes(uint64_t addr, uint8_t *dbytes, uint32_t num_bytes)
{
    addr = addr - base_addr;
    if (addr >= MAX_MEMSIZE || addr + num_bytes >= MAX_MEMSIZE)
        return false;
    for (uint64_t curr_offset = 0; curr_offset < num_bytes; curr_offset++)
        memory_buff[addr + curr_offset] = dbytes[curr_offset];
    return true;
}

uint32_t MMHandler::load32(uint64_t addr, uint32_t width, bool isSignedload)
{
    addr = addr - base_addr;
    uint32_t data = 0;
    uint32_t loaded_data = 0;
    read_bytes(addr, (uint8_t *)&data, width);
    switch (width)
    {
    case SIZE_WORD:
        assert(!(addr & (SIZE_WORD - 1)));
        loaded_data = data;
        break;
    case SIZE_HALFWORD:
        assert(!(addr & (SIZE_HALFWORD - 1)));
        loaded_data = (int16_t)data;
        break;
    case SIZE_BYTE:
        loaded_data = (int8_t)data;
        break;

    default:
        break;
    }
    return loaded_data;
}
bool MMHandler::store32(uint64_t addr, uint32_t data, uint32_t width)
{

    addr = addr - base_addr;
    switch (width)
    {
    case SIZE_WORD:
        if ((addr & (SIZE_WORD - 1)))
            return false;
        break;
    case SIZE_HALFWORD:
        if ((addr & (SIZE_HALFWORD - 1)))
            return false;
        break;
    default:
        break;
    }
    write_bytes(addr, (uint8_t *)&data, width);
    return true;
}
uint64_t MMHandler::load64(uint64_t addr, uint64_t width, bool isSignedload)
{
    addr = addr - base_addr;
    uint64_t data = 0;
    uint64_t loaded_data = 0;
    read_bytes(addr, (uint8_t *)&data, width);
    switch (width)
    {
    case SIZE_DOUBLEWORD:
        assert(!(addr & (SIZE_DOUBLEWORD - 1)));
        loaded_data = data;
        break;
    case SIZE_WORD:
        assert(!(addr & (SIZE_WORD - 1)));
        loaded_data = (int32_t)data;
        break;
    case SIZE_HALFWORD:
        assert(!(addr & (SIZE_HALFWORD - 1)));
        loaded_data = (int16_t)data;
        break;
    case SIZE_BYTE:
        loaded_data = (int8_t)data;
        break;

    default:
        break;
    }
    return loaded_data;
}
bool MMHandler::store64(uint64_t addr, uint64_t data, uint32_t width)
{
    addr = addr - base_addr;
    switch (width)
    {
    case SIZE_DOUBLEWORD:
        if ((addr & (SIZE_DOUBLEWORD - 1)))
            return false;
        break;
    case SIZE_WORD:
        if ((addr & (SIZE_WORD - 1)))
            return false;
        break;
    case SIZE_HALFWORD:
        if ((addr & (SIZE_HALFWORD - 1)))
            return false;
        break;
    default:
        break;
    }
    write_bytes(addr, (uint8_t *)&data, width);
    return true;
}

void MMHandler::set_region(uint64_t start_addr, uint64_t end_addr, uint64_t rsize, string name)
{
    mm_regions.push_back({start_addr, end_addr, rsize, name});
}

void MMHandler::print_all_regions()
{
    for (auto mregion = mm_regions.begin(); mregion != mm_regions.end(); mregion++)
    {
        cout << mregion->name << "start: " << mregion->start_addr << ", end: " << mregion->end_addr << ", size: " << mregion->region_size / KILOBYTES << endl;
        uint32_t data = 0;
        for (uint32_t i = mregion->start_addr; i < mregion->end_addr; i += 4)
        {
            read_bytes(i, (uint8_t *)&data, SIZE_WORD);
            if (data > 0)
                cout << hex << i << ": " << data << endl;
        }
    }
    cout << "print complete" << endl;
}
MMHandler::MMHandler(uint32_t size)
{
    memory_buff = new uint8_t[size];
}

MMHandler::~MMHandler()
{
    delete memory_buff;
}

void MMHandler::setDefaultBaseAddr()
{
    for (auto mreg = mm_regions.begin(); mreg != mm_regions.end(); mreg++)
    {
        if (mreg == mm_regions.begin())
            base_addr = mreg->start_addr;
        else
            base_addr = base_addr > mreg->start_addr ? mreg->start_addr : base_addr;
    }
}
void MMHandler::setBaseAddr(uint64_t addr)
{
    base_addr = addr;
}
bool MMHandler::mmu_resolve(uint32_t addr, uint32_t *ret_addr)
{
    // todo add virtual memory later;
    return false;
}

uint8_t *MMHandler::get_phy_pointer(uint32_t addr)
{
    addr = addr - base_addr;
    return (uint8_t *)&memory_buff[addr];
}


//virtual memory


void MMHandler::process_satp_register(uint64_t satp_value)
{
#ifdef _MMU_64
    uint32_t mode_bits = extract_bits(satp_value, BIT_MODE_START, BIT_MODE_END);
    uint32_t satp_asid = extract_bits(satp_value, BIT_ASID_START, BIT_ASID_END);
    page_table_base = extract_bits(satp_value, BIT_PTB_START, BITS_PTB_END);
    if (mode_bits == MODE_VAL_BARE64)
        mmu_mode = MODE_BARE_64;
    else if (mode_bits == MODE_VALSV39)
        mmu_mode = MODE_SV39;
    mmu_mode = MODE_SV39;
    else if (mode_bits == MODE_SV48)
        mmu_mode = MODE_SV48;
#else
    uint32_t satp_val_32 = (satp_value & 0x00000000FFFFFFFFLL);
    mmu_mode = satp_val_32 >> 31 == 1 ? MODE_SV32 : MODE_BARE_32;
    uint32_t satp_asid = extract_bits(satp_value, BIT_ASID_START, BIT_ASID_END);
    page_table_base = extract_bits(satp_value, BIT_PTB_START, BITS_PTB_END);
#endif
}

void MMHandler::parse_virtual_address(virtual_addr *vaddr, uint64_t addr)
{
    uint32_t num_levels = 0;
    virtual_addr_info vinfo;
#ifdef _MMU_64
    switch (mmu_mode)
    {
    case MODE_SV39:
        vaddr->num_levels = SV39_LEVELS;
        vaddr->level_offsets[0] = extract_bits(addr, SV39_VA_LEVEL_O_START, SV39_VA_LEVEL_0_END);
        vaddr->level_offsets[1] = extract_bits(addr, SV39_VA_LEVEL_1_START, SV39_VA_LEVEL_1_END);
        vaddr->level_offsets[2] = extract_bits(addr, SV39_VA_LEVEL_2_START, SV39_VA_LEVEL_2_END);
        vaddr->page_offset = extract_bits(addr, SV39_VA_PAGE_OFFSET_START, SV39_VA_PAGE_OFFSET_END);
        break;

    case MODE_SV48:
        vaddr->num_levels = SV48_LEVELS;
        vaddr->level_offsets[0] = extract_bits(addr, SV48_VA_LEVEL_O_START, SV48_VA_LEVEL_0_END);
        vaddr->level_offsets[1] = extract_bits(addr, SV48_VA_LEVEL_1_START, SV48_VA_LEVEL_1_END);
        vaddr->level_offsets[2] = extract_bits(addr, SV48_VA_LEVEL_2_START, SV48_VA_LEVEL_2_END);
        vaddr->level_offsets[3] = extract_bits(addr, SV48_VA_LEVEL_3_START, SV48_VA_LEVEL_3_END);
        vaddr->page_offset = extract_bits(addr, SV48_VA_PAGE_OFFSET_START, SV48_VA_PAGE_OFFSET_END);
        break;

    default:
        break;
    }
#else
    vaddr->num_levels = SV32_LEVELS;
    vaddr->level_offsets[0] = extract_bits(addr, SV32_VA_LEVEL_O_START, SV32_VA_LEVEL_0_END);
    vaddr->level_offsets[1] = extract_bits(addr, SV32_VA_LEVEL_1_START, SV32_VA_LEVEL_1_END);
    vaddr->page_offset = extract_bits(addr, SV32_VA_PAGE_OFFSET_START, SV32_VA_PAGE_OFFSET_END);
#endif
}

uint64_t MMHandler::pgtable_walk(uint64_t addr, uint32_t pc, bool fetch, uint64_t satp_value, virtual_addr* vaddr)
{
    uint64_t curr_base = page_table_base;
    uint64_t page_table_entry = 0;
    for (int i = 0; i < vaddr->num_levels -1; i++)
    {
#ifdef _MMU_64
        uint64_t effective_address = curr_base + vaddr.level_offsets[i] * PTE_SIZE_MMU64;
        page_table_entry = *((uint64_t *)mhandler->get_phy_pointer(effective_address));
#else
        uint32_t effective_address = curr_base + vaddr->level_offsets[i] * PTE_SIZE_MMU32;
        page_table_entry = *((uint32_t *)get_phy_pointer(effective_address));
#endif

        if(!(page_table_entry & PMA_BIT_PAGE_PRESENT))
        {
            page_table_entry = 0;
            break;
        }
        if(!(page_table_entry & (PMA_BIT_PAGE_READ|PMA_BIT_PAGE_WRITE|PMA_BIT_PAGE_EXEC)))
        {
            //Non leaf pte;
            curr_base = (page_table_entry >> PMA_BIT_SHIFT) << PG_OFFSET_SHIFT;
        }
        else
        {
           //leaf page's PTE

            uint32_t pma_bits = page_table_entry & set_mask(0,10);
            uint64_t addr = (page_table_entry >> PMA_BIT_SHIFT) ;
            addr = ((addr | vaddr->level_offsets[i]) << PG_OFFSET_SHIFT)|pma_bits;

#ifdef _MMU_64
           assert(((addr) >> 64) == 0);
           page_table_entry = (uint64_t)addr;
#else
           assert(((addr) >> 32) == 0);
           page_table_entry = (uint32_t)addr;
#endif           

        }
    }
    return page_table_entry;
}

bool MMHandler::address_translate(uint64_t addr, physical_address *phy_addr, uint32_t pc, bool fetch, bool writeNotRead, uint32_t satp_val, uint64_t m_csr_priv, uint64_t m_csr_msr, page_fault *pfault)
{   

    virtual_addr vaddr;
//parse SATP register to get the page table base, asid and mode    
    process_satp_register(satp_val);
// Extract level offsets and page offset from the virtual address    
    parse_virtual_address(&vaddr, addr);
//If machine mode and mmu mode is bare mode then vitual address is same as physical address    
    if(m_csr_priv > PRIV_SUPER || satp_val == MODE_BARE_32 || satp_val == MODE_BARE_64)
    {
        phy_addr->phy_addr = addr;
        return true;
    }
//get the page table entry for the virtual address;    
#ifdef _MMU_64    
    uint64_t page_table_entry = pgtable_walk(addr,pc,fetch,satp_val,&vaddr);   
#else
    uint32_t page_table_entry = (uint32_t) pgtable_walk(addr,pc,fetch,satp_val,&vaddr);  
#endif    
    
        bool page_fault = false;
// extact read,write and execute PMA bits        
        uint32_t pma_xwr = page_table_entry & (PMA_BIT_PAGE_READ | PMA_BIT_PAGE_WRITE | PMA_BIT_PAGE_ACCESSED);

// Check for possible page fault routes.
// Initiate page_fault at reserved combintations like page write and executable page write. 
        if(pma_xwr == PMA_BIT_PAGE_WRITE || pma_xwr == (PMA_BIT_PAGE_EXEC | PMA_BIT_PAGE_WRITE))
           page_fault = true;
        else if(m_csr_priv == PRIV_SUPER)
        {
            if(fetch)
            {
                //supervisor mode trying to fetch and instruction page of the user mode
                if(page_table_entry & PMA_BIT_PAGE_USER)
                {
                    page_fault = true;
                } 
                // page is not executable, and access is of instruction fetch in supervisor mode
                else if((page_table_entry & PMA_BIT_PAGE_EXEC) != PMA_BIT_PAGE_EXEC)
                {
                    page_fault = true;
                }

            }
            else
            {   // if page access is to be done in user mode and supervisor access is disabled
                if((m_csr_priv == PRIV_USER) && !(m_csr_msr & SR_SUM))
                {
                    cout <<hex <<"page access :"<<addr<<endl;
                }
                else
                { /*
                   * initiate exception if the access type is write, but write PMA bit is not set
                   * initiate exception if the access type if read , but read PMA bit is not set
                  */
                    if((writeNotRead && ((page_table_entry & PMA_BIT_PAGE_WRITE)!= PMA_BIT_PAGE_WRITE))||
                      (!writeNotRead && ((page_table_entry & PMA_BIT_PAGE_READ) != PMA_BIT_PAGE_READ)))
                      {
                        page_fault = true;
                      }
                }
            }
        } 


    if(page_fault)
    {
        // If page fault is to be reported by the processor set page fault address, 
        // program counter and physical address for exception handler       
        phy_addr->phy_addr = 0xFFFFFFFF;
        pfault->pc = pc;
        pfault->addr = addr;
        //set the values for the cause register, which will be used during exception handling
        if(fetch){
           //And instruction page (executable) was being attempted to read 
            pfault->cause = MCAUSE_FAULT_FETCH;
        }
        else
        {// and read or write page was attempted to be accessed.
            pfault->cause = writeNotRead?MCAUSE_PAGE_FAULT_STORE:MCAUSE_PAGE_FAULT_STORE;
        }
        return false;
    }

#ifdef _MMU_64
// set the 64 bit physical address
    phy_addr->phy_addr64 = ((page_table_entry >> PG_OFFSET_SHIFT) << PG_OFFSET_SHIFT) + vaddr.page_offset;
#else
// set the 32 bit physical address
    phy_addr->phy_addr = (uint32_t)((((page_table_entry & 0x00000000FFFFFFFFLL) >> PG_OFFSET_SHIFT) << PG_OFFSET_SHIFT) + vaddr.page_offset);
#endif
    return true;
}