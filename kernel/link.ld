ENTRY (_main)

phys = 0x00100000;

SECTIONS
{

	.text phys : AT(phys)
	{
	    code = .;
		*(.text)
		*(.rodata)
	}

	.data : AT(phys + (data - code))
	{
	    data = .;
		*(.data)
	}

	.bss : AT(phys + (bss - code))
	{
	    bss = .;
		*(.bss)
	}
	
	end = .;
	
	/DISCARD/ :
	{
	    *(.comment)
	    *(.eh_frame)
	    *(.note.gnu.build-id)
	}
}
