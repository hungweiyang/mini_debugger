#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

void pou()
{ 
    int i = 0;
    i++;
    i++;
    i++;

    printf("Pou: %d\n", i);
}

char opcode_backup[3];

extern void helper();
void DebugHandler(unsigned char* addr)
{
    printf("%lx at 0x%p\n", *(uintptr_t*)addr, addr);

    *(uintptr_t*)addr -= 3;

    unsigned char *breakpoint = (unsigned char*) (*(uintptr_t*)addr);
    breakpoint[0] = opcode_backup[0];
    breakpoint[1] = opcode_backup[1];
    breakpoint[2] = opcode_backup[2];
}

register void *handler_address asm("r15");

#define PROT_NONE       0
#define PROT_READ       1
#define PROT_WRITE      2
#define PROT_EXEC       4
int     mprotect(void *addr, size_t len, int prot);

int main()
{
    handler_address = (void*)helper;

    int result = mprotect((void *) ((uintptr_t)pou & ~(4096-1)), 4096, PROT_READ | PROT_WRITE | PROT_EXEC);

    if (result == -1)
    {
        printf("Error: %s\n", strerror(errno));
        exit(1);
    } 

    unsigned char* p = (unsigned char*)pou;
    p += 0xf;

    opcode_backup[0] = p[0];
    p[0] = 0x41;

    opcode_backup[1] = p[1];
    p[1] = 0xff;

    opcode_backup[2] = p[2];
    p[2] = 0xd7;

    pou();
    return 0;
}