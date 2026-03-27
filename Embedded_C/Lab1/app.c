#include "uart.h"
unsigned char string_buf[100] = "learn-in-depth: Mahmoud";

int main()
{
	
	uart_send_string(string_buf);

	return 0;
}