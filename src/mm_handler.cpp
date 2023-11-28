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
#ifndef MMU_ENABLED
    *ret_addr = addr;
    return false;
#endif
    return false;
}

uint8_t *MMHandler::get_phy_pointer(uint32_t addr)
{
    addr = addr - base_addr;
    return (uint8_t *)&memory_buff[addr];
}