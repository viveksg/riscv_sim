#ifndef __MM__HANDLER__
#define __MM__HANDLER__
#include "mmu.h"
class MMHandler{
    private:
        vector<memory_region> mm_regions;
        uint8_t *memory_buff;
        uint64_t base_addr = 0;
    public:
        bool write_byte(uint64_t addr, uint8_t byte);
        bool read_byte(uint64_t addr, uint8_t *byte);
        bool read_bytes(uint64_t addr, uint8_t *bytes, uint32_t num_bytes);
        bool write_bytes(uint64_t addr, uint8_t *bytes, uint32_t num_bytes);   
        uint32_t load32(uint64_t addr, uint32_t width ,bool isSignedload);
        bool store32(uint64_t addr, uint32_t data, uint32_t width);  
        uint64_t load64(uint64_t addr, uint64_t width ,bool isSignedload);
        bool store64(uint64_t addr, uint64_t data, uint32_t width); 
        void set_region(uint64_t start_addr, uint64_t end_addr,uint64_t r111size, string name);
        void print_all_regions();
        MMHandler(uint32_t size);
        ~MMHandler();
        void setDefaultBaseAddr();
        void setBaseAddr(uint64_t addr);
        //mmu_methods
        bool MMHandler::mmu_resolve(uint32_t addr, uint32_t *ret_addr);
        uint8_t *get_phy_pointer(uint32_t addr);

};

#endif