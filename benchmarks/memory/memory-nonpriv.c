#include "benchmark.h"
#include "harness.h"
#include "mem.h"

#define MEMORY_BENCHMARK_ITERATIONS 100000

static volatile uint32_t value = 0;

static void ALIGN kernel()
{
	uint32_t total_iterations = BENCHMARK_ITERATIONS * MEMORY_BENCHMARK_ITERATIONS;
	uint32_t i;
		
	for(i = 0; i < total_iterations; ++i) {
		arch_nonpriviliged_write(&value);
	}
}

static void kernel_mmu_init()
{
	mem_init();
	mem_create_page_mapping((uintptr_t)&value, (uintptr_t)&value);
	
	mem_mmu_enable();
	mem_tlb_flush();
}
static void kernel_mmu_cleanup()
{
	mem_mmu_disable();
	mem_tlb_flush();
	mem_reset();
}

static benchmark_t bmark_mmu = {
	.name="Memory-NonPriv",
	.category="Memory",
	.kernel_init=kernel_mmu_init,
	.kernel=kernel,
	.kernel_cleanup=kernel_mmu_cleanup
};

REG_BENCHMARK(bmark_mmu);
