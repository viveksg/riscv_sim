#include "elf_parser.h"

ELFParser::ELFParser(string file_location, uint32_t *start_addr, MMHandler* mCntrl)
{
   int elf_fd;
   Elf *elf_pointer;
   Elf_Kind elf_kind_val;
   Elf_Scn *elf_scn;
   Elf_Data *elf_data;
   Elf32_Shdr *elf_shdr;
   size_t section_heaher_index;

   // check if version is none
   if (elf_version(EV_CURRENT) == EV_NONE)
      return;
   // obtain filedescriptor to the elf_file
   if ((elf_fd = open((const char *)file_location.c_str(), O_RDONLY, 0)) < 0)
   {
      cout << "ELF FD " << elf_fd << endl;
      return;
   }
   // get pointer to the beginning
   if ((elf_pointer = elf_begin(elf_fd, ELF_C_READ, NULL)) == NULL)
      return;

   elf_kind_val = elf_kind(elf_pointer);
   if (elf_kind_val != ELF_K_ELF)
      return;
   // get section header index
   if (elf_getshdrstrndx(elf_pointer, &section_heaher_index) != 0)
      return;

   // Get entrypoint header
   GElf_Ehdr _ehdr;
   GElf_Ehdr *ehdr = gelf_getehdr(elf_pointer, &_ehdr);
   // set entrypoint
   *start_addr = ehdr->e_entry;

   int section_index = 0;
   
   // Iterate through each section
   while ((elf_scn = elf_getscn(elf_pointer, section_index)) != NULL)
   {
      elf_shdr = elf32_getshdr(elf_scn);
      // check if the section needs to be allocated on memory
      if ((elf_shdr->sh_flags & SHF_ALLOC) && (elf_shdr->sh_size > 0))
      {
         elf_data = elf_getdata(elf_scn, NULL);
         string region_name = elf_strptr(elf_pointer, section_heaher_index, elf_shdr->sh_name);
         mCntrl->set_region(elf_shdr->sh_addr,elf_shdr->sh_addr+elf_shdr->sh_size,elf_shdr->sh_size,region_name);
      }
      section_index++;
   }
   mCntrl->setDefaultBaseAddr();
   section_index = 0;
   elf_pointer = elf_begin(elf_fd, ELF_C_READ, NULL);
   while((elf_scn = elf_getscn(elf_pointer, section_index))!=NULL)
   {
      // check if current section is text section
      if (elf_shdr->sh_type == SHT_PROGBITS)
      {  
         uint64_t currr_addr = elf_shdr->sh_addr;
         for(int i = 0; i < elf_shdr->sh_size; i++)
         {
            mCntrl->write_byte(currr_addr + i, ((uint8_t *)elf_data->d_buf)[i]);
         }
      }
      section_index++;
   }
   elf_end(elf_pointer);
   close(elf_fd);
}