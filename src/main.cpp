#include "elf_parser.h"
#include "mm_handler.h"
int main(void)
{   
    MMHandler *mmHandler = new MMHandler(8<<24);
    uint32_t start_loc;
    string file_loc = "/home/vivsg/projects/riscv/riscv_sim/elfs/linux.elf";
    ELFParser *elf_parser = new ELFParser(file_loc,&start_loc,mmHandler);
    mmHandler->print_all_regions();
    uint32_t x = 0xFF;
    int32_t y = (int16_t)x;
    int32_t z = (int8_t)x;
   // int8_t t = (int8_t)x;
    cout << x <<" "<< y<<" " <<z <<" "  <<endl;
    delete elf_parser;
    return 0;
}