#include "arch.h"
#include "benchmark.h"
#include "debug.h"
#include "harness.h"
#include "mem.h"

#define EXCEPTION_BENCHMARK_ITERATIONS 50000

static void (*ptr)() = (void (*))0x40000000;

asm (".align 12\n");

static void ALIGN kernel()
{
	uint64_t i, total_iterations = BENCHMARK_ITERATIONS * EXCEPTION_BENCHMARK_ITERATIONS;
	
	debug_spinner_start(EXCEPTION_BENCHMARK_ITERATIONS);
	
	for(i=0; i < total_iterations; ++i) {
		debug_spinner();
		ptr();
	}
}

static void kernel_init()
{
	mem_init();
	mem_mmu_enable();
	mem_tlb_flush();
	
	arch_ifault_install_return();
}
static void kernel_cleanup()
{
	mem_mmu_disable();
	mem_tlb_flush();
	mem_reset();
}

static benchmark_t bmark = {
	.name="Instruction-Fault",
	.category="Exception",
	.kernel_init=kernel_init,
	.kernel=kernel,
	.kernel_cleanup=kernel_cleanup
};

REG_BENCHMARK(bmark);
