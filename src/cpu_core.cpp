#include "cpu_core.h"

void CPUCore::get_register(uint32_t reg_no)
{
}

uint32_t CPUCore::get_opcode(uint64_t addr)
{
    uint32_t opcode = 0;
    uint64_t phy_addr = addr;
#ifdef MMU_ENABLED
    phy_addr = mmu_translate(addr);
#endif
    mHandler->read_bytes(phy_addr, (uint8_t *)&opcode, 4);
    return opcode;
}

void CPUCore::set_register(uint32_t reg_no, uint64_t reg_val)
{
}
void CPUCore::execute()
{
    instruction_packet inst_packet;
    uint32_t opcode = get_opcode(core_pc);
    decode(opcode, &inst_packet);
    //execution_logic_begins

#ifdef VER32
    core_pc += INC_PC_32_BIT;
#else
    core_pc += INC_PC_64_BIT;
#endif
}
void CPUCore::decode(uint64_t opcode, instruction_packet *instr_packet)
{
    int src_reg1 = 0, src_reg2 = 0, dst_reg = 0;
    src_reg1 = (opcode & OPCODE_RS1_MASK) >> OPCODE_RS1_SHIFT; // bits [15-19]
    src_reg2 = (opcode & OPCODE_RS2_MASK) >> OPCODE_RS2_SHIFT; // bits [20-23]
    dst_reg = (opcode & OPCODE_RD_MASK) >> OPCODE_RD_SHIFT;    // bits [7-11]

    //Immdediates
    int i_imm = ((signed)(opcode & OPCODE_TYPEI_IMM_MASK)) >> OPCODE_TYPEI_IMM_SHIFT; // bits[20-31]
    int i_umm = ((signed)(opcode & OPCODE_TYPEU_IMM_MASK)) >> OPCODE_TYPEU_IMM_SHIFT; // bits[12-31]
    int imm20 = i_umm << OPCODE_TYPEU_IMM_SHIFT; //20 bit immediate
    int imm12 = i_imm; //12 bit immediate
    int simm = OPCODE_STYPE_IMM(opcode); //store instr immediate bits[7-11] and bits[25-31]
    int bimm = OPCODE_SBTYPE_IMM(opcode); // bramch instr immediate bits[7-11] and bits[25-31] sign extended
    int jimm20 = OPCODE_UJTYPE_IMM(opcode); // jump instr immediate bits 
    int shift_amount = ((signed)(opcode & OPCODE_SHAMT_MASK))>> OPCODE_SHAMT_SHIFT;
    uint32_t decoded_op = 0;
    if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }
    else if((opcode & INST_ANDI_MASK) == INST_ANDI)
    {
        decoded_op = ENUM_INST_ANDI;
    }

}

void CPUCore::start_simulation(uint64_t start_addr)
{
    core_pc = start_addr;
    continue_simulation = true;
    while (continue_simulation)
    {
        execute();
    }
}
CPUCore::CPUCore(MMHandler *mhndler)
{
    mHandler = mhndler;
}