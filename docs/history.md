# History

* 1979-1980: Original Geometry Engine paper.

* 1982: Wire-wrapped GF1 prototype.

* May 3, 1982: "TB1" board existed by this time (test board 1). The earliest date that development can be said to have started as the BPC1 debug tools were created.

* Late 1983 / Early 1984: IRIS 1000 released, shortly followed by IRIS 1200

* October 12, 1983 "PM2.1 Boot Proms V1.0" released.

* May 5, 1984: "SGI Unix System V" "beta-1.5" released

* October 1984: "IRIS Terminal Guide, Version 1.3"

* 1984: "IRIS Workstation Guide, Version 1.0" (1400 / 1500 released around this time)

* January 1985: PROM 3.x released

* Early 1985: Release W1.7 released

* Later in 1985: GL1-W2.1 released
    * Virtual memory
    * Stack probes removed
    * Remote graphics library

* August 1, 1985: 8MHz Geometry Engines released; 6MHz discontinued

* Autumn 1985: IRIS 2x00 Turbo (68020) released 

* February 1986: IRIS 3010 / 3020 / 3030 released

* Early 1986: 3.3.1 released

* May 1986: 2.4 / 3.4 released

* Late 1986 / Early 1987: 2.5 /3.5 releasd

* 1987: 3.6 released, IRIS 4D/60 released (MIPS-based)

* 1989: 3.7 worked on

# IRIS models

* November 1983: IRIS 1000 / 1200 - Terminal. PM1 (8MHz 68000) Sun licensed, EXOS101 Ethernet, GF1, UC3, DC3, BP2 (Iris 1200 has a floppy drive and 10 multibus slots) and "MicroRAM" Multibus RAM port. Operating system was simply called "SGI Unix System V" e.g. v1.5 with GL1 API
Same time-ish: IRIS 1400 / 1500: Workstation. PM2M1 (10MHz 68010) Sun licensed, Excelan EXOS101, but otherwise the same boards. but with a hard drive now. (ST506).

* 1984-85: IRIS 2000 / 2200: Terminal with PM2 bus and cut down GL2 OS. New GL2 API. GF2, UC4, DC4, BP3 boards. 6MHz GEs, later upgraded to 8MHz.  Excelan EXOS201 ethernet. By this time the software was called GL2 e.g. GL2-W2.5 (for workstation) or T2.5 (for terminal)

* 1984-85: IRIS 2300 / 2400 / 2500: Workstation and full GL2 OS. ST-506 for 2300/2400, ESDI(?) for 2500.

* IRIS 2300/2400/2500 Turbo: IRIS 2300/2400/2500 with Microram board and PM2M1 (aka IP1) processor board replaced with IP2 (Inhouse SGI) and IM1 board. These boards have a 16MHz 68020 instead of a 10mhz 68010 and the ram is on a private bus rather than Multibus so it's faster.

* IRIS 3000 is February 1986: 
 
* IRIS 3010 - Marketed as a terminal but can also run GL2 like 2000 / 2200 off a 20 MB HDD. 68020@16MHz. Basically the same as the IRIS 2300 Turbo.

* IRIS 3020 - Basically the IRIS 2400 Turbo but with a 70 MB ST-506 disk. 68020@16MHz. 

* IRIS 3030 - Uses 170 MB ESDI disk (Interphase Storager 3030). Otherwise the same as the 3020. 68020@16MHz.

IRIS 30x0's manufactured after August 1, 1985 have 8MHz Geometry Engines instead of 6MHz.

* Late 1986 or 1987(?):

* IRIS 31x0 - same as IRIS 3030, but the Geometry Engines now run at 10MHz and there are more of them (usually 12 instead of 8) on the GF2 boards.

* All Iris 3000s run GL2-W3.x instead of 2.x and can have an optional FP1 (Weitek) board too. 

4D/60 initially R2300, then IP4