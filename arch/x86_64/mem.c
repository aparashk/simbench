#include "arch.h"
#include "mem.h"

static unsigned long initial_pagetables;
static unsigned long runtime_pagetables;

static void prepare_runtime_pagetables()
{
	uint64_t *pml4 = (uint64_t *)runtime_pagetables;
	*pml4 = runtime_pagetables + 0x1003;
	
	uint64_t *pdp = (uint64_t *)(runtime_pagetables + 0x1000);
	*pdp = 0x83;
}

void mem_init()
{
	initial_pagetables = 0xc000;
	runtime_pagetables = 0x400000;

	prepare_runtime_pagetables();
}

void mem_reset()
{
	arch_abort();
}

void mem_mmu_enable()
{
	asm volatile("mov %0, %%cr3\n" :: "r"(runtime_pagetables));
}

void mem_mmu_disable()
{
	asm volatile("mov %0, %%cr3\n" :: "r"(initial_pagetables));
}

void mem_tlb_flush()
{
	asm volatile(
		"mov %cr3, %rax\n"
		"mov %rax, %cr3\n");
}

void mem_tlb_evict(uintptr_t ptr)
{
	asm volatile("invlpg (%0)\n" :: "r"(ptr));
}

size_t mem_get_page_size()
{
	return 1 << 12;
}

int mem_create_page_mapping(uintptr_t phys_addr, uintptr_t virt_addr)
{
	arch_abort();
}

int mem_create_page_mapping_device(uintptr_t phys_addr, uintptr_t virt_addr)
{
	arch_abort();
}
