#include "cpu_core.h"

uint32_t CPUCore::get_register(uint32_t reg_no)
{
    return registers[reg_no];
}

uint32_t CPUCore::get_opcode(uint32_t addr)
{
    uint32_t opcode = 0;
    uint64_t phy_addr = addr;
#ifdef MMU_ENABLED
    phy_addr = mmu_translate(addr);
#endif
    mHandler->read_bytes(phy_addr, (uint8_t *)&opcode, 4);
    return opcode;
}

void CPUCore::set_register(uint32_t reg_no, uint32_t reg_val)
{
    registers[reg_no] = reg_val;
}
void CPUCore::execute()
{
    instruction_packet inst_packet;
    uint32_t opcode = get_opcode(core_pc);
    decode(opcode, &inst_packet);
    if (inst_packet.decode_status == DECODE_UNSUCCESSFULL || opcode == 0)
    {
        // cout << "----"
        //    << "decode failed" << endl;
        // continue_simulation = false;

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
    cout << " " << inst_packet.immediate << " " << endl;
    uint32_t reg_rs1 = registers[inst_packet.source_reg1];
    uint32_t reg_rs2 = registers[inst_packet.source_reg2];
    uint32_t reg_rd = 0;
    int imm = inst_packet.immediate;
    uint32_t next_pc = core_pc + 4;
    bool activate_exception = false;
    long long prod_result = 0;
    if (opcode == OPCODE_ZERO)
    {
        cout << "opcode = 0" << endl;
    }
    else
    {

        if (inst_packet.instruction_val == ENUM_INST_ANDI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "andi " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = reg_rs1 & imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_ADDI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "addi " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = reg_rs1 + imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SLTI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "slti " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = (signed)reg_rs1 < (signed)imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SLTIU)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sltiu " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = (unsigned)reg_rs1 & (unsigned)imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_ORI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "ori " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = reg_rs1 | imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_XORI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "xori " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = reg_rs1 ^ imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SLLI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "slli " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = reg_rs1 << imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SRLI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "srli " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = (unsigned)reg_rs1 >> imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SRAI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "srai " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = (signed)reg_rs1 & imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_LUI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "lui " << gpr_names[inst_packet.dst_reg] << ", " << imm << endl;
#endif
            reg_rd = imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_AUIPC)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "auipc " << gpr_names[inst_packet.dst_reg] << ", " << imm << endl;
#endif
            reg_rd = core_pc + imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_ADD)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "add " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = reg_rs1 + reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SUB)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sub " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = reg_rs1 - reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SLT)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "slt " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = (signed)reg_rs1 < (signed)reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SLTU)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sltu " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = (unsigned)reg_rs1 < (unsigned)reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_XOR)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "xor " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = reg_rs1 ^ reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_OR)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "or " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = reg_rs1 | reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_AND)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "and " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = reg_rs1 & reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_SLL)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sll " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = reg_rs1 >> (reg_rs2 & MAX_FIVEBIT);
        }
        else if (inst_packet.instruction_val == ENUM_INST_SRL)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "srl " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = (unsigned)reg_rs1 >> (reg_rs2 & MAX_FIVEBIT);
        }
        else if (inst_packet.instruction_val == ENUM_INST_SRA)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sra " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << endl;
#endif
            reg_rd = (signed)reg_rs1 >> (reg_rs2 & MAX_FIVEBIT);
        }
        else if (inst_packet.instruction_val == ENUM_INST_JAL)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "jal " << gpr_names[inst_packet.dst_reg] << ", " << imm << endl;
#endif
            reg_rd = next_pc;
            next_pc = core_pc + imm;
        }
        else if (inst_packet.instruction_val == ENUM_INST_JALR)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "jalr " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << ", " << imm << endl;
#endif
            reg_rd = next_pc;
            next_pc = ((reg_rs1 + imm) & (~1));
        }
        else if (inst_packet.instruction_val == ENUM_INST_BEQ)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "beg " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << "," << imm << endl;
#endif
            if (reg_rs1 == reg_rs2)
            {
                next_pc = next_pc + imm;
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_BNE)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "bne " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << "," imm << endl;
#endif

            if (reg_rs1 != reg_rs2)
            {
                next_pc = next_pc + imm;
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_BLT)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "blt " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << "," imm << endl;
#endif

            if (((signed)reg_rs1) < ((signed)reg_rs2))
            {
                next_pc = next_pc + imm;
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_BGE)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "bge " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << "," imm << endl;
#endif

            if ((signed)reg_rs1 >= (signed)reg_rs2)
            {
                next_pc = next_pc + imm;
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_BLTU)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "bltu " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << "," imm << endl;
#endif

            if ((unsigned)reg_rs1 < (unsigned)reg_rs2)
            {
                next_pc = next_pc + imm;
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_BGEU)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "bgeu " << gpr_names[inst_packet.source_reg1] << ", " << gpr_names[inst_packet.source_reg2] << "," imm << endl;
#endif

            if ((unsigned)reg_rs1 >= (unsigned)reg_rs2)
            {
                next_pc = next_pc + imm;
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_LB)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "lb " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                registers[inst_packet.dest_reg] = *((char *)mHandler->get_phy_pointer(phy_addr));
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_LH)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "lb " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                registers[inst_packet.dest_reg] = *((int16_t *)mHandler->get_phy_pointer(phy_addr));
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_LW)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "lb " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                registers[inst_packet.dest_reg] = *((int *)mHandler->get_phy_pointer(phy_addr));
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_LBU)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "lb " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                registers[inst_packet.dest_reg] = *((uint8_t *)mHandler->get_phy_pointer(phy_addr));
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_LHU)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "lhu " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                registers[inst_packet.dest_reg] = *((uint16_t *)mHandler->get_phy_pointer(phy_addr));
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_LWU)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "lwu " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                registers[inst_packet.dest_reg] = *((uint32_t *)mHandler->get_phy_pointer(phy_addr));
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_SB)
        {

#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sb " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                *((char *)mHandler->get_phy_pointer(phy_addr)) = registers[inst_packet.dest_reg];
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_SH)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sh " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                *((uint16_t *)mHandler->get_phy_pointer(phy_addr)) = registers[inst_packet.dest_reg];
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_SW)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sh " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," imm << endl;
#endif
            uint32_t phy_addr = 0;
            uint32_t virtual_addr = registers[inst_packet.source_reg1] + imm;
            activate_exception = mHandler->mmu_resolve(virtual_addr, &phy_addr);
            if (!activate_exception)
            {
                *((uint32_t *)mHandler->get_phy_pointer(phy_addr)) = registers[inst_packet.dest_reg];
            }
        }
        else if (inst_packet.instruction_val == ENUM_INST_ECALL)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "ecall " << endl;
#endif
            handle_exception(MCAUSE_ECALL_U + m_csr_mpriv, core_pc);
            activate_exception = true;
        }
        else if (inst_packet.instruction_val == ENUM_INST_EBREAK)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "ebreak" << endl;
#endif
            handle_exception(MCAUSE_BREAKPOINT, core_pc);
            activate_exception = true;
        }
        else if (inst_packet.instruction_val == ENUM_INST_MRET)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mret " << endl;
#endif
                //add code to handle machine mode trap return
        }
        else if (inst_packet.instruction_val == ENUM_INST_SRET)
        {  
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "sret " << endl;
#endif    //add code to handle supervisor mode trap return
        }
        else if (inst_packet.instruction_val == ENUM_INST_CSRRW)
        {  
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "csrw "<< core_pc << gpr_names[inst_packet.dst_reg] << ", " << reg_rs1<< "," << imm << endl;
#endif 
            registers[inst_packet.dest_reg] = csr_access(imm, reg_rs1, true, true, &core_pc);

        }
        else if (inst_packet.instruction_val == ENUM_INST_CSRRS)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "csrs "<< core_pc << gpr_names[inst_packet.dst_reg] << ", " << reg_rs1<<", "<< imm << endl;
#endif
            registers[inst_packet.dest_reg] = csr_access(imm, reg_rs1, true, false, &core_pc);
        }
        else if (inst_packet.instruction_val == ENUM_INST_CSRRC)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "csrs "<< core_pc << gpr_names[inst_packet.dst_reg] << ", " << reg_rs1<<", "<< imm << endl;
#endif
            registers[inst_packet.dest_reg] = csr_access(imm, reg_rs1, false, true, &core_pc);
        }
        else if (inst_packet.instruction_val == ENUM_INST_CSRRWI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "csrwi "<< core_pc << gpr_names[inst_packet.dst_reg] << ", " << inst_packet.source_reg1<<", "<< imm << endl;
#endif
            registers[inst_packet.dest_reg] = csr_access(imm, inst_packet.source_reg1, true, true, &core_pc);
        }
        else if (inst_packet.instruction_val == ENUM_INST_CSRRSI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "csrsi "<< core_pc << gpr_names[inst_packet.dst_reg] << ", " << inst_packet.source_reg1<<", "<< imm << endl;
#endif
            registers[inst_packet.dest_reg] = csr_access(imm, inst_packet.source_reg1, true, false, &core_pc);
        }
        else if (inst_packet.instruction_val == ENUM_INST_CSRRCI)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "csrci "<< core_pc << gpr_names[inst_packet.dst_reg] << ", " << inst_packet.source_reg1<<", "<< imm << endl;
#endif
            registers[inst_packet.dest_reg] = csr_access(imm, inst_packet.source_reg1, false, true, &core_pc);
        }
        else if (inst_packet.instruction_val == ENUM_INST_MUL)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif
            registers[inst_packet.dest_reg] = (signed)reg_rs1 * (signed)reg_rs2;
        }
        else if (inst_packet.instruction_val == ENUM_INST_MULH)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif
              prod_result = (((long long)(int) reg_rs1 )* ((long long)(int) reg_rs2));
              registers[reg_rd] = (int) (prod_result >> SHIFT_32);

        }
        else if (inst_packet.instruction_val == ENUM_INST_MULHSU)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif

              prod_result = (((long long)(int) reg_rs1 )* ((unsigned long long)(unsigned) reg_rs2));
              registers[reg_rd] = (int)(prod_result >> SHIFT_32);
        }
        else if (inst_packet.instruction_val == ENUM_INST_MULHU)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif
              prod_result = (((unsigned long long)(unsigned) reg_rs1 )* ((unsigned long long)(unsigned) reg_rs2));
              registers[reg_rd] = (int)(prod_result >> SHIFT_32);
        }
        else if (inst_packet.instruction_val == ENUM_INST_DIV)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif
            if ((signed)reg_rs1 == INT32_MIN && (signed)reg_rs2 == -1)
                  registers[reg_rd] = reg_rs1;
            else if (reg_rs2 != 0)
                  registers[reg_rd]= (signed)reg_rs1 / (signed)reg_rs2;
            else
                  registers[reg_rd]= (unsigned)-1;
        }
        else if (inst_packet.instruction_val == ENUM_INST_DIVU)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif

            if (reg_rs2 != 0)
                  registers[reg_rd]= (unsigned)reg_rs1 / (unsigned)reg_rs2;
            else
                  registers[reg_rd] = (unsigned)-1;
        }
        else if (inst_packet.instruction_val == ENUM_INST_REM)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif
            if ((signed)reg_rs1 == INT32_MIN && (signed)reg_rs2 == -1)
                  registers[reg_rd]  = 0;
            else if (reg_rs2 != 0)
                  registers[reg_rd]  = (signed)reg_rs1 % (signed)reg_rs2;
            else
                  registers[reg_rd]  = reg_rs1;
        }
        else if (inst_packet.instruction_val == ENUM_INST_REMU)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "mul " << gpr_names[inst_packet.dst_reg] << ", " << gpr_names[inst_packet.source_reg1] << "," <<gpr_names[inst_packet.source_reg2]  << endl;
#endif
            if (reg_rs2 != 0)
                  registers[reg_rd]  = (unsigned)reg_rs1 % (unsigned)reg_rs2;
            else
                  registers[reg_rd]  = reg_rs1;
        }
        else if (inst_packet.instruction_val == ENUM_INST_FENCE)
        {
#ifdef __PRINT_INSTR_
            cout << hex << core_pc << ": "
                 << "fence"<< endl;
#endif
        }
        else if (inst_packet.instruction_val == ENUM_INST_WFI)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOLR_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOSC_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOSWAP_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOADD_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOAND_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOXOR_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOMAX_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOMIN_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOMAXU_W)
        {
        }
        else if (inst_packet.instruction_val == ENUM_INST_AMOMINU_W)
        {
        }
        else
        {
            cout << "Unkown opcode: " << opcode << endl;
        }
    }
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

void CPUCore::start_simulation(uint32_t start_addr)
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