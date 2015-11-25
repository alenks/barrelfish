/*
 * Copyright (c) 2007,2008,2009,2015, ETH Zurich.
 * Copyright (c) 2015, Hewlett Packard Enterprise Development LP.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Universitaetstr. 6, CH-8092 Zurich. Attn: Systems Group.
 */

#ifndef KERNEL_ARMV8_EXCEPTIONS_H
#define KERNEL_ARMV8_EXCEPTIONS_H

/* XXX - this looks like cruft. */
#define AARCH64_EVECTOR_RESET 0x00
#define AARCH64_EVECTOR_UNDEF 0x04
#define AARCH64_EVECTOR_SWI   0x08
#define AARCH64_EVECTOR_PABT  0x0c
#define AARCH64_EVECTOR_DABT  0x10
#define AARCH64_EVECTOR_IRQ   0x18
#define AARCH64_EVECTOR_FIQ   0x1c

enum aarch64_exception_class {
    aarch64_ec_unknown   = 0x0,
    aarch64_ec_wfi       = 0x1,
    aarch64_ec_mcr_cp15  = 0x3,
    aarch64_ec_mcrr_cp15 = 0x4,
    aarch64_ec_mcr_cp14  = 0x5,
    aarch64_ec_ldc_cp14  = 0x6,
    aarch64_ec_fpen      = 0x7,
    aarch64_ec_mcr_cp10  = 0x8,
    aarch64_ec_mcrr_cp14 = 0xc,
    aarch64_ec_il        = 0xe,
    aarch64_ec_svc_aa32  = 0x11,
    aarch64_ec_hvc_aa32  = 0x12,
    aarch64_ec_smc_aa32  = 0x13,
    aarch64_ec_svc_aa64  = 0x15,
    aarch64_ec_hvc_aa64  = 0x16,
    aarch64_ec_smc_aa64  = 0x17,
    aarch64_ec_mrs       = 0x18,
    aarch64_ec_impl      = 0x1f,
    aarch64_ec_iabt_low  = 0x20,
    aarch64_ec_iabt_high = 0x21,
    aarch64_ec_pc_align  = 0x22,
    aarch64_ec_dabt_low  = 0x24,
    aarch64_ec_dabt_high = 0x25,
    aarch64_ec_sp_align  = 0x26,
    aarch64_ec_fpu_aa32  = 0x28,
    aarch64_ec_fpu_aa64  = 0x2c,
    aarch64_ec_serror    = 0x2f,
    aarch64_ec_bkpt_low  = 0x30,
    aarch64_ec_bkpt_high = 0x31,
    aarch64_ec_step_low  = 0x32,
    aarch64_ec_step_high = 0x33,
    aarch64_ec_wpt_low   = 0x34,
    aarch64_ec_wpt_high  = 0x35,
    aarch64_ec_bkpt_soft = 0x38,
    aarch64_ec_bkpt_el2  = 0x3a,
    aarch64_ec_brk       = 0x3c,
};

enum aarch64_dsfc {
    aarch64_dsfc_size_l0     = 0x00,
    aarch64_dsfc_size_l1     = 0x01,
    aarch64_dsfc_size_l2     = 0x02,
    aarch64_dsfc_size_l3     = 0x03,
    aarch64_dsfc_trans_l0    = 0x04,
    aarch64_dsfc_trans_l1    = 0x05,
    aarch64_dsfc_trans_l2    = 0x06,
    aarch64_dsfc_trans_l3    = 0x07,
    aarch64_dsfc_flag_l1     = 0x09,
    aarch64_dsfc_flag_l2     = 0x0a,
    aarch64_dsfc_flag_l3     = 0x0b,
    aarch64_dsfc_perm_l1     = 0x0d,
    aarch64_dsfc_perm_l2     = 0x0e,
    aarch64_dsfc_perm_l3     = 0x0f,
    aarch64_dsfc_external    = 0x10,
    aarch64_dsfc_external_l0 = 0x14,
    aarch64_dsfc_external_l1 = 0x15,
    aarch64_dsfc_external_l2 = 0x16,
    aarch64_dsfc_external_l3 = 0x17,
    aarch64_dsfc_parity      = 0x18,
    aarch64_dsfc_parity_l0   = 0x1c,
    aarch64_dsfc_parity_l1   = 0x1d,
    aarch64_dsfc_parity_l2   = 0x1e,
    aarch64_dsfc_parity_l3   = 0x1f,
    aarch64_dsfc_alighment   = 0x21,
    aarch64_dsfc_tlb_confl   = 0x30,
    aarch64_dsfc_impl1       = 0x34,
    aarch64_dsfc_impl2       = 0x35,
    aarch64_dsfc_sect_dom    = 0x3d,
    aarch64_dsfc_page_dom    = 0x3e,
};

#define CACHE_LINE_BYTES        64
#define ETABLE_ADDR     		0xffffffffffff0000
#define ETABLE_SECTION_OFFSET	0xf000
#define JUMP_TABLE_OFFSET		0x100
#define ETABLE_PHYS_BASE		0x800f0000

#if !defined(__ASSEMBLER__)

/* The exception vector table. */
extern int vectors;

/**
 * Handle page fault in user-mode process.
 *
 * This function should be called in SVC mode with interrupts disabled.
 */
void handle_user_page_fault(lvaddr_t                fault_address,
                            arch_registers_state_t* saved_context)
    __attribute__((noreturn));

/**
 * Handle undefined instruction fault in user-mode process.
 *
 * This function should be called in SVC mode with interrupts disabled.
 */
void handle_user_undef(lvaddr_t                fault_address,
                       arch_registers_state_t* saved_context)
    __attribute__((noreturn));

/**
 * Handle faults in occuring in a priviledged mode.
 *
 * This function should be called with interrupts disabled.
 */
void fatal_kernel_fault(lvaddr_t epc, uint64_t spsr, uint64_t esr,
                        arch_registers_state_t* save_area)
    __attribute__((noreturn));

/**
 * Handle IRQs in occuring in USR or SYS mode.
 *
 * This function should be called with interrupts disabled.
 */
void handle_irq(arch_registers_state_t* save_area, uintptr_t fault_pc,
                uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3)
    __attribute__((noreturn));

#endif // !defined(__ASSEMBLER__)

#endif // __KERNEL_ARMV8_EXCEPTIONS_H__
