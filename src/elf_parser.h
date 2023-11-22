#ifndef __ELF_PARSER__
#include "common.h"
#include "mm_handler.h"
#include <libelf.h>
#include <fcntl.h>
#include <gelf.h>
#include <bfd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

class ELFParser
{

private:
    string file_loc;
    uint8_t *mem;
    MMHandler *mController;

public:
    ELFParser(string file_location, uint32_t *start_addr, MMHandler* mCntrl);
};
#endif