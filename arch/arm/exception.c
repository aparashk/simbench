#include "arch.h"

extern uint32_t vectors_start;
extern uint32_t _VECTORS_RELOCATE;
extern uint32_t pabt_handler;
extern uint32_t dabt_handler;
extern uint32_t swi_handler;
extern uint32_t undef_handler;
extern void pabt_return();
extern void dabt_skip();
extern void swi_skip();
extern void undef_skip();
extern void default_handler();

// The vectors have been copied into the low page so need to do some address calculation
static void install_pabt(uint32_t pabt_location)
{
	uint32_t vector_location = ((uint32_t)&pabt_handler - (uint32_t)&vectors_start) + (uint32_t)&_VECTORS_RELOCATE;
	*(uint32_t*)vector_location = pabt_location;
}
static void install_dabt(uint32_t dabt_location)
{
	uint32_t vector_location = ((uint32_t)&dabt_handler - (uint32_t)&vectors_start) + (uint32_t)&_VECTORS_RELOCATE;
	*(uint32_t*)vector_location = dabt_location;
}
static void install_swi(uint32_t swi_location)
{
	uint32_t vector_location = ((uint32_t)&swi_handler - (uint32_t)&vectors_start) + (uint32_t)&_VECTORS_RELOCATE;
	*(uint32_t*)vector_location = swi_location;
}
static void install_undef(uint32_t undef_location)
{
	uint32_t vector_location = ((uint32_t)&undef_handler - (uint32_t)&vectors_start) + (uint32_t)&_VECTORS_RELOCATE;
	*(uint32_t*)vector_location = undef_location;
}

void arch_ifault_install_return()
{
	install_pabt((uint32_t)pabt_return);
}

void arch_ifault_install_break()
{
	install_pabt((uint32_t)default_handler);
}

void arch_dfault_install_skip()
{
	install_dabt((uint32_t)dabt_skip);
}

void arch_syscall_install_skip()
{
	install_swi((uint32_t)swi_skip);
}

void arch_undef_install_skip()
{
	install_undef((uint32_t)undef_skip);
}

#define MODE_IRQ_DISABLE (1 << 7) 
void arch_irq_enable()
{
#ifdef ARCH_ARMV7
	asm volatile("cpsie i\n");
#else
	uint32_t mode;
	asm("mrs %0, CPSR" : "=r"(mode));
	mode &= ~MODE_IRQ_DISABLE;
	asm("msr CPSR_c, %0" ::"r"(mode));
#endif
}

void arch_irq_disable()
{
#ifdef ARCH_ARMV7
	asm volatile("cpsid i\n");
#else 
	uint32_t mode;
	asm("mrs %0, CPSR" : "=r"(mode));
	mode |= MODE_IRQ_DISABLE;
	asm("msr CPSR_c, %0" ::"r"(mode));	
#endif
}

extern irq_handler_t arch_irq_handler;
void arch_irq_install_handler(irq_handler_t handler)
{
	arch_irq_handler = handler;
}


