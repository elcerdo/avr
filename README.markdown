tests
=====

test00
------

light some leds.
loading string from pgm space.

	PA0 -- P03 : leds
	PB0 : led


test01
------

k2000 light show.
timer0 test.
works with external cristal.

	PA0 -- PA6 : leds
	PB0 : led

test02
------

super tracker test.


scripts
=======

*   **read_fuse.sh** :
    read fuses and display their name. see page 105 on documentation.
*   **write_fuses_external_clock.sh** :
    set fuses to use external 8MHz cristal instread of internal clock (default).
