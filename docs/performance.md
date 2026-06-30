# Performance

## Wireframe

* IRIS 1200 / 1400: 64,000 point transformations per second
* IRIS 3x00:        80,000 point transformations per second [[ResearchGate - 1987 - NASA](https://www.researchgate.net/publication/4706458_Flow_visualization_of_CFD_using_graphics_workstations)] ("10 MFLOPS")
* IRIS 4D/60:       140,000 point transformations ("4,500 3d poly's per frame")

## Flat-shaded

[[US Military Performance Analysis](https://apps.dtic.mil/sti/tr/pdf/ADA199562.pdf)]

IRIS 2400 (original graphics system) - 650 z-buffered, gouraud shaded polygons per second

IRIS 3120 (juniper, not sure if 8 or 10mhz generation):

(?20,000 wireframes per second?)

5,000 flat shaded, non-z-buffered polygons per second
1,000 gouraud shaded, z-buffered polygons per second

the zbuffer is implemented on the GAs, and i think gouraud is being done on the 2903 FBCs, this means that one of those is a 5-cycle thing, OUCH!

meaning:
~333 flat-shaded, non-z-buffered polygons per frame at 15fps
~67 flat-shaded, z-buffered polygons per frame at 15fps (this means the z-buffer GAs take ~5 cycles)

Clover1 graphics (4D/70G):

25,000 flat-shaded, non-z-buffered polygons per second
5,500 gouraud shaded, z-buffered polygons per second

meaning:
~1,667 flat-shaded, non-zbuffered polygons @ 15fps
~367 gouraud shaded, z-buffered polygons @ 15fps

Clover2 graphics (4D/70GT):

60,000 flat-shaded, non-z-buffered polygons per second
32,500 gouraud shaded, z-buffered polygons per second (this means Clover2 can do 2-cycle gouraud shading or 2-cycle z-buffer.)

meaning:
~4,000 flat-shaded, non-zbuffered polygons @ 15fps
~2,167 gouraud shaded, z-buffered polygons @ 15fps

apparently meshes had better perf.