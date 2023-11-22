#include "cpu_core.h"

void CPUCore::get_register(uint32_t reg_no)
{
}
void CPUCore::set_register(uint32_t reg_no, uint64_t reg_val)
{
}
void CPUCore::execute()
{

#ifdef VER32
    core_pc += INC_PC_32_BIT;
#else
    core_pc += INC_PC_64_BIT;
#endif        
}
void CPUCore::decode(uint64_t opcode, instruction_packet *instr_packet)
{
}
void CPUCore::start_simulation(uint64_t start_addr)
{
    core_pc = start_addr;
    continue_simulation = true;
    while(continue_simulation){
        execute();
    }
}
CPUCore::CPUCore(MMHandler *mhndler)
{
    mHandler = mhndler;
}