#include <avr/io.h>
#include <util/delay.h>

#include "state_machine.hpp"

int main(void)
{

	state_machine_c::init();

	while (1) {
		state_machine_c::run();
	}
}
