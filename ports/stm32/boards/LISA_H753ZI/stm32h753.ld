/*
    GNU linker script for STM32H753 without internal filesystem
*/

/* Specify the memory areas */
MEMORY
{
    ITCM (xrw)      : ORIGIN = 0x00000000, LENGTH = 63K     /* Used for stack */
    NOINIT (xrw)    : ORIGIN = 0x0000fc00, LENGTH = 1K      /* Used for noinit memory */
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K   /* entire flash */
    FLASH_APP (rx)  : ORIGIN = 0x08020000, LENGTH = 1920K   /* sectors (1-7) + (0-7) */
    DTCM (rw)       : ORIGIN = 0x20000000, LENGTH = 128K
    RAM (xrw)       : ORIGIN = 0x24000000, LENGTH = 512K    /* AXI SRAM */
    SRAM1 (xrw)     : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2 (xrw)     : ORIGIN = 0x30020000, LENGTH = 128K
    SRAM3 (xrw)     : ORIGIN = 0x30040000, LENGTH = 32K     /* Used for Ethernet buffers */
    SRAM4 (xrw)     : ORIGIN = 0x38000000, LENGTH = 64K
/*    BACKUP_SRAM (rw): ORIGIN = 0x38800000, LENGTH = 4K   */
}

/* produce a link error if there is not this amount of RAM for these sections */
_minimum_stack_size = 2K;
_minimum_heap_size = 16K;

/* Define the stack.  The stack is full descending so begins just above last byte
   of RAM.  Note that EABI requires the stack to be 8-byte aligned for a call. */
_estack = ORIGIN(ITCM) + LENGTH(ITCM) - _estack_reserve;
_sstack = _estack - 16K; /* tunable */

/* RAM extents for the garbage collector */
_ram_start = ORIGIN(RAM);
_ram_end = ORIGIN(RAM) + LENGTH(RAM);
_heap_start = _ebss; /* heap starts just after statically allocated memory */
_heap_end = _ram_end;

/* Define output sections */
SECTIONS
{
    .eth_buffers (NOLOAD) : {
        *eth.o*(.bss.eth_dma)
    } >SRAM3

    /* Uninitialized data section */
    .noinit_section (NOLOAD) : {
        . = ALIGN(4);
        KEEP(*(.noinit))
        . = ALIGN(4);
    } >NOINIT
}
