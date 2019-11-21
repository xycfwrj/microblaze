/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xiomodule.h" // add this file
#include "xil_exception.h"

#define IOBUS_write_data_addr(d,a)       *((volatile u32 *) (0xC0000000+a)) =(d)
#define IOBUS_read_data_addr(d,a)       d = *((volatile u32 *) (0xC0000000+a))


int main()
{
    u32 data,i,j,iold;
    u8 txcenter = 0;
    u16 txd[128];
    XIOModule gpi;
    XIOModule gpo;
    //init_platform();

    //print("Hello World\n\r");

    data = XIOModule_Initialize(&gpi, XPAR_IOMODULE_0_DEVICE_ID);
    data = XIOModule_Start(&gpi);

    data = XIOModule_Initialize(&gpo, XPAR_IOMODULE_0_DEVICE_ID);
    data = XIOModule_Start(&gpo);

     i=0;iold=0;
     j=0;

     for(i=0;i<128;i++)
    	 txd[i] = i+32;
     while (1)
     {
    	 //data = XIOModule_DiscreteRead(&gpi, 1); // read prf,cs (channel 1)
    	 i = *((volatile  u32 *)(gpi.BaseAddress + 0x20));
    	 //XIOModule_DiscreteWrite(&gpo, 1, i); // ack fpga (channel 1)
    	 //*((volatile u32 *)(gpo.BaseAddress+0x10)) = i;  //14 cycles for 1 read+write
    	 if(i>1)
    	 {
        	 if(iold<=1){ //prf _/~
        		 if(txcenter<128) txcenter++;
        		 else txcenter = 0;
        	 }
    		 //XIOModule_IoWriteWord(&gpo,j,j);  //45 cycle each write!
    		 IOBUS_write_data_addr(txd[(j+txcenter)&0x7f],j);  //21 cycle each write!(13 if j,j), @50MHz takes 0.26us for 1 dw, 128dw takes 33us!
    		 j+=4;
    	 }else j=0;

    	 iold = i;
     }

    //cleanup_platform();
    return 0;
}
