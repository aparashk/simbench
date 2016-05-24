#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "define.h"

#define NOINLINE __attribute__((noinline))
#define ALIGN __attribute__((aligned(4096)))

#ifndef FAST_MODE
#define BENCHMARK_ITERATIONS 1000
#define IO_COPRO_BENCHMARK_ITERATIONS 250000
#define IO_DEVICE_BENCHMARK_ITERATIONS 400000
#define DISPATCH_SPD_BENCHMARK_ITERATIONS 500000
#define DISPATCH_SPI_BENCHMARK_ITERATIONS 200
#define DISPATCH_IPD_BENCHMARK_ITERATIONS 100000
#define DISPATCH_IPI_BENCHMARK_ITERATIONS 250
#define MEMORY_TLBFLUSH_BENCHMARK_ITERATIONS 4000
#define MEMORY_COLD_BENCHMARK_ITERATIONS 50000
#define MEMORY_NONPRIV_BENCHMARK_ITERATIONS 300000
#define MEMORY_TLBEVICT_BENCHMARK_ITERATIONS 4000
#define MEMORY_HOT_BENCHMARK_ITERATIONS 500000
#define EXCEPTION_DF_BENCHMARK_ITERATIONS 25000
#define EXCEPTION_SC_BENCHMARK_ITERATIONS 50000
#define EXCEPTION_SWI_BENCHMARK_ITERATIONS 20000
#define EXCEPTION_UNDEF_BENCHMARK_ITERATIONS 50000
#define EXCEPTION_IF_BENCHMARK_ITERATIONS 25000
#define CODEGEN_LB_BENCHMARK_ITERATIONS 500
#define CODEGEN_SB_BENCHMARK_ITERATIONS 100
#else
#define BENCHMARK_ITERATIONS 1
#define IO_COPROCESSOR_BENCHMARK_ITERATIONS 5
#define IO_DEVICE_BENCHMARK_ITERATIONS 5
#define DISPATCH_SPD_ITERATIONS 5
#define DISPATCH_SPI_ITERATIONS 5
#define DISPATCH_IPD_ITERATIONS 5
#define DISPATCH_IPI_ITERATIONS 5
#define MEMORY_TLBFLUSH_BENCHMARK_ITERATIONS 5
#define MEMORY_COLD_BENCHMARK_ITERATIONS 5
#define MEMORY_NONPRIV_BENCHMARK_ITERATIONS 5
#define MEMORY_EVICT_BENCHMARK_ITERATIONS 5
#define MEMORY_HOT_BENCHMARK_ITERATIONS 5
#define EXCEPTION_DF_BENCHMARK_ITERATIONS 5
#define EXCEPTION_SC_BENCHMARK_ITERATIONS 5
#define EXCEPTION_SWI_BENCHMARK_ITERATIONS 5
#define EXCEPTION_UD_BENCHMARK_ITERATIONS 5
#define EXCEPTION_IF_BENCHMARK_ITERATIONS 5
#define CODEGEN_LB_BENCHMARK_ITERATIONS 5
#define CODEGEN_SB_BENCHMARK_ITERATIONS 5
#endif


typedef void (*benchmark_kernel_t)();

typedef struct {
	const char *name;
	const char *category;
	benchmark_kernel_t kernel_init;
	benchmark_kernel_t kernel;
	benchmark_kernel_t kernel_cleanup;
	benchmark_kernel_t kernel_control;
	uint64_t iteration_count;
} benchmark_t;

#define DEFINE_BENCHMARK(__name) const benchmark_t __attribute__((section(".benchmarks"))) __attribute__((aligned(__alignof__(unsigned long))))  __benchmark_##__name

#endif
