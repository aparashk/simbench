#include "arch.h"
#include "armv8.h"
#include "printf.h"

typedef exception_handler_t exception_table_t[64];

static exception_table_t el0_sync_table;
static irq_handler_t el0_irq_handler_;

void armv8_install_sync_handler(int ec, exception_handler_t handler) {
	el0_sync_table[ec] = handler;
}

static int exception_skip() 
{
	// increment the ELR by 4
	uint32_t elr;
	asm volatile ("mrs %0, ELR_EL1" : "=r"(elr));
	elr += 4;
	asm volatile ("msr ELR_EL1, %0" :: "r"(elr));
	
	return 0;
}

void arch_ifault_install_return()
{
	arch_abort();
}

void arch_ifault_install_break()
{
	arch_abort();
}

void arch_dfault_install_skip()
{
	arch_abort();
}

void arch_syscall_install_skip()
{
	arch_abort();
}

void arch_undef_install_skip()
{
	armv8_install_sync_handler(0, exception_skip);
}

void arch_irq_enable()
{
	if(armv8_in_el1()) {
		arch_abort();
	}
	armv8_syscall(SYSCALL_IRQ_ENABLE);
}

void arch_irq_disable()
{
	if(armv8_in_el1()) {
		arch_abort();
	}
	armv8_syscall(SYSCALL_IRQ_DISABLE);
}

void arch_irq_install_handler(irq_handler_t handler)
{
	el0_irq_handler_ = handler;
}


int el1_lower_irq_handler()
{
	if(el0_irq_handler_ == NULL) {
		fprintf(OUTPUT, "No IRQ handler registered!\n");
		return 1;
	} else {
		el0_irq_handler_();
	}
}

int el1_lower_sync_handler() 
{
	uint32_t syndrome;
	asm volatile ("mrs %0, esr_el1" : "=r"(syndrome));
	uint32_t ec = syndrome >> 26;
	uint32_t iss = syndrome & 0x1ffffff;
	uint32_t il = (syndrome >> 25) & 1;
	
	if(el0_sync_table[ec] == NULL) {
		fprintf(OUTPUT, "Exception taken when no handler was registered!\n");
		return 1;
	} else {
		return el0_sync_table[ec]();
	}
}