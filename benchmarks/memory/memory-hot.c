#include "benchmark.h"
#include "debug.h"
#include "mem.h"

static volatile uint32_t value = 0;

__align12;

static void ALIGN kernel()
{
	uint32_t total_iterations = BENCHMARK_ITERATIONS * MEMORY_HOT_BENCHMARK_ITERATIONS;
	uint32_t i;
	
	debug_spinner_start(MEMORY_HOT_BENCHMARK_ITERATIONS);
	
	for(i = 0; i < total_iterations; ++i) {
		debug_spinner();
		value = value;
		value = value;
		value = value;
		value = value;
		value = value;
		value = value;
		value = value;
		value = value;
		value = value;
		value = value;
	}
}

static void kernel_control()
{
	uint32_t total_iterations = BENCHMARK_ITERATIONS * MEMORY_HOT_BENCHMARK_ITERATIONS;
	uint32_t i;
	
	debug_spinner_start(MEMORY_HOT_BENCHMARK_ITERATIONS);
	uint32_t value;
	
	for(i = 0; i < total_iterations; ++i) {
		debug_spinner();
		asm volatile ("");
	}
}

static void kernel_mmu_init()
{
	mem_init();
	mem_mmu_enable();
	mem_tlb_flush();
}
static void kernel_mmu_cleanup()
{
	mem_mmu_disable();
	mem_tlb_flush();
	mem_reset();
}

DEFINE_BENCHMARK(memory_hot_nommu) = {
	.name="Memory-Hot-NoMMU",
	.category="Memory",
	.kernel=kernel,
	.kernel_control=kernel_control,
	.iteration_count = BENCHMARK_ITERATIONS * MEMORY_HOT_BENCHMARK_ITERATIONS
};

DEFINE_BENCHMARK(memory_hot_mmu) = {
	.name="Memory-Hot-MMU",
	.category="Memory",
	.kernel_init=kernel_mmu_init,
	.kernel=kernel,
	.kernel_control=kernel_control,
	.kernel_cleanup=kernel_mmu_cleanup,
	.iteration_count = BENCHMARK_ITERATIONS * MEMORY_HOT_BENCHMARK_ITERATIONS
};
