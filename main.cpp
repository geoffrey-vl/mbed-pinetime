#include "mbed.h"

int main()
{
	int x = 5;
	while(true)
	{
		printf(0, "printf Test: %%d,      x : %d.\r\n", x);
		x++;
		ThisThread::sleep_for(1000);
	}

}