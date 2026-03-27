ENTRY(reset)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x00010000, LENGTH = 2M
}

SECTIONS
{
    . = 0x00010000;

    .startup :
    {
        startup.o(.text)
    } > RAM

    .text :
    {
        *(.text)
    } > RAM

    .data :
    {
        *(.data)
    } > RAM

    . = . + 0x1000;   /* Stack space */
    stack_top = .;
}