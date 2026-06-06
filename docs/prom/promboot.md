IRIS PROM Boot process:

"evec.h" [gl2/stand/mon/evec.h]

1. CPU board IP2 comes up.
2. Ip2 Memory map by default:
    See ip2map.md
    
"locore.c" [gl2/stand/mon/locore.c]

start: 

Boot Phase 0 (pre-LEDs)

3. Store system initialisation level in D1.
   Store 0 in D2.

4. Enter supervisor mode and set interrupt priority to 7
5. Reprogram the interrupt vector base register to point at 30000000h PROM text
6. Turn off cache (CACR <- 0)
7. For DUART 0: 
    Write 0xEB to register 04h (04h on write is Aux. Control Register):
        - BRG set 2
        - Timer mode
        - Crystal or external clock (not divided)
        - Enable change of state interrupts for IP3, IP1, IP0
    Set counter refresh rate to 14
    Set OPCR (DUART register 13h, write) to 0xF4
        - OP7 pin -> Channel B tranmitter interrupt output (complement of TxRDYB, not masked, open-drain)
        - OP6 pin -> Channel B transmitter interrupt output (complement of TxRDYA, not masked, open-drain)
        - OP5 pin -> Channel A transmitter interrupt output (complement of ISR[5], not masked, open-drain)
        - OP4 pin -> Channel A receiver interrupt output (complement of ISR[1], not masked, open-drain)
        - OP3 pin -> counter/timer output
        - OP2 pin -> complement of OPR[2]
8. Determine board revision by reading bit 5 of mouse button register
9. If Rev A, skip to step 11 (step 10 is done later, by Unix.)
10. Write same values as above to DUART 1, then skip to step 12.
11. Configure "master or slave" status (which determines if the pipe and DUARTs are driven by this machine or not. Presumably for networking multiple machines together...)
    -> Set Status and Parity Control registers to 0
    -> If SW_MASTERSLV bit 4 is set, we are a slave, otherwise master.
    -> Set debug LEDs to 0

    If we are a slave:
        -> Allow Multibus access
        -> Disable "Pipe" and DUART accesses (Pipe is probably the internal bus between vram, gpu and cpu)
    
    If we are a master:
        -> Enable Pipe, DUARTs and Multibus


Init phase 0 ("initdone"):

12. Disable user-mode access to MUltibus by setting Multibus Protect register to zero
13. Reset the initial stack pointer
14. Clear "private" part of 2 KB "Boot RAM" (as it's static RAM)
15. Increment initialisation level
16. Remove previous init call from the stack (what?)
16. Call confidence tests

Init phase 1 ("confid"):

17. Test getting the values of the SR, VBR, CACR, CAAR, SFC, DFC registers. Test setting the values of VBR, CACR, CAAR, SFC, DFC registers (otherwise we'd leave SR mode!). 
18. Test memory word alignment.
19. Zero memory.
