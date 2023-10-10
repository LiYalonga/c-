#include <stdio.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <sys/mman.h>  
#include <unistd.h>
#include <string.h>
#include <elf.h>  
#include <dlfcn.h>
#include <pthread.h>
#include <sys/stat.h>
Elf64_Addr mask_addr=0xfffff000;
Elf64_Addr low_addr=0x00000fff;
int main(int argc,char *argv[]){
    int fp = open(argv[1], O_RDWR | O_EXCL); 
    Elf64_Addr code_addr_vaddr=0x8000000;
    Elf64_Addr data_addr_vaddr=0x82fe120;             
    Elf64_Addr code_addr= code_addr_vaddr & mask_addr;
    Elf64_Addr data_addr= data_addr_vaddr & mask_addr;
    Elf64_Addr code_offset_inpage=code_addr_vaddr & low_addr;
    Elf64_Addr data_offset_inpage=data_addr_vaddr & low_addr;
    printf("%lx\n",code_addr);
    printf("%lx\n",data_addr);
    printf("%lx\n",code_offset_inpage);
    printf("%lx\n",data_offset_inpage);

    struct stat sb;
    fstat(fp,&sb);
    printf("%lx\n",sb.st_size);
    void *ret_addr=(void *)mmap(NULL,sb.st_size,PROT_READ,MAP_PRIVATE,fp,0);// mmap return void*
    printf("ret_addr %lx\n",ret_addr);
    Elf64_Ehdr *elf_header=ret_addr;
    printf("e_entry: %lx\n",elf_header->e_entry);
    printf("e_phoff: %lx\n",elf_header->e_phoff);

    uint64_t program_header_addr=ret_addr+(uint64_t)elf_header->e_phoff;
    printf("%lx \n",program_header_addr);

    Elf64_Phdr *program_headers=program_header_addr;

    Elf64_Phdr *program_header_code=program_header_addr;           //program_header_code  code segment
  
    printf("program filesz: %lx \n",program_headers->p_filesz);// Elf64_Xword

    printf("program memsz: %lx \n",program_headers->p_memsz);//Elf64_Xword
    printf("prgeam align: %lx \n",program_headers->p_align);//Elf64_Xword
    printf("prgram type: %lx \n",program_headers->p_type); //Elf64_Word
    printf("prgram vaddr: %lx \n",program_headers->p_vaddr); 
    printf("prgram paddr: %lx \n",program_headers->p_paddr); 
    printf("prgram offset: %lx \n",program_headers->p_offset);
    /*
    void* ret_code_addr=mmap(code_addr,program_headers->p_memsz,PROT_WRITE | PROT_READ | PROT_EXEC,MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED,-1,0);
    printf("ret_code_addr_map: %lx\n",ret_code_addr);
    mprotect(code_addr,program_header_code->p_memsz,PROT_WRITE | PROT_READ |PROT_EXEC);
    if(ret_code_addr!=(int)code_addr){
        printf("mmap code false\n");
    }*/

    printf("\n\n");
    program_headers++;
    Elf64_Phdr *program_header_data=program_headers;                //program_header_data  data segment
    printf("program filesz: %lx \n",program_header_data->p_filesz);// Elf64_Xword
    printf("program memsz: %lx \n",program_header_data->p_memsz);//Elf64_Xword
    printf("prgeam align: %lx \n",program_header_data->p_align);//Elf64_Xword
    printf("prgram type: %lx \n",program_header_data->p_type); //Elf64_Word
    printf("prgram vaddr: %lx \n",program_header_data->p_vaddr); 
    printf("prgram paddr: %lx \n",program_header_data->p_paddr); 
    printf("prgram offset: %lx \n",program_headers->p_offset);  //uint64_t


    void *ret_addr_mmap=(void*)mmap(code_addr,program_header_data->p_vaddr+program_header_data->p_offset-code_addr,PROT_EXEC | PROT_READ | PROT_WRITE,MAP_ANONYMOUS
    |MAP_FIXED|MAP_PRIVATE,-1,0);// length 3fc240    304a00
    mprotect(code_addr,program_header_data->p_vaddr+program_header_data->p_memsz-code_addr,PROT_EXEC | PROT_READ | PROT_WRITE);
    printf("program_header_data->p_vadd %lx\n",program_header_data->p_vaddr);
    printf("program_header_data->p_memsz %lx\n",program_header_data->p_memsz);
    printf("code addr: %lx \n",code_addr);
    int n=(int)(program_header_data->p_vaddr)+(int)(program_header_data->p_memsz)-(int)(code_addr);
    printf("!!!!!!!!!!%lx:  \n",n);
    printf("ret_addr_mmap: %lx\n",ret_addr_mmap);



    void* ret_code_addr_memcpy=(void *)memcpy((uint64_t)code_addr_vaddr,
    (uint64_t)ret_addr+(uint64_t)program_header_code->p_offset,
    program_header_code->p_memsz);

    void* ret_data_addr_memcpy=(void *)memcpy((uint64_t)data_addr_vaddr,
    (uint64_t)ret_addr+(uint64_t)program_header_data->p_offset,
    program_header_data->p_memsz);

    if(ret_code_addr_memcpy!=code_addr_vaddr){
        printf("memcpy code segment is false\n");
    }
    if(ret_data_addr_memcpy!=data_addr_vaddr){
        printf("memcpy data segment is false\n");
    }

    printf("ret_cpde_addr_mencpy is : %lx \n",ret_code_addr_memcpy);
    printf("ret_data_addr_memcpy is : %lx \n",ret_data_addr_memcpy);
   
    while(1){};
}
    /*
    void* ret_data_addr=mmap(data_addr,program_headers->p_memsz+data_offset_inpage,PROT_WRITE | PROT_READ,MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED,-1,0);
    printf("ret_data_addr_map: %lx\n",ret_data_addr);
    mprotect(data_addr,program_header_data->p_memsz,PROT_WRITE | PROT_READ);
    if(ret_data_addr!=data_addr){
        printf("mmap data false\n");
    }
    */   //map data segment