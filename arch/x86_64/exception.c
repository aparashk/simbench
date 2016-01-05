#include "arch.h"
#include "x86.h"

void arch_ifault_install_return()
{
	arch_abort();
}

void arch_ifault_install_break()
{
	arch_abort();
}

static void skip_handler(uint64_t va)
{
	printf("TODO: skip instruction\n");
	arch_abort();
}

void arch_dfault_install_skip()
{
	mem_install_page_fault_handler(skip_handler);
}
