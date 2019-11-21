# microblaze
notes on microblaze mcs application


## 1.only use microblaze to offload fpga complicated FSMs. Picoblaze does not support C compiler, so not use it.
## 2.so only use microblaze MCS' MDM/IO BUS/GPI/GPO for simplicity.
## 3.use register operation to speed GPIO/IOB BUS access(xilinx driver consumes too many cycles), with it,
  - a GPI+GPO takes 14 cycles
  - a IO BUS write takes 21 cycles.
## 4.I did not try it, but here provides a good tutorial on interrupt handling.
  https://www.eit.lth.se/fileadmin/eit/courses/eit070/Laborationer/EIT070Lab04.pdf
  xilinx interrupt controller has too many function calling and I think it consumes too much stack.
## 5.the sample code, the release version consumes resources as below, 
   text	   data	    bss	    dec	    hex	filename
   3488	    360	   1096	   4944	   1350	hh.elf
   only 1.3KB used, comparable to PicoBalze's 1KW instructions.
