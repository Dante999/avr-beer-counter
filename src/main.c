#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "state_machine.h"

int main(void)
{

	state_machine_init();

	sei();

	while (1) {
		state_machine_run();
	}
}
