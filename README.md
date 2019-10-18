#  Modelling the Invisible #

Software for the Galton board exhibit used for [IPPP](https://www.ippp.dur.ac.uk/) [outreach](https://www.modellinginvisible.org/).
This includes a simulation of a Galton board, which we run on Raspberry Pis.

The exhibit also has a Galton board with light gates to count balls falling in different bins.
The code for this is in the folder `job 7139 galton board counter`.
It is for a Psoc 5lp development board ( CY8KIT-059 ) from Cypress, they come from Farnell/RS.
The software is written in Psoc creator which you can download from Cypress’s website.
There are LEDs with resistors in line to produce light and photo diodes and resistors to produce a pulse when the balls drop past the holes which the Psoc registers via interrupts.

## Dependancies

* Python 2.7
  * matplotlib
  * scipy

