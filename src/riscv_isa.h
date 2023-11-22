
#ifndef __RISCV_ISA_H__
#define __RISCV_ISA_H__

//#define C_EXTENSION

//-----------------------------------------------------------------
// General:
//-----------------------------------------------------------------
#define REGISTERS               32

//--------------------------------------------------------------------
// Instruction Encoding
//--------------------------------------------------------------------
#define OPCODE_MAKE_MASK(a,b)   (((1ULL << ((a)+1ULL))-1) & \
                                ~((1ULL << (b))-1))

#define OPCODE_RD_SHIFT         7
#define OPCODE_RS1_SHIFT        15
#define OPCODE_RS2_SHIFT        20
#define OPCODE_HIMM12_SHIFT     20
#define OPCODE_TYPEI_IMM_SHIFT  20
#define OPCODE_TYPEU_IMM_SHIFT  12
#define OPCODE_SHAMT_SHIFT      20
#define OPCODE_AMOOP_SHIFT      27
#define OPCODE_FUNC7_SHIFT      25
#define OPCODE_FUNC3_SHIFT      12
#define OPCODE_AMO_RL_SHIFT     25
#define OPCODE_AMO_AQ_SHIFT     26

#define OPCODE_RD_MASK          OPCODE_MAKE_MASK(11, OPCODE_RD_SHIFT)
#define OPCODE_RS1_MASK         OPCODE_MAKE_MASK(19, OPCODE_RS1_SHIFT)
#define OPCODE_RS2_MASK         OPCODE_MAKE_MASK(24, OPCODE_RS2_SHIFT)
#define OPCODE_TYPEI_IMM_MASK   OPCODE_MAKE_MASK(31, OPCODE_TYPEI_IMM_SHIFT)
#define OPCODE_TYPEU_IMM_MASK   OPCODE_MAKE_MASK(31, OPCODE_TYPEU_IMM_SHIFT)
#define OPCODE_SHAMT_MASK       OPCODE_MAKE_MASK(25, OPCODE_SHAMT_SHIFT)
#define OPCODE_AMOOP_MASK       OPCODE_MAKE_MASK(31, OPCODE_AMOOP_SHIFT)
#define OPCODE_FUNC7_MASK       OPCODE_MAKE_MASK(31, OPCODE_FUNC7_SHIFT)
#define OPCODE_FUNC3_MASK       OPCODE_MAKE_MASK(14, OPCODE_FUNC3_SHIFT)

#define OPCODE_SHIFT_MASK(x,s,n) (((x) >> (s)) & ((1 << (n)) - 1))
#ifdef _MSC_VER
#define OPCODE_IMM_SIGN(x)       (-((int(x) >> 31) & 1))
#else
#define OPCODE_IMM_SIGN(x)       (-(((x) >> 31) & 1))
#endif

#define OPCODE_ITYPE_IMM(x)     ((OPCODE_SHIFT_MASK(x, 20, 12)          | \
                                 (OPCODE_IMM_SIGN(x) << 12)))
#define OPCODE_STYPE_IMM(x)     ((OPCODE_SHIFT_MASK(x, 7, 5)            | \
                                 (OPCODE_SHIFT_MASK(x, 25, 7) << 5)     | \
                                 (OPCODE_IMM_SIGN(x) << 12)))
#define OPCODE_SBTYPE_IMM(x)    ((OPCODE_SHIFT_MASK(x, 8, 4) << 1)      | \
                                 (OPCODE_SHIFT_MASK(x, 25, 6) << 5)     | \
                                 (OPCODE_SHIFT_MASK(x, 7, 1) << 11)     | \
                                 (OPCODE_IMM_SIGN(x) << 12))
#define OPCODE_UTYPE_IMM(x)     ((OPCODE_SHIFT_MASK(x, 12, 20)          | \
                                 (OPCODE_IMM_SIGN(x) << 20)))
#define OPCODE_UJTYPE_IMM(x)    ((OPCODE_SHIFT_MASK(x, 21, 10) << 1)    | \
                                 (OPCODE_SHIFT_MASK(x, 20, 1) << 11)    | \
                                 (OPCODE_SHIFT_MASK(x, 12, 8) << 12)    | \
                                 (OPCODE_IMM_SIGN(x) << 20))

#define OPCODE_ATOMIC_RL(X)     (((X) >> OPCODE_AMO_RL_SHIFT) & 1)
#define OPCODE_ATOMIC_AQ(X)     (((X) >> OPCODE_AMO_AQ_SHIFT) & 1)

//--------------------------------------------------------------------
// Instruction Encoding ("C" Standard extension)
//--------------------------------------------------------------------
#ifdef C_EXTENSION
#define OPCODE_C0_RD_SHIFT          2
#define OPCODE_C0_RD_MASK           OPCODE_MAKE_MASK(4,2)
#define OPCODE_C0_RS1_SHIFT         7
#define OPCODE_C0_RS1_MASK          OPCODE_MAKE_MASK(9,7)
#define OPCODE_C0_RS2_SHIFT         2
#define OPCODE_C0_RS2_MASK          OPCODE_MAKE_MASK(4,2)
#define OPCODE_C12_RD_SHIFT         7
#define OPCODE_C12_RD_MASK          OPCODE_MAKE_MASK(11, 7)
#define OPCODE_C12_RS1_SHIFT        7
#define OPCODE_C12_RS1_MASK         OPCODE_MAKE_MASK(11,7)
#define OPCODE_C12_RS2_SHIFT        2
#define OPCODE_C12_RS2_MASK         OPCODE_MAKE_MASK(6,2)

#define OPCODE_C_IMM_SIGN(x)        (-(((x) >> 12) & 1))

#define OPCODE_C_ITYPE_IMM(x)       ((OPCODE_SHIFT_MASK(x, 2, 5)            | \
                                     (OPCODE_SHIFT_MASK(x, 12, 1) << 5)     | \
                                     (OPCODE_C_IMM_SIGN(x) << 6)))
#define OPCODE_C_SSTYPE_IMM(x)      ((OPCODE_SHIFT_MASK(x, 7, 6)            | \
                                     (OPCODE_C_IMM_SIGN(x) << 6)))
#define OPCODE_C_IWTYPE_IMM(x)      ((OPCODE_SHIFT_MASK(x, 5, 8)            | \
                                     (OPCODE_C_IMM_SIGN(x) << 8)))
#define OPCODE_C_LTYPE_IMM(x)       ((OPCODE_SHIFT_MASK(x, 5, 2)            | \
                                     (OPCODE_SHIFT_MASK(x, 10, 3) << 2)     | \
                                     (OPCODE_C_IMM_SIGN(x) << 5)))
#define OPCODE_C_STYPE_IMM(x)       ((OPCODE_SHIFT_MASK(x, 5, 2)            | \
                                     (OPCODE_SHIFT_MASK(x, 10, 3) << 2)     | \
                                     (OPCODE_C_IMM_SIGN(x) << 5)))
#define OPCODE_C_BTYPE_IMM(x)       ((OPCODE_SHIFT_MASK(x, 2, 5)            | \
                                     (OPCODE_SHIFT_MASK(x, 10, 3) << 5)     | \
                                     (OPCODE_C_IMM_SIGN(x) << 8)))
#define OPCODE_C_JTYPE_IMM(x)       ((OPCODE_SHIFT_MASK(x, 2, 11)           | \
                                     (OPCODE_C_IMM_SIGN(x) << 11)))

#define OPCODE_C_ADDI4SPN_IMM_REODER(x) ((OPCODE_SHIFT_MASK(x, 1, 1) << 2)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 1) << 3)  | \
                                         (OPCODE_SHIFT_MASK(x, 6, 2) << 4)  | \
                                         (OPCODE_SHIFT_MASK(x, 2, 4) << 6))
#define OPCODE_C_ADDI16SP_IMM_REODER(x) ((OPCODE_SHIFT_MASK(x, 4, 1) << 4)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 1) << 5)  | \
                                         (OPCODE_SHIFT_MASK(x, 3, 1) << 6)  | \
                                         (OPCODE_SHIFT_MASK(x, 1, 2) << 7)  | \
                                         (OPCODE_SHIFT_MASK(x, 5, 27) << 9)) 
#define OPCODE_C_LW_IMM_REODER(x)       ((OPCODE_SHIFT_MASK(x, 1, 1) << 2)  | \
                                         (OPCODE_SHIFT_MASK(x, 2, 3) << 3)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 1) << 6)) 
#define OPCODE_C_SW_IMM_REODER(x)       ((OPCODE_SHIFT_MASK(x, 1, 1) << 2)  | \
                                         (OPCODE_SHIFT_MASK(x, 2, 3) << 3)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 1) << 6)) 
#define OPCODE_C_LWSP_IMM_REODER(x)     ((OPCODE_SHIFT_MASK(x, 2, 3) << 2)  | \
                                         (OPCODE_SHIFT_MASK(x, 5, 1) << 5)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 2) << 6)) 
#define OPCODE_C_SWSP_IMM_REODER(x)     ((OPCODE_SHIFT_MASK(x, 2, 3) << 2)  | \
                                         (OPCODE_SHIFT_MASK(x, 5, 1) << 5)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 2) << 6)) 
#define OPCODE_C_JUMP_IMM_REODER(x)     ((OPCODE_SHIFT_MASK(x, 1, 3) << 1)  | \
                                         (OPCODE_SHIFT_MASK(x, 9, 1) << 4)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 1) << 5)  | \
                                         (OPCODE_SHIFT_MASK(x, 5, 1) << 6)  | \
                                         (OPCODE_SHIFT_MASK(x, 4, 1) << 7)  | \
                                         (OPCODE_SHIFT_MASK(x, 7, 2) << 8)  | \
                                         (OPCODE_SHIFT_MASK(x, 6, 1) << 10) | \
                                         (OPCODE_SHIFT_MASK(x, 10, 22) << 11))
#define OPCODE_C_BRANCH_IMM_REODER(x)   ((OPCODE_SHIFT_MASK(x, 1, 2) << 1)  | \
                                         (OPCODE_SHIFT_MASK(x, 5, 2) << 3)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 1) << 5)  | \
                                         (OPCODE_SHIFT_MASK(x, 3, 2) << 6)  | \
                                         (OPCODE_SHIFT_MASK(x, 7, 25) << 8)) 

#define OPCODE_C_CMD_IMM_REODER(x)      ((OPCODE_SHIFT_MASK(x, 1, 1) << 3)  | \
                                         (OPCODE_SHIFT_MASK(x, 0, 1) << 4)  | \
                                         (OPCODE_SHIFT_MASK(x, 6, 2) << 5)  | \
                                         (OPCODE_SHIFT_MASK(x, 2, 4) << 7))
#endif

//--------------------------------------------------------------------
// Instruction Decode Groups
//--------------------------------------------------------------------
// Mask 0x707f
//   andi, addi, slti, sltiu, ori, xori, jalr, beq, bne, blt, bge, 
//   bltu, bgeu, lb, lh, lw, lbu, lhu, lwu, sb, sh, sw,
//   csrrs, csrrc, csrrsi, csrrci

// Mask 0xffffffff
//   scall, sbreak, sret

// Mask 0xfe00707f
//   add, sub, slt, sltu, xor, or, and, sll, srl, sra, mul, mulh,
//   mulhsu, mulhu, div, divu, rem, remu

// Mask 0xfc00707f
//   slli, srli, srai

// Mask 0x7f
//   lui, auipc, jal

//--------------------------------------------------------------------
// Instructions
//--------------------------------------------------------------------
enum eInstructions
{
    ENUM_INST_ANDI,
    ENUM_INST_ADDI,
    ENUM_INST_SLTI,
    ENUM_INST_SLTIU,
    ENUM_INST_ORI,
    ENUM_INST_XORI,
    ENUM_INST_SLLI,
    ENUM_INST_SRLI,
    ENUM_INST_SRAI,
    ENUM_INST_LUI,
    ENUM_INST_AUIPC,
    ENUM_INST_ADD,
    ENUM_INST_SUB,
    ENUM_INST_SLT,
    ENUM_INST_SLTU,
    ENUM_INST_XOR,
    ENUM_INST_OR,
    ENUM_INST_AND,
    ENUM_INST_SLL,
    ENUM_INST_SRL,
    ENUM_INST_SRA,
    ENUM_INST_JAL,
    ENUM_INST_JALR,
    ENUM_INST_BEQ,
    ENUM_INST_BNE,
    ENUM_INST_BLT,
    ENUM_INST_BGE,
    ENUM_INST_BLTU,
    ENUM_INST_BGEU,
    ENUM_INST_LB,
    ENUM_INST_LH,
    ENUM_INST_LW,
    ENUM_INST_LBU,
    ENUM_INST_LHU,
    ENUM_INST_LWU,
    ENUM_INST_SB,
    ENUM_INST_SH,
    ENUM_INST_SW,
    ENUM_INST_ECALL,
    ENUM_INST_EBREAK,
    ENUM_INST_MRET,
    ENUM_INST_SRET,
    ENUM_INST_CSRRW,
    ENUM_INST_CSRRS,
    ENUM_INST_CSRRC,
    ENUM_INST_CSRRWI,
    ENUM_INST_CSRRSI,
    ENUM_INST_CSRRCI,
    ENUM_INST_MUL,
    ENUM_INST_MULH,
    ENUM_INST_MULHSU,
    ENUM_INST_MULHU,
    ENUM_INST_DIV,
    ENUM_INST_DIVU,
    ENUM_INST_REM,
    ENUM_INST_REMU,
    ENUM_INST_FENCE,
    ENUM_INST_WFI,
#ifdef CONFIG_RVA
    ENUM_INST_LR_W,
    ENUM_INST_SC_W,
    ENUM_INST_AMOSWAP_W,
    ENUM_INST_AMOADD_W,
    ENUM_INST_AMOAND_W,
    ENUM_INST_AMOOR_W,
    ENUM_INST_AMOXOR_W,
    ENUM_INST_AMOMAX_W,
    ENUM_INST_AMOMIN_W,
#endif
#ifdef C_EXTENSION
    //"C" standard extension
    ENUM_INST_C_ADDI4SPN,
    ENUM_INST_C_LW,
    ENUM_INST_C_SW,
    ENUM_INST_C_ADDI,
    ENUM_INST_C_JAL,
    ENUM_INST_C_LI,
    ENUM_INST_C_ADDI16SP,
    ENUM_INST_C_LUI,
    ENUM_INST_C_SRLI,
    ENUM_INST_C_SRAI,
    ENUM_INST_C_ANDI,
    ENUM_INST_C_SUB,
    ENUM_INST_C_XOR,
    ENUM_INST_C_OR,
    ENUM_INST_C_AND,
    ENUM_INST_C_J,
    ENUM_INST_C_BEQZ,
    ENUM_INST_C_BNEZ,
    ENUM_INST_C_SLLI,
    ENUM_INST_C_LWSP,
    ENUM_INST_C_JR,
    ENUM_INST_C_MV,
    ENUM_INST_C_JALR,
    ENUM_INST_C_ADD,
    ENUM_INST_C_SWSP,
    //Vishare added instruction
    ENUM_INST_LPRCFG,
    ENUM_INST_C_XVEC,
	ENUM_INST_C_WAIT,
    ENUM_INST_C_TSKARG,
	ENUM_INST_C_DMAINIT,
	ENUM_INST_C_DMALPRC,
	ENUM_INST_C_DMAPINC,
	ENUM_INST_C_DMASTRD,
#endif
    ENUM_INST_MAX
};

#define INST_GRP_LOAD       0x03    // lb, lh, lw, lbu, lhu
#define INST_GRP_FENCE      0x0F    // fence.*
#define INST_GRP_ALUIMM     0x13    // addi, slti, sltiu, xori, ori, andi,
                                    // slli, srli, srai
#define INST_GRP_AUIPC      0x17    // auipc
#define INST_GRP_STORE      0x23    // sb, sh, sw
#define INST_GRP_AMO        0x2F    // atomic memory access instructions
#define INST_GRP_ALUREG     0x33    // add, sub, sll, slt, sltu,
                                    // xor, srl, sra, or, and,
                                    // mul, mulh, mulhsu, mulhu,
                                    // div, divu, rem, remu
#define INST_GRP_LUI        0x37    // lui
#define INST_GRP_CMPJMP     0x63    // beq, bne, blt, bge, bltu, bgeu
#define INST_GRP_JALR       0x67    // jalr
#define INST_GRP_JAL        0x6F    // jal
#define INST_GRP_SYSTEM     0x73    // ecall, ebreak, csrr*

// andi
#define INST_ANDI           0x7013
#define INST_ANDI_MASK      0x707f

// addi
#define INST_ADDI           0x13
#define INST_ADDI_MASK      0x707f

// slti
#define INST_SLTI           0x2013
#define INST_SLTI_MASK      0x707f

// sltiu
#define INST_SLTIU          0x3013
#define INST_SLTIU_MASK     0x707f

// ori
#define INST_ORI            0x6013
#define INST_ORI_MASK       0x707f

// xori
#define INST_XORI           0x4013
#define INST_XORI_MASK      0x707f

// slli
#define INST_SLLI           0x1013
#define INST_SLLI_MASK      0xfc00707f

// srli
#define INST_SRLI           0x5013
#define INST_SRLI_MASK      0xfc00707f

// srai
#define INST_SRAI           0x40005013
#define INST_SRAI_MASK      0xfc00707f

// lui
#define INST_LUI            0x37
#define INST_LUI_MASK       0x7f

// auipc
#define INST_AUIPC          0x17
#define INST_AUIPC_MASK     0x7f

// add
#define INST_ADD            0x33
#define INST_ADD_MASK       0xfe00707f

// sub
#define INST_SUB            0x40000033
#define INST_SUB_MASK       0xfe00707f

// slt
#define INST_SLT            0x2033
#define INST_SLT_MASK       0xfe00707f

// sltu
#define INST_SLTU           0x3033
#define INST_SLTU_MASK      0xfe00707f

// xor
#define INST_XOR            0x4033
#define INST_XOR_MASK       0xfe00707f

// or
#define INST_OR             0x6033
#define INST_OR_MASK        0xfe00707f

// and
#define INST_AND            0x7033
#define INST_AND_MASK       0xfe00707f

// sll
#define INST_SLL            0x1033
#define INST_SLL_MASK       0xfe00707f

// srl
#define INST_SRL            0x5033
#define INST_SRL_MASK       0xfe00707f

// sra
#define INST_SRA            0x40005033
#define INST_SRA_MASK       0xfe00707f

// jal
#define INST_JAL            0x6f
#define INST_JAL_MASK       0x7f

// jalr
#define INST_JALR           0x67
#define INST_JALR_MASK      0x707f

// beq
#define INST_BEQ            0x63
#define INST_BEQ_MASK       0x707f

// bne
#define INST_BNE            0x1063
#define INST_BNE_MASK       0x707f

// blt
#define INST_BLT            0x4063
#define INST_BLT_MASK       0x707f

// bge
#define INST_BGE            0x5063
#define INST_BGE_MASK       0x707f

// bltu
#define INST_BLTU           0x6063
#define INST_BLTU_MASK      0x707f

// bgeu
#define INST_BGEU           0x7063
#define INST_BGEU_MASK      0x707f

// lb
#define INST_LB             0x03
#define INST_LB_MASK        0x707f

// lh
#define INST_LH             0x1003
#define INST_LH_MASK        0x707f

// lw
#define INST_LW             0x2003
#define INST_LW_MASK        0x707f

// lbu
#define INST_LBU            0x4003
#define INST_LBU_MASK       0x707f

// lhu
#define INST_LHU            0x5003
#define INST_LHU_MASK       0x707f

// lwu
#define INST_LWU            0x6003
#define INST_LWU_MASK       0x707f

// sb
#define INST_SB             0x23
#define INST_SB_MASK        0x707f

// sh
#define INST_SH             0x1023
#define INST_SH_MASK        0x707f

// sw
#define INST_SW             0x2023
#define INST_SW_MASK        0x707f

// ecall
#define INST_ECALL          0x73
#define INST_ECALL_MASK     0xffffffff

// ebreak
#define INST_EBREAK         0x100073
#define INST_EBREAK_MASK    0xffffffff

// sfence
#define INST_SFENCE         0x12000073
#define INST_SFENCE_MASK    0xfe007fff

// fence
#define INST_FENCE          0x0f
#define INST_FENCE_MASK     0x707f

// ifence
#define INST_IFENCE         0x100f
#define INST_IFENCE_MASK    0x707f

// mret
#define INST_MRET           0x30200073
#define INST_MRET_MASK      0xffffffff

// sret
#define INST_SRET           0x10200073
#define INST_SRET_MASK      0xffffffff

// uret
//#define INST_URET 0x200073
//#define INST_URET_MASK 0xffffffff

// csrrw
#define INST_CSRRW          0x1073
#define INST_CSRRW_MASK     0x707f

// csrrs
#define INST_CSRRS          0x2073
#define INST_CSRRS_MASK     0x707f

// csrrc
#define INST_CSRRC          0x3073
#define INST_CSRRC_MASK     0x707f

// csrrwi
#define INST_CSRRWI         0x5073
#define INST_CSRRWI_MASK    0x707f

// csrrsi
#define INST_CSRRSI         0x6073
#define INST_CSRRSI_MASK    0x707f

// csrrci
#define INST_CSRRCI         0x7073
#define INST_CSRRCI_MASK    0x707f

// mul
#define INST_MUL            0x2000033
#define INST_MUL_MASK       0xfe00707f

// mulh
#define INST_MULH           0x2001033
#define INST_MULH_MASK      0xfe00707f

// mulhsu
#define INST_MULHSU         0x2002033
#define INST_MULHSU_MASK    0xfe00707f

// mulhu
#define INST_MULHU          0x2003033
#define INST_MULHU_MASK     0xfe00707f

// div
#define INST_DIV            0x2004033
#define INST_DIV_MASK       0xfe00707f

// divu
#define INST_DIVU           0x2005033
#define INST_DIVU_MASK      0xfe00707f

// rem
#define INST_REM            0x2006033
#define INST_REM_MASK       0xfe00707f

// remu
#define INST_REMU           0x2007033
#define INST_REMU_MASK      0xfe00707f

// wfi
#define INST_WFI            0x10500073
#define INST_WFI_MASK       0xffff8fff

#define INST_HIMM12_ECALL   0x0000
#define INST_HIMM12_EBREAK  0x0001
#define INST_HIMM12_SRET    0x0102
#define INST_HIMM12_WFI     0x0105
#define INST_HIMM12_SFENCE  0x0120
#define INST_HIMM12_MRET    0x0302

#ifdef CONFIG_RVA
#define INST_AMO_OPCODE     (0x2f <<  0)
#define INST_AMO_FUNC3      (0x02 << 12)
#define INST_AMO            (INST_AMO_FUNC3 | INST_AMO_OPCODE)
#define INST_AMO_MASK       ((MASK(3) << 12) | MASK(7))

#define AMO_OP_LR           0x02
#define AMO_OP_SC           0x03
#define AMO_OP_AMOSWAP      0x01
#define AMO_OP_AMOADD       0x00
#define AMO_OP_AMOXOR       0x04
#define AMO_OP_AMOOR        0x08
#define AMO_OP_AMOAND       0x0C
#define AMO_OP_AMOMIN       0x10
#define AMO_OP_AMOMAX       0x14
#define AMO_OP_AMOMINU      0x18
#define AMO_OP_AMOMAXU      0x1C
#define AMO_FLAG_RL         (1U << 25)
#define AMO_FLAG_AQ         (1U << 26)
#define AMO_INVALID_COOKIE  (~0U)
#endif // CONFIG_RVA

#ifdef C_EXTENSION
// c.addi4spn
#define INST_C_ADDI4SPN      0x0000
#define INST_C_ADDI4SPN_MASK 0xe003  
// c.lw
#define INST_C_LW            0x4000
#define INST_C_LW_MASK       0xe003  
// c.sw
#define INST_C_SW            0xc000
#define INST_C_SW_MASK       0xe003
// c.addi
#define INST_C_ADDI          0x0001
#define INST_C_ADDI_MASK     0xe003
// c.jal
#define INST_C_JAL           0x2001
#define INST_C_JAL_MASK      0xe003
// c.li
#define INST_C_LI            0x4001
#define INST_C_LI_MASK       0xe003
// c.addi16sp
#define INST_C_ADDI16SP      0x6101
#define INST_C_ADDI16SP_MASK 0xef83
// c.lui
#define INST_C_LUI           0x6001
#define INST_C_LUI_MASK      0xe003
// c.j
#define INST_C_J             0xa001
#define INST_C_J_MASK        0xe003
// c.beqz
#define INST_C_BEQZ          0xc001
#define INST_C_BEQZ_MASK     0xe003
// c.bnez
#define INST_C_BNEZ          0xe001
#define INST_C_BNEZ_MASK     0xe003
// c.slli
#define INST_C_SLLI          0x0002
#define INST_C_SLLI_MASK     0xe003
// c.lwsp
#define INST_C_LWSP          0x4002
#define INST_C_LWSP_MASK     0xe003
// c.swsp
#define INST_C_SWSP          0xc002
#define INST_C_SWSP_MASK     0xe003
// c.srli
#define INST_C_SRLI          0x8001
#define INST_C_SRLI_MASK     0xec03
// c.srai
#define INST_C_SRAI          0x8401
#define INST_C_SRAI_MASK     0xec03
// c.andi
#define INST_C_ANDI          0x8801
#define INST_C_ANDI_MASK     0xec03
// c.sub
#define INST_C_SUB           0x8c01
#define INST_C_SUB_MASK      0xfc63
// c.xor
#define INST_C_XOR           0x8c21
#define INST_C_XOR_MASK      0xfc63
// c.or
#define INST_C_OR            0x8c41
#define INST_C_OR_MASK       0xfc63
// c.and
#define INST_C_AND           0x8c61
#define INST_C_AND_MASK      0xfc63
// c.jr
#define INST_C_JR            0x8002
#define INST_C_JR_MASK       0xf07f
// c.jalr
#define INST_C_JALR          0x9002
#define INST_C_JALR_MASK     0xf07f
// c.mv
#define INST_C_MV            0x8002
#define INST_C_MV_MASK       0xf003
// c.add
#define INST_C_ADD           0x9002
#define INST_C_ADD_MASK      0xf003
//lprcfg
#define INST_LPRCFG          0xb
#define INST_LPRCFG_MASK     0x7f
//c.xvec
#define INST_C_XVEC          0x8000
#define INST_C_XVEC_MASK     0xe003
//c.wait
#define INST_C_WAIT          0xa000
#define INST_C_WAIT_MASK     0xe003
//c.tskarg
#define INST_C_TSKARG        0x2002
#define INST_C_TSKARG_MASK   0xe003
//c.dmainit
#define INST_C_DMAINIT       0x2000
#define INST_C_DMAINIT_MASK  0xe003
//c.dmalprc
#define INST_C_DMALPRC       0x6002
#define INST_C_DMALPRC_MASK  0xe003
//c.dmapinc
#define INST_C_DMAPINC       0xa002
#define INST_C_DMAPINC_MASK  0xe003
//c.dmastride
#define INST_C_DMASTRD       0xe002
#define INST_C_DMASTRD_MASK  0xe003
#endif //C_EXTENSION

#define IS_LOAD_INST(a)     (((a)  & 0x7F) == 0x03)
#define IS_STORE_INST(a)    (((a)  & 0x7F) == 0x23)
#define IS_BRANCH_INST(a)   ((((a) & 0x7F) == 0x6f)  || \
                            (((a)  & 0x7F) == 0x67)  || \
                            (((a)  & 0x7F) == 0x63)  || \
                            (((a)  & INST_ECALL_MASK) == INST_ECALL)   || \
                            (((a)  & INST_EBREAK_MASK) == INST_EBREAK) || \
                            (((a)  & INST_MRET_MASK) == INST_MRET))

#define IS_ALU_3R_INST(a)           (((a)  & 0x7F)   == 0x33)
#define IS_ALU_2RI_INST(a)          ((((a) & 0x7F)   == 0x13)     || \
                                    (((a)  & 0x7F)   == 0x67))
#define IS_COND_BRANCH_2RI_INST(a) \
                            (((a) & 0x7F) == 0x63)
#define IS_RD_I_INST(a)     (((a) & INST_JAL_MASK)   == INST_JAL) || \
                            (((a) & INST_LUI_MASK)   == INST_LUI) || \
                            (((a) & INST_AUIPC_MASK) == INST_AUIPC))

//--------------------------------------------------------------------
// Privilege levels
//--------------------------------------------------------------------
#define PRIV_USER         0
#define PRIV_SUPER        1
#define PRIV_MACHINE      3

//--------------------------------------------------------------------
// Status Register
//--------------------------------------------------------------------
#define SR_UIE          (1 << 0)
#define SR_SIE          (1 << 1)
#define SR_MIE          (1 << 3)
#define SR_UPIE         (1 << 4)
#define SR_SPIE         (1 << 5)
#define SR_MPIE         (1 << 7)
#define SR_SPP          (1 << 8)

#define SR_MPP_SHIFT    11
#define SR_MPP_MASK     0x3
#define SR_MPP          (SR_MPP_MASK  << SR_MPP_SHIFT)
#define SR_MPP_U        (PRIV_USER    << SR_MPP_SHIFT)
#define SR_MPP_S        (PRIV_SUPER   << SR_MPP_SHIFT)
#define SR_MPP_M        (PRIV_MACHINE << SR_MPP_SHIFT)

#define SR_GET_MPP(val) (((val) >> SR_MPP_SHIFT) & SR_MPP_MASK)

#define SR_SUM          (1 << 18)

#define SR_SMODE_MASK   (SR_UIE | SR_SIE | SR_UPIE | SR_SPIE | SR_SPP | SR_SUM)

//--------------------------------------------------------------------
// IRQ Numbers
//--------------------------------------------------------------------
#define IRQ_S_SOFT              1
#define IRQ_M_SOFT              3
#define IRQ_S_TIMER             5
#define IRQ_M_TIMER             7
#define IRQ_S_EXT               9
#define IRQ_M_EXT               11
#define IRQ_MIN                 (IRQ_S_SOFT)
#define IRQ_MAX                 (IRQ_M_EXT + 1)
#define IRQ_MASK                ((1 << IRQ_M_EXT)   | (1 << IRQ_S_EXT)   | \
                                 (1 << IRQ_M_TIMER) | (1 << IRQ_S_TIMER) | \
                                 (1 << IRQ_M_SOFT)  | (1 << IRQ_S_SOFT))

#define SR_IP_MSIP              (1 << IRQ_M_SOFT)
#define SR_IP_MTIP              (1 << IRQ_M_TIMER)
#define SR_IP_MEIP              (1 << IRQ_M_EXT)
#define SR_IP_SSIP              (1 << IRQ_S_SOFT)
#define SR_IP_STIP              (1 << IRQ_S_TIMER)
#define SR_IP_SEIP              (1 << IRQ_S_EXT)

//--------------------------------------------------------------------
// SATP CSR bits
//--------------------------------------------------------------------
#define SATP_PPN_SHIFT          0
#define SATP_PPN_MASK           0x3FFFFF
#define SATP_ASID_SHIFT         22
#define SATP_ASID_MASK          0x1FF
#define SATP_MODE               0x80000000

//--------------------------------------------------------------------
// CSR Registers - Simulation control
//--------------------------------------------------------------------
#define CSR_DSCRATCH                        0x7b2
#define CSR_DSCRATCH_MASK                   0xFFFFFFFF
#define CSR_SIM_CTRL                        0x8b2
#define CSR_SIM_CTRL_MASK                   0xFFFFFFFF
#define CSR_SIM_CTRL_EXIT                   (0  << 24)
#define CSR_SIM_CTRL_PUTC                   (1  << 24)
#define CSR_SIM_CTRL_GETC                   (2  << 24)
#define CSR_SIM_CTRL_TRACE                  (4  << 24)
#define CSR_SIM_PRINTF                      (5  << 24)
#define CSR_SIM_CTRL_WRITE 		            (6  << 24)  // SYS_write
#define CSR_SIM_CTRL_READ  		            (7  << 24)  // SYS_read
#define CSR_SIM_CTRL_OPEN  		            (8  << 24)  // SYS_open
#define CSR_SIM_CTRL_CLOSE 		            (9  << 24)  // SYS_close
#define CSR_SIM_CTRL_ETH_SEND               (10 << 24)
#define CSR_SIM_CTRL_ETH_RECV               (11 << 24)

//--------------------------------------------------------------------
// CSR Registers - Thread control
//--------------------------------------------------------------------
#define CSR_THREAD_START                    0x800
#define CSR_THREAD_START_MASK               0xFFFFFFFF
#define CSR_THREAD_PC                       0x801
#define CSR_THREAD_PC_MASK                  0xFFFFFFFF
#define CSR_THREAD_STACKADDR                0x802
#define CSR_THREAD_STACKADDR_MASK           0xFFFFFFFF
#define CSR_THREAD_STACKSIZE                0x803
#define CSR_THREAD_STACKSIZE_MASK           0xFFFFFFFF
#define CSR_THREAD_ARG                      0x804
#define CSR_THREAD_ARG_MASK                 0xFFFFFFFF
#define CSR_THREAD_SCHEDCORE                0x805
#define CSR_THREAD_SCHEDCORE_MASK           0xFFFFFFFF
#define CSR_THREAD_PID                      0x806
#define CSR_THREAD_PID_MASK                 0xFFFFFFFF
#define CSR_THREAD_DONE                     0x807
#define CSR_THREAD_DONE_MASK                0xFFFFFFFF

//--------------------------------------------------------------------
// CSR Registers - Hardware Function control
//--------------------------------------------------------------------
#define CSR_HW_FUNC_START                   0x808
#define CSR_HW_FUNC_START_MASK              0xFFFFFFFF
#define CSR_HW_FUNC_ARG                     0x809
#define CSR_HW_FUNC_ARG_MASK                0xFFFFFFFF


//--------------------------------------------------------------------
// Kernel thread scheduling
//--------------------------------------------------------------------
#define CSR_INIT_KERNEL_PC                  0x80a
#define CSR_KERNEL_GET_NEXT_THREAD          0x80b
#define CSR_KERNEL_GET_NEXT_PAGE            0x80c
#define CSR_MPR_USE_NEXT_PAGE               0x80d
#define CSR_KERNEL_ACTIVATE_THREAD          0x80e
#define CSR_THREAD_START_V2                 0x80f
#define CSR_INIT_KERNEL_STACK_POINTER       0x810
#define CSR_KERNEL_IRP_STORE                0x811
#define CSR_KERNEL_IRP_LOAD                 0x812
#define CSR_THREAD_JOIN                     0x813
#define CSR_MUTEX_LOCK                      0x814
#define CSR_MUTEX_UNLOCK                    0x815
#define CSR_THREAD_COND_UNLOCK_AND_WAIT     0x816
#define CSR_THREAD_COND_SIGNAL              0x817
#define CSR_THREAD_EXIT                     0x81a
#define CSR_GET_SPECIAL_TRIGGER_REGISTER    0x81b
#define CSR_SET_SPECIAL_TRIGGER_REGISTER    0x81c
#define CSR_SET_BREAKPOINT                  0x81d
#define CSR_CLEAR_BREAKPOINT                0x81e
#define CSR_GET_BREAKPOINT_INDEX            0x81f
#define CSR_SET_THREAD_SCHEDULE_COUNTER     0x820

//--------------------------------------------------------------------
// Mutex Cache
//--------------------------------------------------------------------
#define CSR_MCACHE_LOCK                     0x822
#define CSR_MCACHE_UNLOCK                   0x823
#define CSR_MCACHE_CORES_WRITE_DATA         0x824
#define CSR_MCACHE_CORES_REGISTER           0x825

#define CSR_HW_FUNC_GET_TRIGGER             0x826
#define CSR_HW_FUNC_CLR_TRIGGER             0x827
#define CSR_HW_FUNC_SET_RESPONSE            0x828

#define CSR_TRACE_RESET                     0x830
#define CSR_TRACE_SET_TRIGGER               0x831
#define CSR_TRACE_SET_DELAY                 0x832
#define CSR_TRACE_SET_SIZE                  0x833
#define CSR_TRACE_POP                       0x834

#define CSR_KERNEL_GET_NEXT_PRIORITY_THREAD  0x835
#define CSR_DISABLE_KERNEL_TIRRGER           0x836
#define CSR_COMMANDER_PC                     0x837
#define CSR_COMMANDER_STOP                   0x838
#define CSR_COMMANDER_INST                   0x839
#define CSR_COMMANDER_RETURN                 0x83a

#define CSR_IOBUF_INIT                      0x840
#define CSR_IOBUF_TRIGGER                   0x841
#define CSR_IOBUF_PUSH                      0x842
#define CSR_IOBUF_POP                       0x843
#define CSR_IOBUF_FULL                      0x844
#define CSR_IOBUF_EMPTY                     0x845

#define CSR_SET_MPU                         0x850
#define CSR_CLEAR_MPU                       0x851
#define CSR_GET_MPU                         0x852

//--------------------------------------------------------------------
// Misc
//--------------------------------------------------------------------
#define CSR_PRINT_ACTIVE_THREADS_STATS      0x890
#define CSR_THREAD_SELF                     0x891
#define CSR_HOST_FUNC_CALL                  0x892
#define CSR_X86_FUNC_CALL                   CSR_HOST_FUNC_CALL

//--------------------------------------------------------------------
// CSR Registers - Machine
//--------------------------------------------------------------------
#define CSR_MSTATUS             0x300
#define CSR_MSTATUS_MASK        0xFFFFFFFF
#define CSR_MISA                0x301
#define CSR_MISA_MASK           0xFFFFFFFF
#define CSR_MEDELEG             0x302
#define CSR_MEDELEG_MASK        0xFFFFFFFF
#define CSR_MIDELEG             0x303
#define CSR_MIDELEG_MASK        0xFFFFFFFF
#define CSR_MIE                 0x304
#define CSR_MIE_MASK            IRQ_MASK
#define CSR_MTVEC               0x305
#define CSR_MTVEC_MASK          0xFFFFFFFF
#define CSR_MSCRATCH            0x340
#define CSR_MSCRATCH_MASK       0xFFFFFFFF
#define CSR_MEPC                0x341
#define CSR_MEPC_MASK           0xFFFFFFFF
#define CSR_MCAUSE              0x342
#define CSR_MCAUSE_MASK         0x8000000F
#define CSR_MTVAL               0x343         // Was disabled
#define CSR_MTVAL_MASK          0xFFFFFFFF    // Was disabled
#define CSR_MIP                 0x344
#define CSR_MIP_MASK            IRQ_MASK
#define CSR_CYCLE               0xc00
#define CSR_CYCLE_MASK          0xFFFFFFFF
#define CSR_MTIME               0xc01
#define CSR_MTIME_MASK          0xFFFFFFFFLLU
#define CSR_CYCLEH              0xc80
#define CSR_CYCLEH_MASK         0xFFFFFFFF
#define CSR_MTIMEH              0xc81
#define CSR_MTIMEH_MASK         0xFFFFFFFF
#define CSR_MTIMECMP            0x7c0
#define CSR_MTIMECMP_MASK       0xFFFFFFFF
#define CSR_MVENDORID           0xF11
#define CSR_MVENDORID_MASK      0xFFFFFFFF
#define MVENDORID_VALUE         0
#define CSR_MARCHID             0xF12
#define CSR_MARCHID_MASK        0xFFFFFFFF
#define MARCHID_VALUE           0
#define CSR_MIMPID              0xF13
#define CSR_MIMPID_MASK         0xFFFFFFFF
#define MIMPID_VALUE            0
#define CSR_MHARTID             0xF14
#define CSR_MHARTID_MASK        0xFFFFFFFF
#define MHARTID_VALUE           0
#define CSR_MCYCLE              0xB00
#define CSR_MCYCLE_MASK         0xFFFFFFFF
#define CSR_MCYCLEH             0xB80
#define CSR_MCYCLEH_MASK        0xFFFFFFFF

#define CSR_PMPCFG0             0x3a0 // pmpcfg0
#define CSR_PMPCFG0_MASK        0xFFFFFFFF
#define CSR_PMPCFG1             0x3a1 // pmpcfg1
#define CSR_PMPCFG1_MASK        0xFFFFFFFF
#define CSR_PMPCFG2             0x3a2 // pmpcfg2
#define CSR_PMPCFG2_MASK        0xFFFFFFFF
#define CSR_PMPADDR0            0x3b0 // pmpaddr0
#define CSR_PMPADDR0_MASK       0xFFFFFFFF

#define CSR_DFLUSH              CSR_PMPCFG0
#define CSR_DFLUSH_MASK         CSR_PMPCFG0_MASK
#define CSR_DWRITEBACK          CSR_PMPCFG1
#define CSR_DWRITEBACK_MASK     CSR_PMPCFG1_MASK
#define CSR_DINVALIDATE         CSR_PMPCFG2
#define CSR_DINVALIDATE_MASK    CSR_PMPCFG2_MASK

//--------------------------------------------------------------------
// CSR Registers - Supervisor
//--------------------------------------------------------------------
#define CSR_SSTATUS             0x100
#define CSR_SSTATUS_MASK        SR_SMODE_MASK
#define CSR_SIE                 0x104
#define CSR_SIE_MASK            ((1 << IRQ_S_EXT)   |   \
                                 (1 << IRQ_S_TIMER) |   \
                                 (1 << IRQ_S_SOFT)      \
                                )
#define CSR_STVEC               0x105
#define CSR_STVEC_MASK          0xFFFFFFFF
//#define CSR_SCOUNTEREN        0x106
#define CSR_SSCRATCH            0x140
#define CSR_SSCRATCH_MASK       0xFFFFFFFF
#define CSR_SEPC                0x141
#define CSR_SEPC_MASK           0xFFFFFFFF
#define CSR_SCAUSE              0x142
#define CSR_SCAUSE_MASK         0x8000000F
#define CSR_STVAL               0x143
#define CSR_STVAL_MASK          0xFFFFFFFF
#define CSR_SIP                 0x144
#define CSR_SIP_MASK            ((1 << IRQ_S_EXT)    |  \
                                 (1 << IRQ_S_TIMER)  |  \
                                 (1 << IRQ_S_SOFT)      \
                                )
#define CSR_SPTBR               0x180
#define CSR_SPTBR_MASK          0xFFFFFFFF
#define CSR_SATP                0x180
#define CSR_SATP_MASK           0xFFFFFFFF

//--------------------------------------------------------------------
// ISA Register
//--------------------------------------------------------------------
#define MISA_RV32 (1 << (32 - 2))

#define MISA_RV(x) (1 << (x - 'A'))

#define MISA_RVI MISA_RV('I')
#define MISA_RVE MISA_RV('E')
#define MISA_RVM MISA_RV('M')
#define MISA_RVA MISA_RV('A')
#define MISA_RVF MISA_RV('F')
#define MISA_RVD MISA_RV('D')
#define MISA_RVC MISA_RV('C')
#define MISA_RVS MISA_RV('S')
#define MISA_RVU MISA_RV('U')
#define MISA_RVK MISA_RV('K')


#define MISA_VALUE (MISA_RV32 | MISA_RVI | MISA_RVM | MISA_RVS | MISA_RVU)

//--------------------------------------------------------------------
// Register Enumerations:
//--------------------------------------------------------------------
enum eRegisters
{ 
    RISCV_REGNO_FIRST   =   0,
    RISCV_REGNO_GPR0    =   RISCV_REGNO_FIRST,
    RISCV_REGNO_GPR31   =   31,
    RISCV_REGNO_PC      =   32,
    RISCV_REGNO_CSR0    =   65,
    RISCV_REGNO_CSR4095 =   RISCV_REGNO_CSR0 + 4095,
    RISCV_REGNO_PRIV    =   4161,
    RISCV_REGNO_COUNT
};

struct SpecialTrigger {
    enum {
        TRACE_BUFFER_FULL             = 0x01,
        BREAKPOINT_TRIGGER            = 0x02,
        IOBUF_LARGER                  = 0x04,
        IOBUF_SMALLER                 = 0x08,
        IOBUF_LEVEL0                  = 0x10,
        IOBUF_LEVEL1                  = 0x20,
        IOBUF_EDGE_RISING             = 0x40,
        IOBUF_EDGE_FALLING            = 0x80,
        INVALID_STACK_POINTER_ACCESS  = 0x100,
        INVALID_MEMORY_LOAD           = 0x200,
        INVALID_MEMORY_STORE          = 0x400,
        INVALID_PC_ACCESS             = 0x800,
        HW_CTRL_TRIGGER               = 0x1000
    };
};

//--------------------------------------------------------------------
// GPR Enumerations:
//--------------------------------------------------------------------
enum gpr
{
    GPR_ZERO = 0,
    GPR_RA   = 1,
    GPR_SP   = 2,
    GPR_GP   = 3,
    GPR_TP   = 4,
    GPR_T0   = 5,
    GPR_T1   = 6,
    GPR_T2   = 7,
    GPR_S0   = 8,
    GPR_S1   = 9,
    GPR_A0   = 10,
    GPR_A1   = 11,
    GPR_A2   = 12,
    GPR_A3   = 13,
    GPR_A4   = 14,
    GPR_A5   = 15,
    GPR_A6   = 16,
    GPR_A7   = 17,
    GPR_S2   = 18,
    GPR_S3   = 19,
    GPR_S4   = 20,
    GPR_S5   = 21,
    GPR_S6   = 22,
    GPR_S7   = 23,
    GPR_S8   = 24,
    GPR_S9   = 25,
    GPR_S10  = 26,
    GPR_S11  = 27,
    GPR_T3   = 28,
    GPR_T4   = 29,
    GPR_T5   = 30,
    GPR_T6   = 31
};

//--------------------------------------------------------------------
// Exception Causes
//--------------------------------------------------------------------
#define MCAUSE_INT                      31
#define MCAUSE_MISALIGNED_FETCH         ((0 << MCAUSE_INT) | 0)
#define MCAUSE_FAULT_FETCH              ((0 << MCAUSE_INT) | 1)
#define MCAUSE_ILLEGAL_INSTRUCTION      ((0 << MCAUSE_INT) | 2)
#define MCAUSE_BREAKPOINT               ((0 << MCAUSE_INT) | 3)
#define MCAUSE_MISALIGNED_LOAD          ((0 << MCAUSE_INT) | 4)
#define MCAUSE_FAULT_LOAD               ((0 << MCAUSE_INT) | 5)
#define MCAUSE_MISALIGNED_STORE         ((0 << MCAUSE_INT) | 6)
#define MCAUSE_FAULT_STORE              ((0 << MCAUSE_INT) | 7)
#define MCAUSE_ECALL_U                  ((0 << MCAUSE_INT) | 8)
#define MCAUSE_ECALL_S                  ((0 << MCAUSE_INT) | 9)
#define MCAUSE_ECALL_M                  ((0 << MCAUSE_INT) | 11)
#define MCAUSE_PAGE_FAULT_INST          ((0 << MCAUSE_INT) | 12)
#define MCAUSE_PAGE_FAULT_LOAD          ((0 << MCAUSE_INT) | 13)
#define MCAUSE_PAGE_FAULT_STORE         ((0 << MCAUSE_INT) | 15)
#define MCAUSE_INTERRUPT                (1 << MCAUSE_INT)

//--------------------------------------------------------------------
// MMU Defs
//--------------------------------------------------------------------
#define MMU_LEVELS      2
#define MMU_PTIDXBITS   10
#define MMU_PTESIZE     4
#define MMU_PGSHIFT     (MMU_PTIDXBITS + 2)
#define MMU_PGSIZE      (1 << MMU_PGSHIFT)
#define MMU_VPN_BITS    (MMU_PTIDXBITS * MMU_LEVELS)
#define MMU_PPN_BITS    (32 - MMU_PGSHIFT)
#define MMU_VA_BITS     (MMU_VPN_BITS + MMU_PGSHIFT)

#define PAGE_PRESENT    (1 << 0)
#define PAGE_READ       (1 << 1)    // Readable
#define PAGE_WRITE      (1 << 2)    // Writable
#define PAGE_EXEC       (1 << 3)    // Executable
#define PAGE_USER       (1 << 4)    // User
#define PAGE_GLOBAL     (1 << 5)    // Global
#define PAGE_ACCESSED   (1 << 6)    // Set by hardware on any access
#define PAGE_DIRTY      (1 << 7)    // Set by hardware on any write
#define PAGE_SOFT       (3 << 8)    // Reserved for software
#define PAGE_EXWR       (PAGE_EXEC  | PAGE_WRITE)
#define PAGE_UREAD      (PAGE_READ  | PAGE_USER)
#define PAGE_UWRITE     (PAGE_WRITE | PAGE_USER)

#define PAGE_FLAGS      (0x3FF)

#define PAGE_SPECIAL    _PAGE_SOFT
#define PAGE_TABLE(pte) (((pte) & (PAGE_PRESENT | \
                                   PAGE_READ    | \
                                   PAGE_WRITE   | \
                                   PAGE_EXEC))  == PAGE_PRESENT)

#define PAGE_PFN_SHIFT                      10
#define PAGE_BITS                           12
#define PAGE_SIZE                           (1U << PAGE_BITS)
#define PAGE_MASK                           (PAGE_SIZE - 1)
#if defined(CONFIG_CORES) \
    && (CONFIG_CORES > 0)
#define RISCV_CORES_NUM                     CONFIG_CORES
#else
#define RISCV_CORES_NUM                     1
#endif

#define MULTIPAGE_REGISTER_NUM_PAGES        10

// Index of the register in mpr which contains thread_id associated
// with that page.
#define REGISTER_PAGE_THREAD_REGISTER_INDEX 50
// 32 GPR + 1 PC + 1 m_mpu_sp_lim it
#define REGISTER_PAGE_NUM_REGISTER_INDEX    34

//Commander RISCV
//#define ENABLE_COMMANDER
#define COMMANDER_NUM           4
#define CMDR_ID                 256

#endif