# MicroBlaze MCS
notes on microblaze mcs application


1. only use microblaze to offload fpga complicated FSMs. Picoblaze does not support C compiler, so not use it.
2. only use microblaze MCS' MDM/IO BUS/GPI/GPO for simplicity. You have to choose MDM to enable SDK IDE debug&download. Without MDM you need compile .elf into bitstream and download it in vivado.
3. use register operation to speed GPIO/IOB BUS access(the driver consumes too many cycles), with it,
  - a GPI+GPO takes 14 cycles
  - a IO BUS write takes 13 cycles.
  - a IO BUS write plus some basic logics take 21 cycles
4. I did not try it, but here provides a good tutorial on interrupt handling.
  https://www.eit.lth.se/fileadmin/eit/courses/eit070/Laborationer/EIT070Lab04.pdf
   
   the original intc initialization has too many function calling and I think it consumes too much stack.
5. for the sample code, the release version consumes resources as below, 
  - text	   data	    bss	    dec	    hex	filename
  - 3488	    360	   1096	   4944	   1350	hh.elf
   
   only 1.3KB used, comparable to PicoBalze's 1KW instruction ROM
6. I have not yet handled QSPI flash, but some people are complaining its low performance.
7. Considering its poor performance, it is better to have an external cortex-M MCU if complicated logics exist.
