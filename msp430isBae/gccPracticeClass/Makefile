# Project name
SOURCE          = final.c
ADDITIONAL      = pow.S
# Get base name so we can create .elf file
NAME            = $(basename $(SOURCE))

# Flash to board with mspdebug
flash: $(NAME).elf
	mspdebug rf2500 "prog $(NAME).elf"

# Erase board
erase:
	mspdebug rf2500 erase

# Remote debug board
debug: $(NAME).elf
	( mspdebug rf2500 "gdb" 1>/dev/null & ); msp430-gdb $(NAME).elf -ex "target remote :2000"
