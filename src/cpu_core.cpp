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
    if (inst_packet.decode_status == DECODE_UNSUCCESSFULL || opcode == 0)
    {
        //cout << "----"
          //   << "decode failed" << endl;
        //continue_simulation = false;

#ifdef VER64
    core_pc += INC_PC_64_BIT;
#else
    core_pc += INC_PC_32_BIT;
#endif
        return;
    }

    cout << hex << core_pc << ": " << opcode << endl;
    cout << hex << "-----" << inst_names[inst_packet.instruction_val] << " " << instr_types[inst_packet.instruction_type - INSTR_TYPE_BASE];
    cout << inst_packet.source_reg1 << " " << inst_packet.source_reg2 << " " << inst_packet.dest_reg << " ";
    cout <<" " <<inst_packet.immediate << " " << endl;

#ifdef VER64
    core_pc += INC_PC_64_BIT;
#else
    core_pc += INC_PC_32_BIT;
#endif
}

void CPUCore::decode(uint32_t opcode, instruction_packet *instr_packet)
{
    int src_reg1 = 0, src_reg2 = 0, dst_reg = 0;
    src_reg1 = (opcode & OPCODE_RS1_MASK) >> OPCODE_RS1_SHIFT; // bits [15-19]
    src_reg2 = (opcode & OPCODE_RS2_MASK) >> OPCODE_RS2_SHIFT; // bits [20-23]
    dst_reg = (opcode & OPCODE_RD_MASK) >> OPCODE_RD_SHIFT;    // bits [7-11]

    // Immdediates
    int i_imm = ((signed)(opcode & OPCODE_TYPEI_IMM_MASK)) >> OPCODE_TYPEI_IMM_SHIFT; // bits[20-31]
    int i_umm = ((signed)(opcode & OPCODE_TYPEU_IMM_MASK)) >> OPCODE_TYPEU_IMM_SHIFT; // bits[12-31]
    int imm20 = i_umm << OPCODE_TYPEU_IMM_SHIFT;                                      // 20 bit immediate
    int imm12 = i_imm;                                                                // 12 bit immediate
    int simm = OPCODE_STYPE_IMM(opcode);                                              // store instr immediate bits[7-11] and bits[25-31]
    int bimm = OPCODE_SBTYPE_IMM(opcode);                                             // bramch instr immediate bits[7-11] and bits[25-31] sign extended
    int jimm20 = OPCODE_UJTYPE_IMM(opcode);                                           // jump instr immediate bits
    int shift_amount = ((signed)(opcode & OPCODE_SHAMT_MASK)) >> OPCODE_SHAMT_SHIFT;
    uint32_t decoded_op = 0;
    for (int i = 0; i < MAX_INSTR; i++)
    {
        instr_def curr_def = instr_defs[i];
        if ((opcode & curr_def.instruction_mask) == curr_def.instruction_match)
        {
            instr_packet->decode_status = DECODE_SUCCESSFULL;
            instr_packet->instruction_type = curr_def.instruction_type;
            instr_packet->instruction_val = curr_def.instruction_enum;
            instr_packet->source_reg1 = src_reg1;
            instr_packet->source_reg2 = src_reg2;
            instr_packet->dest_reg = dst_reg;
            if (curr_def.shift_amt == SHAMT_REQUIRED)
                instr_packet->shamt = shift_amount;
            instr_packet->immediate_type = curr_def.immediate_type;
            switch (curr_def.immediate_type)
            {
            case IMM_TYPE_NONE:
                break;
            case IMM_TYPE_IMM12:
                instr_packet->immediate = imm12;
                break;
            case IMM_TYPE_IMM20:
                instr_packet->immediate = imm20;
                break;
            case IMM_TYPE_SIMM:
                instr_packet->immediate = simm;
                break;
            case IMM_TYPE_BIMM:
                instr_packet->immediate = bimm;
                break;
            case IMM_TYPE_JIMM20:
                instr_packet->immediate = jimm20;
                break;
            default:
                break;
            }
            return;
        }
    }
    instr_packet->decode_status = DECODE_UNSUCCESSFULL;
}

void CPUCore::start_simulation(uint64_t start_addr)
{
    core_pc = start_addr;
    continue_simulation = true;
    while (continue_simulation || core_pc < 0xFFFFFFFF)
    {
        execute();
    }
}
CPUCore::CPUCore(MMHandler *mhndler)
{
    mHandler = mhndler;
}