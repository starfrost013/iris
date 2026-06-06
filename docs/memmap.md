02000000h -> Multibus memory (last 1mb)

02100000h -> PROM bss

30000000h -> PROM Text, then Data

30000800h -> Mouse button state & IP2 board revision (??)

    bit 0 - right button down

    bit 1 - middle button down

    bit 2 - left button down

    bit 4 - IP2 board revision (set - RevB, otherwise RevA)

31000000h -> Mouse movement state ("quadrature")
    
    bit 0 -> x-movement detected

    bit 1 -> direction of x movement (0 = --, 1 = ++)

    bit 2 -> y-movement detected

    bit 3 -> direction of y movement (0 = --, 1 = ++)
    
31800000h -> DIP Switch register (used for system configuration)

    bit 15 - Master/slave and which "2mb multibus space" is occupied by this unit

    bits 14:13 - reserved

    bits 12:11 - RS232 console speed

        0 -> 9600 baud

        1 -> 300 baud

        2 -> 1200 baud

        3 -> 19200 baud

        4 -> 600 baud (not actually possible but defined?)

    bits 10:8 - "display combination" (primary/secondary)

        0 -> 60Hz non-interlaced/60Hz non-interlaced
        
        1 -> 60Hz non-interlaced/30Hz interlaced

        2 -> 60Hz non-interlaced/NTSC television

        3 -> 60Hz non-interlaced/PAL television

        4 -> 30Hz interlaced/60Hz non-interlaced
        
        5 -> 30Hz interlaced/30Hz interlaced
        
        6 -> 30Hz interlaced/NTSC television
        
        7 -> 30Hz interlaced/PAL television
        
    bit 7 - reserved
    
    bit 6 - use secondary display while booting

    bit 5 - quiet boot logging

    bit 4 - autoboot (off = stop in PROM)

    bit 3:0 - specified boot method

        0 -> boot from hard drive (ipX, sdX, then mdX)
        
        1 -> tape drive
        
        2 -> floppy disk (sfX, then mfX)
        
        3 -> netboot over ethernet using XNS

        4 -> not used

        5 -> don't bother, enter into PROM

        6 -> boot from PROM development board for PROM testing

        7 -> netboot via TCP/UDP
        
        8 -> not used 

        9 -> Hard disk, always using Interphase SMD controller

        0xA / 0xB -> Tape, always using Storager 3030 tape (different IDs for different device names?)

        0xC -> Hard disk, always using Storager HD

        0xD / 0xE -> Tape, always using DSD tape controller (different IDs for different device names?)

        0xF -> Hard disk, always using DSD 

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
    
    I DOUBT MEMORY I/O AND DMA ARE THE SAME RANGE!

    Bit 7: Allow upper memory access (8-16mb)

    Bit 6: Allow lower memory access (0-7mb)

    Bit 5: Allow upper I/O access (8-16mb)

    Bit 4: Allow lower I/O access (0-7mb)

    Bit 3: Allow GL2 DMA access (8-11mb)

    Bit 2: Allow GF board access (I/O page 1)

    Bit 1: Allow UC board access (I/O page 3)

    Bit 0: Allow DC board access (I/O page 4)

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
