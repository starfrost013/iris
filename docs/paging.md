4KB pages (1 MB = 256 pages)

Maximum 32 MB RAM

PTE format:

    bit 31: dirty bit

    bit 30: referenced bit

    bits 29:28: protect info

        - 0 = no access

        - 1 = read access

        - 2 = system (PROM) access only

        - 3 = read/write

    bits 28:13: unused

    bits 12:0 - page number

MMU segment / address space types:

    0 - "Test" / Data Segment

    1 - Stack

    2 - Kernel

    3 - PROM ("System")

    4 - Multibus Memory

    5 - Multibus I/O

    6 - Pipebus (Geometry Engine)

    7 - Pipebus (Floating point board)