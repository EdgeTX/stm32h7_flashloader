MEMORY
{
  RAM (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
}

SECTIONS
{
  PrgCode :
  {
    __PrgCode_start__ = .;
		*(.PrgCode)
		*(.PrgCode*)
		*(.text)
		*(.text*)
		. = ALIGN(4);
	} >RAM

	__PrgCode_end__ = __PrgCode_start__ + SIZEOF(PrgCode);
	__PrgCode_size__ = SIZEOF(PrgCode);

	PrgData :
	{
		. = ALIGN(4);
	  __PrgData_start__ = .;
		*(.PrgData)
		*(.PrgData*)
		*(.data)
		*(.data*)
		*(.rodata)
		*(.rodata*)
		_sbss = .;
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
	} >RAM

  __PrgData_end__ = __PrgData_start__ + SIZEOF(PrgData);
	__PrgData_size__ = SIZEOF(PrgData);

	DevDscr :
	{
		. = ALIGN(4);
	  __DevDscr_start__ = .;
		*(.DevDscr)
		*(.DevDscr*)
		*(.constdata)
		*(.constdata*)
		. = ALIGN(4);
	} >RAM

  __DevDscr_end__ = __DevDscr_start__ + SIZEOF(DevDscr);
	__DevDscr_size__ = SIZEOF(DevDscr);

	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}
}
