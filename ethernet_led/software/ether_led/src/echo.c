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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xparameters.h"
#include "xil_io.h"
#include "xgpio.h"

#include "lwip/err.h"
#include "lwip/tcp.h"
#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

XGpio led;

void AXI_GPIO_Init();
void set_led(char cmd[4]);

int transfer_data() {
	return 0;
}

void print_app_header()
{
	xil_printf("\n\r\n\r-----lwIP TCP echo server ------\n\r");
	xil_printf("TCP packets sent to port 6001 will be echoed back\n\r");
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)

{
	char cmd[3];
	char msg[17];

	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	/* Copy command */
	memcpy(cmd, p->payload, 4);

	/* Echo back the command */
	/* In this case, we assume that the msg is < TCP_SND_BUF (8192) */
	if (tcp_sndbuf(tpcb) > sizeof(msg))
	{
		sprintf(msg, "\nEntered Command: %s\n", (char*)p->payload);
		err = tcp_write(tpcb, msg, sizeof(msg), 1);
	}
	else
	{
		xil_printf("no space in tcp_sndbuf\n\r");
	}

	/* Process command */
	set_led(cmd);

	/* free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}


int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new();
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ADDR_ANY, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	/* Initialize AXI GPIO */
	AXI_GPIO_Init();

	return 0;
}

void AXI_GPIO_Init()
{
	XGpio_Initialize(&led, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&led, 1, 0x00000000);

	xil_printf("AXI GPIO initialized as output.\n");
}



void set_led(char cmd[4]){
	uint8_t led_value = 0;

    if (cmd[0] =='1'){
    	led_value = (led_value | (1 << 3));
    }
    else {
    	led_value = (led_value & ~(1 << 3));
    }

    if (cmd[1] =='1'){
       	led_value = (led_value | (1 << 2));
       }
       else {
       	led_value = (led_value & ~(1 << 2));
       }
    if (cmd[2] =='1'){
        	led_value = (led_value | (1 << 1));
        }
        else {
        	led_value = (led_value & ~(1 << 1));
        }

        if (cmd[3] =='1'){
           	led_value = (led_value | (1 << 0));
           }
           else {
           	led_value = (led_value & ~(1 << 0));
           }
        xil_printf("LED value = %X\n", led_value);
        XGpio_DiscreteWrite(&led, 1, led_value);
}




