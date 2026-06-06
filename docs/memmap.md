02000000h -> Multibus memory (last 1mb)

02100000h -> PROM bss

30000000h -> PROM Text, then Data

30000800h -> Mouse button state & IP2 board revision (??)
    bit 0 - right button down

    bit 1 - middle button down

    bit 2 - left button down

    bit 4 - IP2 board revision (set - RevB, otherwise Reva)

31000000h -> Mouse movement state
    (todo)
    
31800000h -> Switch register (used for system configuration)

    bit 15 - Master/slave and which "2mb multibus space" is occupied by this unit
    bits 14:13 - reserved

    bits 12:11 - RS232 console speed
        0 -> 9600 baud
        1 -> 300 baud
        2 ->
    (todo)

32000000h -> Signetics SCC68681 Dual UART 0 (tty0-1)
    32000000-3200000f are registers 
    TODO [https://www.nxp.com/docs/en/data-sheet/SCC68681.pdf]

32800000h -> Signetics SCC68681 Dual UART 1 (tty2-3)
    32800000-3280000f are registers 
    TODO [https://www.nxp.com/docs/en/data-sheet/SCC68681.pdf]

33000000h -> 2KB SRAM Memory for PROM (Can run without any RAM boards)

34000000h -> Clock Control (8b-it)
    (todo)

35000000h -> Clock Data (8-bit)
    (todo)

36000000h -> "OS Base" (8-bit)
    (todo)
38000000h -> Status Register (16-bit)

    Bit 3-0: Debug LEDs.

    Bit 4: Enable external interrupts.

    Bit 5: Enable internal interrupts.

    Bit 6: Force multibus init when set. (unsets?)

    Bit 7: Allow access to system segment only

    Bit 8: User-mode access to floating point board allowed

    Bit 9: User access to geometry engine allowed

    Bit 10: Allow IP2 board access from Multibus

    Bit 11: "Hold bus until CBRQ asserted"

    Bit 12: Assert master of Pipe bus?

    Bit 13: Forcibly generate bad parity (debug?)

    Bit 14: Enable watchdog timer

    Bit 15: Fast timeout on accesses
    (todo)

39000000h -> Parity Control (8-bit)
    (todo)

3A000000h -> Multibus Protect (8-bit)
    (todo)

pagetable stuff: 

3B000000h -> Pagetable Map Base (32-bit)
    (todo)

3C000000h -> Text/Data Base (16-bit)
    (todo)

3D000000h -> Text/Data Limit (16-bit)
    (todo)

3E000000h -> Stack Base (16-bit)
    (todo)

3F000000h -> Stack Limit (16-bit)
    (todo)

CPU will setup supervisor stack pointer based on pointer at 00000000h jump to initial PC (pointer at 00000004h) and jump to system initialisation at 30000000h
