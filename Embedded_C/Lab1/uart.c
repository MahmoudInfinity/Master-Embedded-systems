#include "uart.h"

#define UART0DR *((volatile unsigned int *)(0x101F1000))


void uart_send_string(const unsigned char * p_tx_send)
{
   while(* p_tx_send != '\0')
   {

   	UART0DR = (unsigned int)(* p_tx_send);

   	 p_tx_send ++;

   }
  

}