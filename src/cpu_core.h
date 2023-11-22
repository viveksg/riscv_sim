#include "common.h"
#include "mm_handler.h"

typedef struct
{
  int source_reg1;
  int source_reg2;
  int dest_reg;
  uint32_t opcode;
  uint64_t immediate;
} instruction_packet;

class CPUCore
{
private:
  int64_t registers[MAX_REG];
  int64_t csrs[MAX_CSRS];
  MMHandler *mHandler;
  uint64_t core_pc = 0;
  bool continue_simulation;

public:
  void get_register(uint32_t reg_no);
  void set_register(uint32_t reg_no, uint64_t reg_val);
  void execute();
  void decode(uint64_t opcode, instruction_packet *instr_packet);
  void start_simulation(uint64_t start_addr);
  CPUCore(MMHandler *mhndler);
};