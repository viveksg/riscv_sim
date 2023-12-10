#include "common.h"
#include "mm_handler.h"

typedef struct
{
  int source_reg1;
  int source_reg2;
  int dest_reg;
  uint32_t opcode;
  int64_t immediate;
  int32_t shamt;
  uint32_t decode_status;
  uint64_t instruction_val;
  uint32_t instruction_type;
  uint32_t immediate_type;
} instruction_packet;

class CPUCore
{
private:
  uint32_t registers[MAX_REG];
  uint32_t csrs[MAX_CSRS];
  MMHandler *mHandler;
  uint32_t core_pc = 0;
  bool continue_simulation;
  //csr registers
    // CSR - Machine
    uint32_t m_csr_mhartid;
    uint32_t m_csr_mcycle;
    uint32_t m_csr_mcycleh;
    uint32_t m_csr_mepc;
    uint32_t m_csr_mcause;
    uint32_t m_csr_msr;
    uint32_t m_csr_mpriv;
    uint32_t m_csr_mevec;
    uint32_t m_csr_mie;
    uint32_t m_csr_mip;
    uint64_t m_csr_mtval;
    uint64_t m_csr_mtime;
    uint64_t m_csr_mtime_thread_scheduled;
    uint64_t m_csr_mtimecmp;
    uint32_t m_csr_mscratch;
    uint32_t m_csr_mideleg;
    uint32_t m_csr_medeleg;
    uint32_t m_pc_x;
    bool m_fault = false;
    bool m_break = false;
    uint32_t m_trace = 0xFFFFFFFF;
    // CSR - Supervisor
    uint32_t m_csr_sepc;
    uint32_t m_csr_sevec;
    uint32_t m_csr_scause;
    uint32_t m_csr_stval;
    uint32_t m_csr_satp;
    uint32_t m_csr_sscratch;
public:
  uint32_t get_opcode(uint32_t addr);
  uint32_t get_register(uint32_t reg_no);
  void set_register(uint32_t reg_no, uint32_t reg_val);
  void execute();
  void decode(uint32_t opcode, instruction_packet *instr_packet);
  void start_simulation(uint32_t start_addr);
  void handle_exception(uint32_t cause, uint32_t pc_val , uint32_t badaddr /*= 0*/);
  uint32_t csr_access(uint32_t addr, uint32_t data, bool set, uint32_t clr, uint32_t *pc);
  CPUCore(MMHandler *mhndler);
};